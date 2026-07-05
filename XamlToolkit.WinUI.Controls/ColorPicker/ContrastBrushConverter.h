#pragma once

#include "ContrastBrushConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#endif

namespace winrt
{
	using namespace Windows::UI;
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct ContrastBrushConverter : ContrastBrushConverterT<ContrastBrushConverter>
    {
        static constexpr uint8_t AlphaThreshold = 128;

        ContrastBrushConverter() = default;

        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);

        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);

    private:
        bool UseLightContrastColor(winrt::Color displayedColor) const;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct ContrastBrushConverter : ContrastBrushConverterT<ContrastBrushConverter, implementation::ContrastBrushConverter>
    {
    };
}
