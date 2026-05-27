#pragma once

#ifdef __INTELLISENSE__
#include <optional>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
	using namespace Microsoft::UI::Xaml::Markup;
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
