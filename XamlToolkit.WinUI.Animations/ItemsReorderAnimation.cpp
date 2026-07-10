#include "pch.h"
#include "winrt_module_imports.h"
#include "ItemsReorderAnimation.h"
#if __has_include("ItemsReorderAnimation.g.cpp")
#include "ItemsReorderAnimation.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <winrt/Windows.ApplicationModel.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Hosting;
	using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ItemsReorderAnimation::DurationProperty = 
        winrt::DependencyProperty::RegisterAttached(
            L"Duration",
            winrt::xaml_typename<winrt::TimeSpan>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    const wil::single_threaded_property<winrt::DependencyProperty> ItemsReorderAnimation::ReorderAnimationProperty = 
        winrt::DependencyProperty::RegisterAttached(
            L"ReorderAnimation",
            winrt::xaml_typename<winrt::ImplicitAnimationCollection>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    winrt::TimeSpan ItemsReorderAnimation::GetDuration(winrt::ListViewBase const& listView)
    {
        return winrt::unbox_value<winrt::TimeSpan>(listView.GetValue(DurationProperty()));
    }

    void ItemsReorderAnimation::SetDuration(winrt::ListViewBase const& listView, winrt::TimeSpan const& value)
    {
        listView.SetValue(DurationProperty(), winrt::box_value(value));
    }

    void ItemsReorderAnimation::OnDurationChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (winrt::Windows::ApplicationModel::DesignMode::DesignModeEnabled())
        {
            return;
        }

        if (auto listView = d.try_as<winrt::ListViewBase>())
        {
            auto duration = winrt::unbox_value<winrt::TimeSpan>(e.NewValue());
            AssignReorderAnimation(listView, duration);

            listView.ContainerContentChanging({ &ItemsReorderAnimation::OnContainerContentChanging });
            listView.ChoosingItemContainer({ &ItemsReorderAnimation::OnChoosingItemContainer });
        }
    }

    void ItemsReorderAnimation::AssignReorderAnimation(winrt::ListViewBase const& listView, winrt::TimeSpan const& duration)
    {
        auto visual = winrt::ElementCompositionPreview::GetElementVisual(listView);
        auto compositor = visual.Compositor();
        auto animationCollection = listView.GetValue(ReorderAnimationProperty()).try_as<winrt::ImplicitAnimationCollection>();

        if (!animationCollection)
        {
            animationCollection = compositor.CreateImplicitAnimationCollection();
            listView.SetValue(ReorderAnimationProperty(), animationCollection);
        }

        if (duration == winrt::TimeSpan{ 0 })
        {
            animationCollection.Remove(L"Offset");
        }
        else
        {
            auto offsetAnimation = compositor.CreateVector3KeyFrameAnimation();
            offsetAnimation.InsertExpressionKeyFrame(1.0f, L"this.FinalValue");
            offsetAnimation.Duration(duration);
            offsetAnimation.Target(L"Offset");

            auto animationGroup = compositor.CreateAnimationGroup();
            animationGroup.Add(offsetAnimation);

            animationCollection.Insert(L"Offset", animationGroup);
        }
    }

    void ItemsReorderAnimation::OnContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args)
    {
        if (args.InRecycleQueue())
        {
            PokeUIElementZIndex(args.ItemContainer());
        }
        else
        {
            auto visual = winrt::ElementCompositionPreview::GetElementVisual(args.ItemContainer());
            auto animationCollection = sender.GetValue(ReorderAnimationProperty()).try_as<winrt::ImplicitAnimationCollection>();
            visual.ImplicitAnimations(animationCollection);
        }
    }

    void ItemsReorderAnimation::OnChoosingItemContainer([[maybe_unused]] winrt::ListViewBase const& sender, winrt::ChoosingItemContainerEventArgs const& args)
    {
        if (args.ItemContainer())
        {
            PokeUIElementZIndex(args.ItemContainer());
        }
    }

    void ItemsReorderAnimation::PokeUIElementZIndex(winrt::UIElement const& element)
    {
        auto oldZIndex = winrt::Canvas::GetZIndex(element);
        winrt::Canvas::SetZIndex(element, oldZIndex + 1);
        winrt::Canvas::SetZIndex(element, oldZIndex);
    }
}
