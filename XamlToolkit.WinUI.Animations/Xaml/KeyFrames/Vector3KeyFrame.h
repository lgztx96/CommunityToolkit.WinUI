#pragma once

#include "Vector3KeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt
{
    using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct Vector3KeyFrame : Vector3KeyFrameT<Vector3KeyFrame, KeyFrameBase<winrt::hstring, winrt::float3>>
    {
    public:
        Vector3KeyFrame() = default;

        winrt::hstring Value() const
        {
            return winrt::unbox_value_or<winrt::hstring>(GetValue(ValueProperty()), L"");
        }
        void Value(winrt::hstring const& value)
        {
            SetValue(ValueProperty(), winrt::box_value(value));
        }

    protected:
        std::optional<winrt::float3> GetParsedValue() const override
        {
            auto value = Value();
            if (value.empty())
            {
                return std::nullopt;
            }

            return AnimationExtensions::ToVector3(value);
        }

    public:
        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Vector3KeyFrame : Vector3KeyFrameT<Vector3KeyFrame, implementation::Vector3KeyFrame> {};
}
