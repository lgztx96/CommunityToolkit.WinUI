#pragma once

#include "SegmentedMarginConverter.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SegmentedMarginConverter : SegmentedMarginConverterT<SegmentedMarginConverter>
	{
		static const wil::single_threaded_property<winrt::DependencyProperty> LeftItemMarginProperty;

		winrt::Thickness LeftItemMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(LeftItemMarginProperty())); }
		void LeftItemMargin(winrt::Thickness const& value) { SetValue(LeftItemMarginProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> MiddleItemMarginProperty;

		winrt::Thickness MiddleItemMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(MiddleItemMarginProperty())); }
		void MiddleItemMargin(winrt::Thickness const& value) { SetValue(MiddleItemMarginProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> RightItemMarginProperty;

		winrt::Thickness RightItemMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(RightItemMarginProperty())); }
		void RightItemMargin(winrt::Thickness const& value) { SetValue(RightItemMarginProperty(), winrt::box_value(value)); }

		winrt::IInspectable Convert(
			winrt::IInspectable const& value, 
			winrt::TypeName const& targetType, 
			winrt::IInspectable const& parameter, 
			winrt::hstring const& language) const;

		winrt::IInspectable ConvertBack(
			winrt::IInspectable const& value,
			winrt::TypeName const& targetType,
			winrt::IInspectable const& parameter,
			winrt::hstring const& language) const;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct SegmentedMarginConverter : SegmentedMarginConverterT<SegmentedMarginConverter, implementation::SegmentedMarginConverter>
	{
	};
}
