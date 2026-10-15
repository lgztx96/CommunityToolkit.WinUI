#pragma once

#include "ItemsCountToCornerRadiusConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    struct ItemsCountToCornerRadiusConverter : ItemsCountToCornerRadiusConverterT<ItemsCountToCornerRadiusConverter>
    {
        ItemsCountToCornerRadiusConverter() = default;

        [[nodiscard]] winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName const& targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);

        [[nodiscard]] winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName const& targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::XamlToolkit::WinUI::Converters::factory_implementation
{
    struct ItemsCountToCornerRadiusConverter : ItemsCountToCornerRadiusConverterT<ItemsCountToCornerRadiusConverter, implementation::ItemsCountToCornerRadiusConverter>
    {
    };
}
