#pragma once

#include "ScalarKeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ScalarKeyFrame : ScalarKeyFrameT<ScalarKeyFrame, KeyFrameBase<winrt::IReference<double>, double>>
    {
    public:
        ScalarKeyFrame() = default;

        winrt::IReference<double> Value() const
        {
            return GetValue(ValueProperty()).try_as<winrt::IReference<double>>();
        }
        void Value(winrt::IReference<double> const& value)
        {
            SetValue(ValueProperty(), winrt::box_value(value));
        }

    protected:
        std::optional<double> GetParsedValue() const override
        {
            return Value();
        }

    public:
        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ScalarKeyFrame : ScalarKeyFrameT<ScalarKeyFrame, implementation::ScalarKeyFrame> {};
}
