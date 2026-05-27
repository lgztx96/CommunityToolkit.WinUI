#include "pch.h"
#include "winrt_module_imports.h"
#include "SegmentedMarginConverter.h"
#if __has_include("SegmentedMarginConverter.g.cpp")
#include "SegmentedMarginConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	IInspectable SegmentedMarginConverter::Convert(winrt::Windows::Foundation::IInspectable const& value,
		[[maybe_unused]] winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
		[[maybe_unused]] winrt::Windows::Foundation::IInspectable const& parameter,
		[[maybe_unused]] winrt::hstring const& language)
	{
		auto segmentedItem = value.as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>();
		auto listView = ItemsControl::ItemsControlFromItemContainer(segmentedItem);

		int32_t index = listView.IndexFromContainer(segmentedItem);

		if (index == 0)
		{
			return winrt::box_value(LeftItemMargin());
		}
		else if (unsigned(index) == listView.Items().Size() - 1)
		{
			return winrt::box_value(RightItemMargin());
		}
		else
		{
			return winrt::box_value(MiddleItemMargin());
		}
	}

	IInspectable SegmentedMarginConverter::ConvertBack(winrt::Windows::Foundation::IInspectable const& value,
		[[maybe_unused]] winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
		[[maybe_unused]] winrt::Windows::Foundation::IInspectable const& parameter,
		[[maybe_unused]] winrt::hstring const& language)
	{
		return value;
	}
}
