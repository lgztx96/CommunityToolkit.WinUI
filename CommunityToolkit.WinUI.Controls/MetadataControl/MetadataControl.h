#pragma once

#include "MetadataControl.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct MetadataControl : MetadataControlT<MetadataControl>
	{
	private:
		static constexpr std::wstring_view TextContainerPart = L"TextContainer";

		TextBlock _textContainer;

		IObservableVector<IInspectable>::VectorChanged_revoker _vectorChangedRevoker;

	public:
		static void OnMetadataItemsChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SeparatorProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Separator",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L" • "), &MetadataControl::OnPropertyChanged }
			);

		hstring Separator()
		{
			return winrt::unbox_value<hstring>(GetValue(SeparatorProperty));
		}

		void Separator(hstring const& value)
		{
			SetValue(SeparatorProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AccessibleSeparatorProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"AccessibleSeparator",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L", "), &MetadataControl::OnPropertyChanged }
			);

		hstring AccessibleSeparator()
		{
			return winrt::unbox_value<hstring>(GetValue(AccessibleSeparatorProperty));
		}

		void AccessibleSeparator(hstring const& value)
		{
			SetValue(AccessibleSeparatorProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ItemsProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Items",
				winrt::xaml_typename<winrt::Windows::Foundation::Collections::IVector<MetadataItem>>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &MetadataControl::OnMetadataItemsChanged }
			);

		winrt::Windows::Foundation::Collections::IVector<MetadataItem> Items()
		{
			return winrt::unbox_value<winrt::Windows::Foundation::Collections::IVector<MetadataItem>>(GetValue(ItemsProperty));
		}

		void Items(winrt::Windows::Foundation::Collections::IVector<MetadataItem> const& value)
		{
			SetValue(ItemsProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TextBlockStyleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"TextBlockStyle",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr }
			);

		winrt::Microsoft::UI::Xaml::Style TextBlockStyle()
		{
			return winrt::unbox_value<struct Style>(GetValue(TextBlockStyleProperty));
		}

		void TextBlockStyle(winrt::Microsoft::UI::Xaml::Style const& value)
		{
			SetValue(TextBlockStyleProperty, winrt::box_value(value));
		}

		MetadataControl();

		void OnApplyTemplate();

	private:
		void OnActualThemeChanged(FrameworkElement const& sender, IInspectable const& args);

		void Update();

		void NotifyLiveRegionChanged();
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct MetadataControl : MetadataControlT<MetadataControl, implementation::MetadataControl>
	{
	};
}
