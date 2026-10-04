#include "pch.h"
#include "winrt_module_imports.h"
#include "ItemsCountToCornerRadiusConverter.h"
#if __has_include("ItemsCountToCornerRadiusConverter.g.cpp")
#include "ItemsCountToCornerRadiusConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    winrt::IInspectable ItemsCountToCornerRadiusConverter::Convert(winrt::IInspectable const& value,
        [[maybe_unused]] winrt::TypeName const& targetType,
        [[maybe_unused]] winrt::IInspectable const& parameter,
        [[maybe_unused]] winrt::hstring const& language)
    {
        uint32_t itemsCount{ 0 };

        if (auto countInt = value.try_as<int32_t>())
        {
            itemsCount = static_cast<uint32_t>(*countInt);
        }
        else if (auto countUint = value.try_as<uint32_t>())
        {
            itemsCount = *countUint;
        }

        if (itemsCount > 0)
        {
            return winrt::box_value(winrt::CornerRadius{ 8, 8, 0, 0 });
        }
        return winrt::box_value(winrt::CornerRadius{ 8, 8, 8, 8 });
    }

    winrt::IInspectable ItemsCountToCornerRadiusConverter::ConvertBack(
        [[maybe_unused]] winrt::IInspectable const& value,
        [[maybe_unused]] winrt::TypeName const& targetType,
        [[maybe_unused]] winrt::IInspectable const& parameter,
        [[maybe_unused]] winrt::hstring const& language)
    {
        return value;
    }
}
