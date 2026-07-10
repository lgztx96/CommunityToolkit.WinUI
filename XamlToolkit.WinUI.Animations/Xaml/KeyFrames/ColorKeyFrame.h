#pragma once

#include "ColorKeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"

namespace winrt 
{
    using namespace Windows::UI;
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ColorKeyFrame : ColorKeyFrameT<ColorKeyFrame, KeyFrameBase<winrt::IReference<winrt::Color>, winrt::Color>>
    {
    public:
        ColorKeyFrame() = default;

        winrt::IReference<winrt::Color> Value() const
        {
            return GetValue(ValueProperty()).try_as<winrt::IReference<winrt::Color>>();
        }
        void Value(winrt::IReference<winrt::Color> const& value)
        {
            SetValue(ValueProperty(), winrt::box_value(value));
        }

    protected:
        std::optional<winrt::Color> GetParsedValue() const override
        {
            return Value();
        }

    public:
        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ColorKeyFrame : ColorKeyFrameT<ColorKeyFrame, implementation::ColorKeyFrame> {};
}
