#pragma once

#include "TabbedCommandBarItem.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct TabbedCommandBarItem : TabbedCommandBarItemT<TabbedCommandBarItem>
	{
	private:
		ItemsControl _primaryItemsControl{ nullptr };
		Button _moreButton{ nullptr };

	public:
		TabbedCommandBarItem();

		void OnApplyTemplate();

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> HeaderProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L"") }
			);

		IInspectable Header()
		{
			return GetValue(HeaderProperty).try_as<IInspectable>();
		}

		void Header(IInspectable const& value) {
			SetValue(HeaderProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsContextualProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"IsContextual",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(false) }
			);

		bool IsContextual()
		{
			return winrt::unbox_value<bool>(GetValue(IsContextualProperty));
		}

		void IsContextual(bool const& value)
		{
			SetValue(IsContextualProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OverflowButtonAlignmentProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"OverflowButtonAlignment",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::HorizontalAlignment>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Left) }
			);

		winrt::Microsoft::UI::Xaml::HorizontalAlignment OverflowButtonAlignment()
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::HorizontalAlignment>(GetValue(OverflowButtonAlignmentProperty));
		}

		void OverflowButtonAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment const& value)
		{
			SetValue(OverflowButtonAlignmentProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CommandAlignmentProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CommandAlignment",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::HorizontalAlignment>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch) }
			);

		winrt::Microsoft::UI::Xaml::HorizontalAlignment CommandAlignment()
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::HorizontalAlignment>(GetValue(CommandAlignmentProperty));
		}

		void CommandAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment const& value)
		{
			SetValue(CommandAlignmentProperty, winrt::box_value(value));
		}
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct TabbedCommandBarItem : TabbedCommandBarItemT<TabbedCommandBarItem, implementation::TabbedCommandBarItem>
	{
	};
}
