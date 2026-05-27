#pragma once

#include "Implicit.g.h"
#include "Xaml/ImplicitAnimationSet.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// Attached properties to support implicitly triggered animations for UIElement instances.
    /// </summary>
    struct Implicit : ImplicitT<Implicit>
    {
    public:
        Implicit() = default;

        /// <summary>
        /// Gets the value of the ShowAnimationsProperty property.
        /// </summary>
        static Animations::ImplicitAnimationSet GetShowAnimations(Microsoft::UI::Xaml::UIElement const& element);

        /// <summary>
        /// Sets the value of the ShowAnimationsProperty property.
        /// </summary>
        static void SetShowAnimations(Microsoft::UI::Xaml::UIElement const& element, Animations::ImplicitAnimationSet const& value);

        /// <summary>
        /// Gets the value of the HideAnimationsProperty property.
        /// </summary>
        static Animations::ImplicitAnimationSet GetHideAnimations(Microsoft::UI::Xaml::UIElement const& element);

        /// <summary>
        /// Sets the value of the HideAnimationsProperty property.
        /// </summary>
        static void SetHideAnimations(Microsoft::UI::Xaml::UIElement const& element, Animations::ImplicitAnimationSet const& value);

        /// <summary>
        /// Gets the value of the AnimationsProperty property.
        /// </summary>
        static Animations::ImplicitAnimationSet GetAnimations(Microsoft::UI::Xaml::UIElement const& element);

        /// <summary>
        /// Sets the value of the AnimationsProperty property.
        /// </summary>
        static void SetAnimations(Microsoft::UI::Xaml::UIElement const& element, Animations::ImplicitAnimationSet const& value);

        /// <summary>
        /// Identifies the ShowAnimationsProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<DependencyProperty> ShowAnimationsProperty;

        /// <summary>
        /// Identifies the HideAnimationsProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<DependencyProperty> HideAnimationsProperty;

        /// <summary>
        /// Identifies the AnimationsProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<DependencyProperty> AnimationsProperty;

    private:
        static void OnShowAnimationsPropertyChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
        static void OnHideAnimationsPropertyChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
        static void OnAnimationsPropertyChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Implicit : ImplicitT<Implicit, implementation::Implicit> {};
}
