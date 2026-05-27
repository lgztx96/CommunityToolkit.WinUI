#pragma once

#include "ScalarKeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ScalarKeyFrame : ScalarKeyFrameT<ScalarKeyFrame, KeyFrameBase<winrt::Windows::Foundation::IReference<double>, double>>
    {
    public:
        ScalarKeyFrame() = default;

        winrt::Windows::Foundation::IReference<double> Value() const
        {
            return GetValue(ValueProperty).try_as<winrt::Windows::Foundation::IReference<double>>();
        }
        void Value(winrt::Windows::Foundation::IReference<double> const& value)
        {
            SetValue(ValueProperty, winrt::box_value(value));
        }

    protected:
        std::optional<double> GetParsedValue() const override
        {
            return Value();
        }

    public:
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ScalarKeyFrame : ScalarKeyFrameT<ScalarKeyFrame, implementation::ScalarKeyFrame> {};
}
