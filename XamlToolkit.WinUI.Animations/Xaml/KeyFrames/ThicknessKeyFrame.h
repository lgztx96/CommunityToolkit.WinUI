#pragma once

#include "ThicknessKeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ThicknessKeyFrame : ThicknessKeyFrameT<ThicknessKeyFrame, KeyFrameBase<winrt::Windows::Foundation::IReference<Thickness>, Thickness>>
    {
    public:
        ThicknessKeyFrame() = default;

        winrt::Windows::Foundation::IReference<Thickness> Value() const
        {
            return GetValue(ValueProperty).try_as<winrt::Windows::Foundation::IReference<Thickness>>();
        }
        void Value(winrt::Windows::Foundation::IReference<Thickness> const& value)
        {
            SetValue(ValueProperty, winrt::box_value(value));
        }

    protected:
        std::optional<Thickness> GetParsedValue() const override
        {
            return Value();
        }

    public:
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ThicknessKeyFrame : ThicknessKeyFrameT<ThicknessKeyFrame, implementation::ThicknessKeyFrame>
    {
    };
}
