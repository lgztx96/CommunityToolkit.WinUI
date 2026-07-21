#include "pch.h"
#include "winrt_module_imports.h"
#include "SwitchConverter.h"
#if __has_include("SwitchConverter.g.cpp")
#include "SwitchConverter.g.cpp"
#endif
#include "SwitchHelpers.h"
#include "CaseCollection.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	SwitchConverter::SwitchConverter()
	{
		SwitchCases(winrt::make<CaseCollection>());
	}

	winrt::IInspectable SwitchConverter::Convert(
		winrt::IInspectable const& value, 
		[[maybe_unused]] winrt::TypeName const& targetType, 
		[[maybe_unused]] winrt::IInspectable const& parameter, 
		[[maybe_unused]] winrt::hstring const& language) const {

		if (auto result = SwitchHelpers::EvaluateCases(SwitchCases(), value))
		{
			return result.Content();
		}

		return nullptr;
	}

	winrt::IInspectable SwitchConverter::ConvertBack(
		[[maybe_unused]] winrt::IInspectable const& value, 
		[[maybe_unused]] winrt::TypeName const& targetType, 
		[[maybe_unused]] winrt::IInspectable const& parameter, 
		[[maybe_unused]] winrt::hstring const& language)
	{
		throw winrt::hresult_not_implemented();
	}
}
