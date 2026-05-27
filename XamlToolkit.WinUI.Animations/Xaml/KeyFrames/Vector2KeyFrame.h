#pragma once

#include "Vector2KeyFrame.g.h"
#include "../Abstract/KeyFrame{TValue,TKeyFrame}.h"
#include "../Extensions/AnimationExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct Vector2KeyFrame : Vector2KeyFrameT<Vector2KeyFrame, KeyFrameBase<winrt::hstring, winrt::Windows::Foundation::Numerics::float2>>
    {
    public:
        Vector2KeyFrame() = default;

        winrt::hstring Value() const
        {
            return winrt::unbox_value_or<winrt::hstring>(GetValue(ValueProperty), L"");
        }
        void Value(winrt::hstring const& value)
        {
            SetValue(ValueProperty, winrt::box_value(value));
        }

    protected:
        std::optional<winrt::Windows::Foundation::Numerics::float2> GetParsedValue() const override
        {
            auto value = Value();
            if (value.empty())
            {
                return std::nullopt;
            }

            return Animations::AnimationExtensions::ToVector2(value);
        }

    public:
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ValueProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Vector2KeyFrame : Vector2KeyFrameT<Vector2KeyFrame, implementation::Vector2KeyFrame> {};
}
