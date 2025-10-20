#pragma once

#include "NullToTransparentConverter.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::UI::Xaml::Interop;

    struct NullToTransparentConverter : NullToTransparentConverterT<NullToTransparentConverter>
    {
        NullToTransparentConverter() = default;

        IInspectable Convert(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);

        IInspectable ConvertBack(IInspectable const& value, TypeName targetType, IInspectable const& parameter, winrt::hstring const& language);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct NullToTransparentConverter : NullToTransparentConverterT<NullToTransparentConverter, implementation::NullToTransparentConverter>
    {
    };
}
