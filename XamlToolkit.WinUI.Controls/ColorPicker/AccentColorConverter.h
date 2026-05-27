#pragma once

#include "AccentColorConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct AccentColorConverter : AccentColorConverterT<AccentColorConverter>
    {
        AccentColorConverter() = default;

        static inline constexpr double ValueDelta = 0.1;

        static XamlToolkit::WinUI::HsvColor GetAccent(XamlToolkit::WinUI::HsvColor hsvColor, int accentStep);

        IInspectable Convert(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);

        IInspectable ConvertBack(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct AccentColorConverter : AccentColorConverterT<AccentColorConverter, implementation::AccentColorConverter>
    {
    };
}
