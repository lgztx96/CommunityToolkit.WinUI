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
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Hosting;
	using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<DependencyProperty> ItemsReorderAnimation::DurationProperty = DependencyProperty::RegisterAttached(
        L"Duration",
        winrt::xaml_typename<winrt::Windows::Foundation::TimeSpan>(),
        winrt::xaml_typename<class_type>(),
        nullptr);

    const wil::single_threaded_property<DependencyProperty> ItemsReorderAnimation::ReorderAnimationProperty = DependencyProperty::RegisterAttached(
        L"ReorderAnimation",
        winrt::xaml_typename<ImplicitAnimationCollection>(),
        winrt::xaml_typename<class_type>(),
        nullptr);

    winrt::Windows::Foundation::TimeSpan ItemsReorderAnimation::GetDuration(ListViewBase const& listView)
    {
        return winrt::unbox_value<winrt::Windows::Foundation::TimeSpan>(listView.GetValue(DurationProperty));
    }

    void ItemsReorderAnimation::SetDuration(ListViewBase const& listView, winrt::Windows::Foundation::TimeSpan const& value)
    {
        listView.SetValue(DurationProperty, winrt::box_value(value));
    }

    void ItemsReorderAnimation::OnDurationChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        if (Windows::ApplicationModel::DesignMode::DesignModeEnabled())
        {
            return;
        }

        if (auto listView = d.try_as<ListViewBase>())
        {
            auto duration = winrt::unbox_value<winrt::Windows::Foundation::TimeSpan>(e.NewValue());
            AssignReorderAnimation(listView, duration);

            listView.ContainerContentChanging({ &ItemsReorderAnimation::OnContainerContentChanging });
            listView.ChoosingItemContainer({ &ItemsReorderAnimation::OnChoosingItemContainer });
        }
    }

    void ItemsReorderAnimation::AssignReorderAnimation(ListViewBase const& listView, winrt::Windows::Foundation::TimeSpan const& duration)
    {
        auto visual = ElementCompositionPreview::GetElementVisual(listView);
        auto compositor = visual.Compositor();
        auto animationCollection = listView.GetValue(ReorderAnimationProperty).try_as<ImplicitAnimationCollection>();

        if (!animationCollection)
        {
            animationCollection = compositor.CreateImplicitAnimationCollection();
            listView.SetValue(ReorderAnimationProperty, animationCollection);
        }

        if (duration == winrt::Windows::Foundation::TimeSpan{ 0 })
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

    void ItemsReorderAnimation::OnContainerContentChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args)
    {
        if (args.InRecycleQueue())
        {
            PokeUIElementZIndex(args.ItemContainer());
        }
        else
        {
            auto visual = ElementCompositionPreview::GetElementVisual(args.ItemContainer());
            auto animationCollection = sender.GetValue(ReorderAnimationProperty).try_as<ImplicitAnimationCollection>();
            visual.ImplicitAnimations(animationCollection);
        }
    }

    void ItemsReorderAnimation::OnChoosingItemContainer([[maybe_unused]] ListViewBase const& sender, ChoosingItemContainerEventArgs const& args)
    {
        if (args.ItemContainer())
        {
            PokeUIElementZIndex(args.ItemContainer());
        }
    }

    void ItemsReorderAnimation::PokeUIElementZIndex(UIElement const& element)
    {
        auto oldZIndex = Controls::Canvas::GetZIndex(element);
        Controls::Canvas::SetZIndex(element, oldZIndex + 1);
        Controls::Canvas::SetZIndex(element, oldZIndex);
    }
}
