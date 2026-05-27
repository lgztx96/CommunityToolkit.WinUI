#pragma once

#include "ItemsReorderAnimation.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// Provides the ability to assign a reorder animation to a ListViewBase.
    /// </summary>
    struct ItemsReorderAnimation : ItemsReorderAnimationT<ItemsReorderAnimation>
    {
    public:
        ItemsReorderAnimation() = default;

        /// <summary>
        /// Gets the duration of the reorder animation.
        /// </summary>
        static Windows::Foundation::TimeSpan GetDuration(Microsoft::UI::Xaml::Controls::ListViewBase const& listView);

        /// <summary>
        /// Sets the duration of the reorder animation.
        /// </summary>
        static void SetDuration(Microsoft::UI::Xaml::Controls::ListViewBase const& listView, Windows::Foundation::TimeSpan const& value);

        /// <summary>
        /// Identifies the DurationProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> DurationProperty;

        /// <summary>
        /// Identifies the attached "ReorderAnimation" <see cref="DependencyProperty"/>.
        /// </summary>
        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ReorderAnimationProperty;

    private:
        static void OnDurationChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
        static void AssignReorderAnimation(Microsoft::UI::Xaml::Controls::ListViewBase const& listView, Windows::Foundation::TimeSpan const& duration);
        static void OnContainerContentChanging(Microsoft::UI::Xaml::Controls::ListViewBase const& sender, Microsoft::UI::Xaml::Controls::ContainerContentChangingEventArgs const& args);
        static void OnChoosingItemContainer(Microsoft::UI::Xaml::Controls::ListViewBase const& sender, Microsoft::UI::Xaml::Controls::ChoosingItemContainerEventArgs const& args);
        static void PokeUIElementZIndex(Microsoft::UI::Xaml::UIElement const& element);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ItemsReorderAnimation : ItemsReorderAnimationT<ItemsReorderAnimation, implementation::ItemsReorderAnimation> {};
}
