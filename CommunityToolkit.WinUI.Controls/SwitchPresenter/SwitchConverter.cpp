#include "pch.h"
#include "SwitchConverter.h"
#if __has_include("SwitchConverter.g.cpp")
#include "SwitchConverter.g.cpp"
#endif
#include "SwitchHelpers.h"
#include "CaseCollection.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	SwitchConverter::SwitchConverter()
	{
		SwitchCases(winrt::make<CaseCollection>());
	}

	IInspectable SwitchConverter::Convert(IInspectable const& value, TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language) {

		if (!TargetType())
		{
			TargetType(targetType);
		}

		if (auto result = SwitchHelpers::EvaluateCases(SwitchCases(), value, TargetType()))
			return result.Content();

		return nullptr;
	}

	IInspectable SwitchConverter::ConvertBack([[maybe_unused]] IInspectable const& value, [[maybe_unused]] TypeName targetType, [[maybe_unused]] IInspectable const& parameter, [[maybe_unused]] winrt::hstring const& language)
	{
		throw winrt::hresult_not_implemented();
	}
}
