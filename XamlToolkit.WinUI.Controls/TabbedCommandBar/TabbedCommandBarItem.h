#pragma once

#include "TabbedCommandBarItem.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct TabbedCommandBarItem : TabbedCommandBarItemT<TabbedCommandBarItem>
	{
		static constexpr auto PrimaryItemsControlPartName = L"PrimaryItemsControl";
		static constexpr auto MoreButtonPartName = L"MoreButton";

		TabbedCommandBarItem();

		void OnApplyTemplate();

		static inline const wil::single_threaded_property<winrt::DependencyProperty> HeaderProperty =
			winrt::DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(L"") }
			);

		winrt::IInspectable Header() const
		{
			return GetValue(HeaderProperty());
		}

		void Header(winrt::IInspectable const& value) 
		{
			SetValue(HeaderProperty(), value);
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> IsContextualProperty =
			winrt::DependencyProperty::Register(
				L"IsContextual",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(false) });

		bool IsContextual() const
		{
			return winrt::unbox_value<bool>(GetValue(IsContextualProperty()));
		}

		void IsContextual(bool const& value)
		{
			SetValue(IsContextualProperty(), winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> OverflowButtonAlignmentProperty =
			winrt::DependencyProperty::Register(
				L"OverflowButtonAlignment",
				winrt::xaml_typename<winrt::HorizontalAlignment>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(winrt::HorizontalAlignment::Left) });

		winrt::HorizontalAlignment OverflowButtonAlignment() const
		{
			return winrt::unbox_value<winrt::HorizontalAlignment>(GetValue(OverflowButtonAlignmentProperty()));
		}

		void OverflowButtonAlignment(winrt::HorizontalAlignment const& value)
		{
			SetValue(OverflowButtonAlignmentProperty(), winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> CommandAlignmentProperty =
			winrt::DependencyProperty::Register(
				L"CommandAlignment",
				winrt::xaml_typename<winrt::HorizontalAlignment>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(winrt::HorizontalAlignment::Stretch) });

		winrt::HorizontalAlignment CommandAlignment() const
		{
			return winrt::unbox_value<winrt::HorizontalAlignment>(GetValue(CommandAlignmentProperty()));
		}

		void CommandAlignment(winrt::HorizontalAlignment const& value)
		{
			SetValue(CommandAlignmentProperty(), winrt::box_value(value));
		}

	private:
		winrt::ItemsControl _primaryItemsControl{ nullptr };
		winrt::Button _moreButton{ nullptr };

		int64_t _commandAlignmentChangedToken{ 0 };
		int64_t _overflowAlignmentChangedToken{ 0 };
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct TabbedCommandBarItem : TabbedCommandBarItemT<TabbedCommandBarItem, implementation::TabbedCommandBarItem>
	{
	};
}
