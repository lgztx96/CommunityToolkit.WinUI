#pragma once

#include "HeaderedContentControl.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct HeaderedContentControl : HeaderedContentControlT<HeaderedContentControl>
	{
	private:
		static constexpr std::wstring_view PartHeaderPresenter = L"HeaderPresenter";

		static void OnOrientationChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnHeaderChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

	public:
		HeaderedContentControl();

		void OnApplyTemplate();

		static inline const wil::single_threaded_property<winrt::DependencyProperty> HeaderProperty =
			winrt::DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ nullptr, &HeaderedContentControl::OnHeaderChanged });

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

		static inline const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty =
			winrt::DependencyProperty::Register(
				L"Orientation",
				winrt::xaml_typename<winrt::Orientation>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(winrt::Orientation::Vertical), &HeaderedContentControl::OnOrientationChanged });

		winrt::Orientation Orientation() const
		{
			return winrt::unbox_value<winrt::Orientation>(GetValue(OrientationProperty()));
		}

		void Orientation(winrt::Orientation const& value)
		{
			SetValue(OrientationProperty(), winrt::box_value(value));
		}

		virtual void OnHeaderChanged(winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);

	private:
		void SetHeaderVisibility();

		void SetOrientation();
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct HeaderedContentControl : HeaderedContentControlT<HeaderedContentControl, implementation::HeaderedContentControl>
	{
	};
}
