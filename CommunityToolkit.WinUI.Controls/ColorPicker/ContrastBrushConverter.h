#pragma once

#include "ContrastBrushConverter.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::UI::Xaml::Interop;

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

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct ContrastBrushConverter : ContrastBrushConverterT<ContrastBrushConverter, implementation::ContrastBrushConverter>
    {
    };
}
