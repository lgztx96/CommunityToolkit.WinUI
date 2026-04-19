#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
	class ConverterTools
	{
	public:
		static bool TryParseBool(winrt::IInspectable const& parameter);

		static winrt::IInspectable Convert(winrt::IInspectable const& value, winrt::TypeName const& targetType);

		static std::optional<winrt::hstring> TryConvertToString(winrt::IInspectable const& value);

		static bool ValueEquals(winrt::IPropertyValue const& valueA, winrt::IPropertyValue const& valueB);
	};
}
