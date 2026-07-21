#pragma once

#include "HeaderedItemsControl.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct HeaderedItemsControl : HeaderedItemsControlT<HeaderedItemsControl>
	{
		HeaderedItemsControl();

		static void OnFooterChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnHeaderChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> HeaderProperty =
			winrt::DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &HeaderedItemsControl::OnHeaderChanged });

		winrt::IInspectable Header() const
		{
			return GetValue(HeaderProperty());
		}

		void Header(winrt::IInspectable const& value)
		{
			SetValue(HeaderProperty(), value);
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> HeaderTemplateProperty =
			winrt::DependencyProperty::Register(
				L"HeaderTemplate",
				winrt::xaml_typename<winrt::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		winrt::DataTemplate HeaderTemplate() const
		{
			return GetValue(HeaderTemplateProperty()).try_as<winrt::DataTemplate>();
		}

		void HeaderTemplate(winrt::DataTemplate const& value)
		{
			SetValue(HeaderTemplateProperty(), value);
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> FooterProperty =
			winrt::DependencyProperty::Register(
				L"Footer",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &HeaderedItemsControl::OnFooterChanged });

		winrt::IInspectable Footer() const
		{
			return GetValue(FooterProperty());
		}

		void Footer(winrt::IInspectable const& value)
		{
			SetValue(FooterProperty(), value);
		}

		static inline const wil::single_threaded_property<winrt::DependencyProperty> FooterTemplateProperty =
			winrt::DependencyProperty::Register(
				L"FooterTemplate",
				winrt::xaml_typename<winrt::DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr });

		winrt::DataTemplate FooterTemplate() const
		{
			return GetValue(FooterTemplateProperty()).try_as<winrt::DataTemplate>();
		}

		void FooterTemplate(winrt::DataTemplate const& value)
		{
			SetValue(FooterTemplateProperty(), value);
		}

		virtual void OnFooterChanged(winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);

		virtual void OnHeaderChanged(winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct HeaderedItemsControl : HeaderedItemsControlT<HeaderedItemsControl, implementation::HeaderedItemsControl>
	{
	};
}
