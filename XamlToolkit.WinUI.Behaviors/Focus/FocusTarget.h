#pragma once

#include "FocusTarget.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// A target for the FocusBehavior.
    /// </summary>
    struct FocusTarget : FocusTargetT<FocusTarget>
    {
        FocusTarget() = default;

        /// <summary>
        /// Gets or sets the control that will receive the focus.
        /// </summary>
        winrt::Control Control() const
        {
            return GetValue(ControlProperty()).try_as<winrt::Control>();
        }
        void Control(winrt::Control const& value)
        {
            SetValue(ControlProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Raised when Control property changed.
        /// It can change if we use x:Load on the control.
        /// </summary>
        wil::untyped_event<winrt::IInspectable> ControlChanged;

        static const wil::single_threaded_property<winrt::DependencyProperty> ControlProperty;

    private:
        static void OnControlChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& args);
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct FocusTarget : FocusTargetT<FocusTarget, implementation::FocusTarget>
    {
    };
}
