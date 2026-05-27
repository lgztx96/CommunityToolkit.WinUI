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

	IInspectable SwitchConverter::Convert(IInspectable const& value, [[maybe_unused]] TypeName const& targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) {

		if (auto result = SwitchHelpers::EvaluateCases(SwitchCases(), value))
			return result.Content();

		return nullptr;
	}

	IInspectable SwitchConverter::ConvertBack([[maybe_unused]] IInspectable const& value, [[maybe_unused]] TypeName const& targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
	{
		throw winrt::hresult_not_implemented();
	}
}
