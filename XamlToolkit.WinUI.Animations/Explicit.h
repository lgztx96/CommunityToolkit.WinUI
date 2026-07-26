#pragma once

#include "Explicit.g.h"
#include "Xaml/AnimationDictionary.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// Attached properties to support explicitly triggered animations for UIElement instances.
    /// </summary>
    struct Explicit
    {
        /// <summary>
        /// Gets the value of the AnimationsProperty property.
        /// </summary>
        static winrt::XamlToolkit::WinUI::Animations::AnimationDictionary GetAnimations(winrt::UIElement const& element);

        /// <summary>
        /// Sets the value of the AnimationsProperty property.
        /// </summary>
        static void SetAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::AnimationDictionary const& value);

        /// <summary>
        /// Identifies the AnimationsProperty dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> AnimationsProperty;

    private:
        static void OnAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Explicit : ExplicitT<Explicit, implementation::Explicit> {};
}
