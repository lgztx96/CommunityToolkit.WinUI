#pragma once

#include "ColorKeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ColorKeyFrame : ColorKeyFrameT<ColorKeyFrame, KeyFrameBase<winrt::Windows::Foundation::IReference<Windows::UI::Color>, Windows::UI::Color>>
    {
    public:
        ColorKeyFrame() = default;

        winrt::Windows::Foundation::IReference<Windows::UI::Color> Value() const
        {
            return GetValue(ValueProperty).try_as<winrt::Windows::Foundation::IReference<Windows::UI::Color>>();
        }
        void Value(winrt::Windows::Foundation::IReference<Windows::UI::Color> const& value)
        {
            SetValue(ValueProperty, winrt::box_value(value));
        }

    protected:
        std::optional<Windows::UI::Color> GetParsedValue() const override
        {
            return Value();
        }

    public:
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ColorKeyFrame : ColorKeyFrameT<ColorKeyFrame, implementation::ColorKeyFrame> {};
}
