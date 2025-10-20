#pragma once

#include "CornerRadiusConverter.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using namespace winrt;
    using namespace Windows::Foundation;
    using namespace Windows::UI::Xaml::Interop;
    using namespace Microsoft::UI::Xaml;

    struct CornerRadiusConverter : CornerRadiusConverterT<CornerRadiusConverter>
    {
        CornerRadiusConverter() = default;

        IInspectable Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language);

        IInspectable ConvertBack(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct CornerRadiusConverter : CornerRadiusConverterT<CornerRadiusConverter, implementation::CornerRadiusConverter>
    {
    };
}
