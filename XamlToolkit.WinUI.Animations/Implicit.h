#pragma once

#include "Implicit.g.h"
#include "Xaml/ImplicitAnimationSet.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;
}

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
        static winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet GetShowAnimations(winrt::UIElement const& element);

        /// <summary>
        /// Sets the value of the ShowAnimationsProperty property.
        /// </summary>
        static void SetShowAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet const& value);

        /// <summary>
        /// Gets the value of the HideAnimationsProperty property.
        /// </summary>
        static winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet GetHideAnimations(winrt::UIElement const& element);

        /// <summary>
        /// Sets the value of the HideAnimationsProperty property.
        /// </summary>
        static void SetHideAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet const& value);

        /// <summary>
        /// Gets the value of the AnimationsProperty property.
        /// </summary>
        static winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet GetAnimations(winrt::UIElement const& element);

        /// <summary>
        /// Sets the value of the AnimationsProperty property.
        /// </summary>
        static void SetAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet const& value);

        /// <summary>
        /// Identifies the ShowAnimationsProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ShowAnimationsProperty;

        /// <summary>
        /// Identifies the HideAnimationsProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> HideAnimationsProperty;

        /// <summary>
        /// Identifies the AnimationsProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> AnimationsProperty;

    private:
        static void OnShowAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
        static void OnHideAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
        static void OnAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Implicit : ImplicitT<Implicit, implementation::Implicit> {};
}
