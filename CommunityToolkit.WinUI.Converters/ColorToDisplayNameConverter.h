#pragma once

#include "ColorToDisplayNameConverter.g.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.h>

namespace winrt::CommunityToolkit::WinUI::Converters::implementation
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::UI;
	using namespace winrt::Microsoft::UI;
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Media;
	using namespace winrt::Windows::UI::Xaml::Interop;

    struct ColorToDisplayNameConverter : ColorToDisplayNameConverterT<ColorToDisplayNameConverter>
    {
        ColorToDisplayNameConverter() = default;

        IInspectable Convert(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);

        IInspectable ConvertBack(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::CommunityToolkit::WinUI::Converters::factory_implementation
{
    struct ColorToDisplayNameConverter : ColorToDisplayNameConverterT<ColorToDisplayNameConverter, implementation::ColorToDisplayNameConverter>
    {
    };
}
