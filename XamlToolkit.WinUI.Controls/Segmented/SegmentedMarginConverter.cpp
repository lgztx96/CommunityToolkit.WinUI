#include "pch.h"
#include "winrt_module_imports.h"
#include "SegmentedMarginConverter.h"
#if __has_include("SegmentedMarginConverter.g.cpp")
#include "SegmentedMarginConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> SegmentedMarginConverter::LeftItemMarginProperty =
		winrt::DependencyProperty::Register(
			L"LeftItemMargin",
			winrt::xaml_typename<winrt::Thickness>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> SegmentedMarginConverter::MiddleItemMarginProperty =
		winrt::DependencyProperty::Register(
			L"MiddleItemMargin",
			winrt::xaml_typename<winrt::Thickness>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> SegmentedMarginConverter::RightItemMarginProperty =
		winrt::DependencyProperty::Register(
			L"RightItemMargin",
			winrt::xaml_typename<winrt::Thickness>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	winrt::IInspectable SegmentedMarginConverter::Convert(
		winrt::IInspectable const& value,
		[[maybe_unused]] winrt::TypeName const& targetType,
		[[maybe_unused]] winrt::IInspectable const& parameter,
		[[maybe_unused]] winrt::hstring const& language) const
	{
		auto segmentedItem = value.as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>();

		if (auto listView = winrt::ItemsControl::ItemsControlFromItemContainer(segmentedItem))
		{
			int32_t index = listView.IndexFromContainer(segmentedItem);

			if (index == 0)
			{
				return winrt::box_value(LeftItemMargin());
			}
			else if (index == static_cast<int>(listView.Items().Size()) - 1)
			{
				return winrt::box_value(RightItemMargin());
			}
		}

		return winrt::box_value(MiddleItemMargin());
	}

	winrt::IInspectable SegmentedMarginConverter::ConvertBack(
		winrt::IInspectable const& value,
		[[maybe_unused]] winrt::TypeName const& targetType,
		[[maybe_unused]] winrt::IInspectable const& parameter,
		[[maybe_unused]] winrt::hstring const& language) const
	{
		return value;
	}
}
