#pragma once

#include "ThicknessKeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ThicknessKeyFrame : ThicknessKeyFrameT<ThicknessKeyFrame, KeyFrameBase<winrt::IReference<winrt::Thickness>, winrt::Thickness>>
    {
    public:
        ThicknessKeyFrame() = default;

        winrt::IReference<winrt::Thickness> Value() const
        {
            return GetValue(ValueProperty()).try_as<winrt::IReference<winrt::Thickness>>();
        }
        void Value(winrt::IReference<winrt::Thickness> const& value)
        {
            SetValue(ValueProperty(), winrt::box_value(value));
        }

    protected:
        std::optional<Thickness> GetParsedValue() const override
        {
            return Value();
        }

    public:
        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ThicknessKeyFrame : ThicknessKeyFrameT<ThicknessKeyFrame, implementation::ThicknessKeyFrame>
    {
    };
}
