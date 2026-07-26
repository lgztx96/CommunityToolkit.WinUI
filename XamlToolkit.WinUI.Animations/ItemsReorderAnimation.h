#pragma once

#include "ItemsReorderAnimation.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// Provides the ability to assign a reorder animation to a ListViewBase.
    /// </summary>
    struct ItemsReorderAnimation
    {
        /// <summary>
        /// Gets the duration of the reorder animation.
        /// </summary>
        static winrt::TimeSpan GetDuration(winrt::ListViewBase const& listView);

        /// <summary>
        /// Sets the duration of the reorder animation.
        /// </summary>
        static void SetDuration(winrt::ListViewBase const& listView, winrt::TimeSpan const& value);

        /// <summary>
        /// Identifies the DurationProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> DurationProperty;

        /// <summary>
        /// Identifies the attached "ReorderAnimation" <see cref="DependencyProperty"/>.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ReorderAnimationProperty;

    private:
        static void OnDurationChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
        static void AssignReorderAnimation(winrt::ListViewBase const& listView, winrt::TimeSpan const& duration);
        static void OnContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);
        static void OnChoosingItemContainer(winrt::ListViewBase const& sender, winrt::ChoosingItemContainerEventArgs const& args);
        static void PokeUIElementZIndex(winrt::UIElement const& element);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ItemsReorderAnimation : ItemsReorderAnimationT<ItemsReorderAnimation, implementation::ItemsReorderAnimation> {};
}
