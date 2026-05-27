#pragma once

#include "ScalarAnimation.g.h"
#include "../Abstract/CustomAnimation{TValue,TKeyFrame}.h"
#include "../KeyFrames/ScalarKeyFrame.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ScalarAnimation;

    struct ScalarAnimationTraits
    {
        using owner_type = ScalarAnimation;
        using class_type = Animations::ScalarAnimation;
        using public_value_type = winrt::Windows::Foundation::IReference<double>;
        using parsed_value_type = double;
        using keyframe_impl_type = implementation::ScalarKeyFrame;
        using keyframe_type = Animations::ScalarKeyFrame;
        static std::optional<parsed_value_type> Parse(winrt::Windows::Foundation::IReference<double> const& value)
        {
            return value;
        }
    };

    struct ScalarAnimation : ScalarAnimationT<ScalarAnimation, CustomAnimationBase<ScalarAnimationTraits>> {};
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ScalarAnimation : ScalarAnimationT<ScalarAnimation, implementation::ScalarAnimation> {};
}
