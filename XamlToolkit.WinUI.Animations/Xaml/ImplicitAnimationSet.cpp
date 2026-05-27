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
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;

    namespace
    {
        IImplicitTimeline* AsInplicitTimeline(DependencyObject const& item)
        {
            auto animationNode = item.try_as<winrt::XamlToolkit::WinUI::Animations::Animation>();

            if (!animationNode)
            {
                throw winrt::hresult_invalid_argument(L"ImplicitAnimationSet only accepts Animation timelines.");
            }

            auto animationNodeImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Animations::implementation::Animation>(animationNode);
            auto timeline = dynamic_cast<IImplicitTimeline*>(animationNodeImpl);

            if (!timeline)
            {
                throw winrt::hresult_invalid_argument(L"ImplicitAnimationSet only accepts IImplicitTimeline nodes.");
            }

            return timeline;
        }
    }

    ImplicitAnimationSet::ImplicitAnimationSet()
    {
        vectorChangedRevoker = VectorChanged(winrt::auto_revoke, { this, &ImplicitAnimationSet::OnVectorChanged });
    }

    winrt::weak_ref<UIElement> ImplicitAnimationSet::ParentReference() const
    {
        return parent;
    }

    void ImplicitAnimationSet::ParentReference(winrt::weak_ref<UIElement> const& value)
    {
        parent = value;
    }

    void ImplicitAnimationSet::OnVectorChanged(Windows::Foundation::Collections::IObservableVector<DependencyObject> const& sender, Windows::Foundation::Collections::IVectorChangedEventArgs const& event)
    {
        if (event.CollectionChange() == Windows::Foundation::Collections::CollectionChange::ItemInserted ||
            event.CollectionChange() == Windows::Foundation::Collections::CollectionChange::ItemChanged)
        {
            auto timeline = AsInplicitTimeline(sender.GetAt(event.Index()));
            if (auto it = animationPropertyChangedEventTokens.find(timeline); it != animationPropertyChangedEventTokens.end())
            {
                timeline->AnimationPropertyChanged(it->second);
            }

            animationPropertyChangedEventTokens[timeline] = timeline->AnimationPropertyChanged({ this, &ImplicitAnimationSet::RaiseAnimationsChanged });
        }

        AnimationsChanged.invoke(*this, nullptr);
    }

    void ImplicitAnimationSet::RaiseAnimationsChanged([[maybe_unused]] IInspectable const& sender, IInspectable const& e)
    {
        AnimationsChanged.invoke(*this, e);
    }

    CompositionAnimationGroup ImplicitAnimationSet::GetCompositionAnimationGroup(UIElement const& element)
    {
        auto visual = ElementCompositionPreview::GetElementVisual(element);
        auto animations = visual.Compositor().CreateAnimationGroup();

        for (auto const& item : *this)
        {
            auto timeline = AsInplicitTimeline(item);
            winrt::hstring key;
            auto animation = timeline->GetAnimation(element, key);
            animations.Add(animation);
        }

        return animations;
    }

    ImplicitAnimationCollection ImplicitAnimationSet::GetImplicitAnimationCollection(UIElement const& element)
    {
        auto visual = ElementCompositionPreview::GetElementVisual(element);
        auto compositor = visual.Compositor();
        auto animations = compositor.CreateImplicitAnimationCollection();

        for (auto const& item : *this)
        {
            auto timeline = AsInplicitTimeline(item);
            winrt::hstring key;
            auto animation = timeline->GetAnimation(element, key);

            if (key.empty())
            {
                key = animation.Target();
            }

            if (!animations.HasKey(key))
            {
                animations.Insert(key, compositor.CreateAnimationGroup());
            }

            animations.Lookup(key).as<CompositionAnimationGroup>().Add(animation);
        }

        return animations;
    }
}
