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
    using namespace XamlToolkit::WinUI;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct AccentColorConverter : AccentColorConverterT<AccentColorConverter>
    {
        AccentColorConverter() = default;

        static constexpr double ValueDelta = 0.1;

        static winrt::HsvColor GetAccent(winrt::HsvColor hsvColor, int accentStep);

        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);

        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct AccentColorConverter : AccentColorConverterT<AccentColorConverter, implementation::AccentColorConverter>
    {
    };
}
