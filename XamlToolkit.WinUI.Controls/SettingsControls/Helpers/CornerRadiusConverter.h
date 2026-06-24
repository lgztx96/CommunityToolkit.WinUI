#pragma once

#include "CornerRadiusConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::Xaml::Interop;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct CornerRadiusConverter : CornerRadiusConverterT<CornerRadiusConverter>
    {
        CornerRadiusConverter() = default;

        winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName const& targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);

        winrt::IInspectable ConvertBack(winrt::IInspectable const& value, winrt::TypeName const& targetType, winrt::IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct CornerRadiusConverter : CornerRadiusConverterT<CornerRadiusConverter, implementation::CornerRadiusConverter>
    {
    };
}
