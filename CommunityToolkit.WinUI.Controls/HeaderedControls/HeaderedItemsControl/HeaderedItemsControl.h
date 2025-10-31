#pragma once

#include "HeaderedItemsControl.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct HeaderedItemsControl : HeaderedItemsControlT<HeaderedItemsControl>
	{
		HeaderedItemsControl();

		static void OnFooterChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> HeaderProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &HeaderedItemsControl::OnHeaderChanged });

		winrt::Windows::Foundation::IInspectable Header()
		{
			return GetValue(HeaderProperty);
		}

		void Header(winrt::Windows::Foundation::IInspectable const& value)
		{
			SetValue(HeaderProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> HeaderTemplateProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"HeaderTemplate",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::DataTemplate HeaderTemplate()
		{
			return GetValue(HeaderTemplateProperty).as<winrt::Microsoft::UI::Xaml::DataTemplate>();
		}

		void HeaderTemplate(winrt::Microsoft::UI::Xaml::DataTemplate const& value)
		{
			SetValue(HeaderTemplateProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> FooterProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Footer",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &HeaderedItemsControl::OnFooterChanged });

		winrt::Windows::Foundation::IInspectable Footer()
		{
			return GetValue(FooterProperty);
		}

		void Footer(winrt::Windows::Foundation::IInspectable const& value)
		{
			SetValue(FooterProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> FooterTemplateProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"FooterTemplate",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::DataTemplate FooterTemplate()
		{
			return GetValue(FooterTemplateProperty).as<winrt::Microsoft::UI::Xaml::DataTemplate>();
		}

		void FooterTemplate(winrt::Microsoft::UI::Xaml::DataTemplate const& value)
		{
			SetValue(FooterTemplateProperty, value);
		}

		virtual void OnFooterChanged(IInspectable const& oldValue, IInspectable const& newValue);

		virtual void OnHeaderChanged(IInspectable const& oldValue, IInspectable const& newValue);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct HeaderedItemsControl : HeaderedItemsControlT<HeaderedItemsControl, implementation::HeaderedItemsControl>
	{
	};
}
