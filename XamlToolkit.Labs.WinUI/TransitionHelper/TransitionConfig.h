#pragma once

#include "TransitionConfig.g.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace ::winrt::Microsoft::UI::Xaml::Media::Animation;
    using namespace ::winrt::Windows::Foundation;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct TransitionConfig : TransitionConfigT<TransitionConfig>
    {
        TransitionConfig() = default;

        wil::single_threaded_rw_property<winrt::hstring> Id;
        wil::single_threaded_rw_property<enum ScaleMode> ScaleMode{ ScaleMode::None };
        wil::single_threaded_rw_property<winrt::XamlToolkit::Labs::WinUI::IScalingCalculator> CustomScalingCalculator{ nullptr };
        wil::single_threaded_rw_property<bool> EnableClipAnimation{ false };
        wil::single_threaded_rw_property<winrt::Point> NormalizedCenterPoint{};
        wil::single_threaded_rw_property<winrt::IReference<winrt::XamlToolkit::WinUI::Animations::EasingType>> EasingType{ nullptr };
        wil::single_threaded_rw_property<winrt::IReference<winrt::EasingMode>> EasingMode{ nullptr };
        wil::single_threaded_rw_property<winrt::IReference<winrt::Point>> OpacityTransitionProgressKey{ nullptr };
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct TransitionConfig : TransitionConfigT<TransitionConfig, implementation::TransitionConfig>
    {
    };
}
