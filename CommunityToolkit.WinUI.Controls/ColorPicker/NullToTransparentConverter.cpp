#include "pch.h"
#include "NullToTransparentConverter.h"
#if __has_include("NullToTransparentConverter.g.cpp")
#include "NullToTransparentConverter.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    IInspectable NullToTransparentConverter::Convert(IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        return value;
    }

    IInspectable NullToTransparentConverter::ConvertBack([[maybe_unused]] IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        if (auto colorValue = value.try_as<Color>()) {
            return value;
        }

        return winrt::box_value(winrt::Microsoft::UI::Colors::Transparent());
    }
}
