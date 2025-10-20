#pragma once

#include "SegmentedMarginConverter.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct SegmentedMarginConverter : SegmentedMarginConverterT<SegmentedMarginConverter>
	{
		static inline const wil::single_threaded_property<DependencyProperty> LeftItemMarginProperty =
			DependencyProperty::Register(L"LeftItemMargin", winrt::xaml_typename<Thickness>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		Thickness LeftItemMargin() { return winrt::unbox_value<Thickness>(GetValue(LeftItemMarginProperty)); }
		void LeftItemMargin(Thickness value) { SetValue(LeftItemMarginProperty, winrt::box_value(value)); }


		static inline const wil::single_threaded_property<DependencyProperty> MiddleItemMarginProperty =
			DependencyProperty::Register(L"MiddleItemMargin", winrt::xaml_typename<Thickness>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		Thickness MiddleItemMargin() { return winrt::unbox_value<Thickness>(GetValue(MiddleItemMarginProperty)); }
		void MiddleItemMargin(Thickness value) { SetValue(MiddleItemMarginProperty, winrt::box_value(value)); }


		static inline const wil::single_threaded_property<DependencyProperty> RightItemMarginProperty =
			DependencyProperty::Register(L"RightItemMargin", winrt::xaml_typename<Thickness>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		Thickness RightItemMargin() { return winrt::unbox_value<Thickness>(GetValue(RightItemMarginProperty)); }
		void RightItemMargin(Thickness value) { SetValue(RightItemMarginProperty, winrt::box_value(value)); }

		IInspectable Convert(
			winrt::Windows::Foundation::IInspectable const& value,
			winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
			winrt::Windows::Foundation::IInspectable const& parameter,
			winrt::hstring const& language);

		IInspectable ConvertBack(
			winrt::Windows::Foundation::IInspectable const& value,
			winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
			winrt::Windows::Foundation::IInspectable const& parameter,
			winrt::hstring const& language);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct SegmentedMarginConverter : SegmentedMarginConverterT<SegmentedMarginConverter, implementation::SegmentedMarginConverter>
	{
	};
}
