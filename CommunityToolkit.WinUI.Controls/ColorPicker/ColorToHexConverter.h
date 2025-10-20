#pragma once

#include "ColorToHexConverter.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::UI::Xaml::Interop;

    struct ColorToHexConverter : ColorToHexConverterT<ColorToHexConverter>
    {
        ColorToHexConverter() = default;

        IInspectable Convert(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);

        IInspectable ConvertBack(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct ColorToHexConverter : ColorToHexConverterT<ColorToHexConverter, implementation::ColorToHexConverter>
    {
    };
}
