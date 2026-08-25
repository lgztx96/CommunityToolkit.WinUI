#include "pch.h"
#include "winrt_module_imports.h"
#include "ImplicitAnimationSet.h"
#if __has_include("ImplicitAnimationSet.g.cpp")
#include "ImplicitAnimationSet.g.cpp"
#endif
#include "Abstract/Animation.h"

#ifdef __INTELLISENSE__
#include "Xaml/Interfaces/IImplicitTimeline.h"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    namespace
    {
        IImplicitTimeline* AsImplicitTimeline(winrt::DependencyObject const& item)
        {
            const auto animationNode = item.try_as<winrt::XamlToolkit::WinUI::Animations::Animation>();

            if (!animationNode)
            {
                throw winrt::hresult_invalid_argument(L"ImplicitAnimationSet only accepts Animation timelines.");
            }

            const auto animationNodeImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Animations::implementation::Animation>(animationNode);
            const auto timeline = dynamic_cast<IImplicitTimeline*>(animationNodeImpl);

            if (!timeline)
            {
                throw winrt::hresult_invalid_argument(L"ImplicitAnimationSet only accepts IImplicitTimeline nodes.");
            }

            return timeline;
        }
    }

    ImplicitAnimationSet::ImplicitAnimationSet()
    {
        VectorChanged(winrt::auto_revoke, { this, &ImplicitAnimationSet::OnVectorChanged });
    }

    winrt::weak_ref<winrt::UIElement> ImplicitAnimationSet::ParentReference() const noexcept
    {
        return _parent;
    }

    void ImplicitAnimationSet::ParentReference(winrt::weak_ref<winrt::UIElement> const& value) noexcept
    {
        _parent = value;
    }

    void ImplicitAnimationSet::OnVectorChanged(winrt::IObservableVector<DependencyObject> const& sender, winrt::IVectorChangedEventArgs const& event)
    {
        if (event.CollectionChange() == winrt::CollectionChange::ItemInserted ||
            event.CollectionChange() == winrt::CollectionChange::ItemChanged)
        {
            const auto timeline = AsImplicitTimeline(sender.GetAt(event.Index()));
            if (const auto it = _itemsEventTokens.find(timeline); it != _itemsEventTokens.end())
            {
                timeline->AnimationPropertyChanged(it->second);
            }

            _itemsEventTokens[timeline] = timeline->AnimationPropertyChanged({ get_weak(), &ImplicitAnimationSet::RaiseAnimationsChanged});
        }

        AnimationsChanged.invoke(*this, nullptr);
    }

    void ImplicitAnimationSet::RaiseAnimationsChanged([[maybe_unused]] winrt::IInspectable const& sender, winrt::IInspectable const& e)
    {
        AnimationsChanged.invoke(*this, e);
    }

    winrt::CompositionAnimationGroup ImplicitAnimationSet::GetCompositionAnimationGroup(winrt::UIElement const& element)
    {
        const auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
        const auto animations = visual.Compositor().CreateAnimationGroup();

        for (const auto& item : *this)
        {
            const auto timeline = AsImplicitTimeline(item);
            winrt::hstring key;
            auto animation = timeline->GetAnimation(element, key);
            animations.Add(animation);
        }

        return animations;
    }

    winrt::ImplicitAnimationCollection ImplicitAnimationSet::GetImplicitAnimationCollection(winrt::UIElement const& element)
    {
        const auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
        const auto compositor = visual.Compositor();
        const auto animations = compositor.CreateImplicitAnimationCollection();

        for (const auto& item : *this)
        {
            auto timeline = AsImplicitTimeline(item);
            winrt::hstring key;
            const auto animation = timeline->GetAnimation(element, key);

            if (key.empty())
            {
                key = animation.Target();
            }

            if (!animations.HasKey(key))
            {
                animations.Insert(key, compositor.CreateAnimationGroup());
            }

            animations.Lookup(key).as<winrt::CompositionAnimationGroup>().Add(animation);
        }

        return animations;
    }
}
