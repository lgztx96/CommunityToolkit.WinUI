#include "pch.h"
#include "winrt_module_imports.h"
#include "NullToTransparentConverter.h"
#if __has_include("NullToTransparentConverter.g.cpp")
#include "NullToTransparentConverter.g.cpp"
#endif

namespace winrt
{
	using namespace Windows::UI;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    winrt::IInspectable NullToTransparentConverter::Convert(winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        return value;
    }

    winrt::IInspectable NullToTransparentConverter::ConvertBack([[maybe_unused]] winrt::IInspectable const& value, [[maybe_unused]] winrt::TypeName targetType, [[maybe_unused]] winrt::IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
    {
        if (auto colorValue = value.try_as<winrt::Color>()) {
            return value;
        }

        return winrt::box_value(winrt::Windows::UI::Colors::Transparent());
    }
}
