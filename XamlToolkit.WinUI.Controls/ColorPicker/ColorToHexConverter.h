#pragma once

#include "ColorToHexConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct ColorToHexConverter : ColorToHexConverterT<ColorToHexConverter>
    {
        ColorToHexConverter() = default;

        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);

        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct ColorToHexConverter : ColorToHexConverterT<ColorToHexConverter, implementation::ColorToHexConverter>
    {
    };
}
