#include "pch.h"
#include "winrt_module_imports.h"
#include "CornerRadiusConverter.h"
#if __has_include("CornerRadiusConverter.g.cpp")
#include "CornerRadiusConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    winrt::IInspectable CornerRadiusConverter::Convert(winrt::IInspectable const& value,
        [[maybe_unused]] winrt::TypeName const& targetType,
        [[maybe_unused]] winrt::IInspectable const& parameter,
        [[maybe_unused]] winrt::hstring const& language)
    {
        if (auto cornerRadius = value.try_as<winrt::CornerRadius>())
        {
            return winrt::box_value(winrt::CornerRadius({ 0.0, 0.0, cornerRadius->BottomRight, cornerRadius->BottomLeft }));
        }
        else
        {
            return value;
        }
    }

    winrt::IInspectable CornerRadiusConverter::ConvertBack(
        [[maybe_unused]] winrt::IInspectable const& value,
        [[maybe_unused]] winrt::TypeName const& targetType,
        [[maybe_unused]] winrt::IInspectable const& parameter,
        [[maybe_unused]] winrt::hstring const& language)
    {
        return value;
    }
}
