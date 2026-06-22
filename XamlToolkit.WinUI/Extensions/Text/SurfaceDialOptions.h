#pragma once

#include "SurfaceDialOptions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Input.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::Input;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct SurfaceDialOptions : SurfaceDialOptionsT<SurfaceDialOptions>
    {
        SurfaceDialOptions() = default;

        static XamlToolkit::WinUI::SurfaceDialOptions Default();

        wil::single_threaded_rw_property<RadialControllerMenuKnownIcon> Icon = RadialControllerMenuKnownIcon::Ruler;
        wil::single_threaded_rw_property<double> StepValue{ 0.0 };
        wil::single_threaded_rw_property<bool> EnableHapticFeedback{ true };
        wil::single_threaded_rw_property<double> RotationResolutionInDegrees{ 10.0 };
        wil::single_threaded_rw_property<double> MinValue{ -100.0 };
        wil::single_threaded_rw_property<double> MaxValue{ 100.0 };
        wil::single_threaded_rw_property<bool> EnableTapToNextControl{ true };
        wil::single_threaded_rw_property<bool> EnableMinMaxValue{ false };
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct SurfaceDialOptions : SurfaceDialOptionsT<SurfaceDialOptions, implementation::SurfaceDialOptions>
    {
    };
}
