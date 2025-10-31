#pragma once

#include "HeaderedContentControl.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct HeaderedContentControl : HeaderedContentControlT<HeaderedContentControl>
	{
	private:
		static constexpr std::wstring_view PartHeaderPresenter = L"HeaderPresenter";

		static void OnOrientationChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

	public:
		HeaderedContentControl();

		void OnApplyTemplate();

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> HeaderProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Header",
				winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &HeaderedContentControl::OnHeaderChanged });

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
			return GetValue(HeaderTemplateProperty).try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
		}

		void HeaderTemplate(winrt::Microsoft::UI::Xaml::DataTemplate const& value)
		{
			SetValue(HeaderTemplateProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OrientationProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Orientation",
				winrt::xaml_typename<enum Orientation>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(Orientation::Vertical), &HeaderedContentControl::OnOrientationChanged });

		winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation()
		{
			return winrt::unbox_value<enum Orientation>(GetValue(OrientationProperty));
		}

		void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value)
		{
			SetValue(OrientationProperty, winrt::box_value(value));
		}

		virtual void OnHeaderChanged(IInspectable const& oldValue, IInspectable const& newValue);

	private:
		void SetHeaderVisibility();

		void SetOrientation();
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct HeaderedContentControl : HeaderedContentControlT<HeaderedContentControl, implementation::HeaderedContentControl>
	{
	};
}
