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
        uint8_t AlphaThreshold = 128;

        ContrastBrushConverter() = default;

        IInspectable Convert(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);

        IInspectable ConvertBack(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);

    private:
        bool UseLightContrastColor(Color displayedColor);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct ContrastBrushConverter : ContrastBrushConverterT<ContrastBrushConverter, implementation::ContrastBrushConverter>
    {
    };
}
