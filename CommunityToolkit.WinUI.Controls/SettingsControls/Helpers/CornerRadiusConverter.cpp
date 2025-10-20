#include "pch.h"
#include "CornerRadiusConverter.h"
#if __has_include("CornerRadiusConverter.g.cpp")
#include "CornerRadiusConverter.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	IInspectable CornerRadiusConverter::Convert(IInspectable const& value,
		[[maybe_unused]] TypeName const& targetType,
		[[maybe_unused]] IInspectable const& parameter,
		[[maybe_unused]] hstring const& language)
	{
		if (auto cornerRadius = value.try_as<CornerRadius>(); cornerRadius)
		{
			return winrt::box_value(CornerRadius({ 0.0, 0.0, cornerRadius->BottomRight, cornerRadius->BottomLeft }));
		}
		else
		{
			return value;
		}
	}

	IInspectable CornerRadiusConverter::ConvertBack(
		[[maybe_unused]] IInspectable const& value,
		[[maybe_unused]] TypeName const& targetType,
		[[maybe_unused]] IInspectable const& parameter,
		[[maybe_unused]] hstring const& language)
	{
		return value;
	}
}
