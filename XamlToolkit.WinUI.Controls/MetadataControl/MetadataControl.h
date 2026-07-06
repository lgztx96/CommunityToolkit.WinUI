#pragma once

#include "MetadataControl.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct MetadataControl : MetadataControlT<MetadataControl>
	{
		static constexpr std::wstring_view TextContainerPart = L"TextContainer";

		MetadataControl();

		void OnApplyTemplate();

		static const wil::single_threaded_property<winrt::DependencyProperty> SeparatorProperty;

		winrt::hstring Separator() const
		{
			return winrt::unbox_value<winrt::hstring>(GetValue(SeparatorProperty()));
		}

		void Separator(hstring const& value)
		{
			SetValue(SeparatorProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> AccessibleSeparatorProperty;

		winrt::hstring AccessibleSeparator() const
		{
			return winrt::unbox_value<winrt::hstring>(GetValue(AccessibleSeparatorProperty()));
		}

		void AccessibleSeparator(winrt::hstring const& value)
		{
			SetValue(AccessibleSeparatorProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> ItemsProperty;

		winrt::IVector<MetadataItem> Items()
		{
			return winrt::unbox_value<winrt::IVector<MetadataItem>>(GetValue(ItemsProperty()));
		}

		void Items(winrt::IVector<MetadataItem> const& value)
		{
			SetValue(ItemsProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> TextBlockStyleProperty;

		winrt::Style TextBlockStyle() const
		{
			return winrt::unbox_value<winrt::Style>(GetValue(TextBlockStyleProperty()));
		}

		void TextBlockStyle(winrt::Style const& value)
		{
			SetValue(TextBlockStyleProperty(), winrt::box_value(value));
		}

	private:
		static void OnMetadataItemsChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		void OnActualThemeChanged(FrameworkElement const& sender, IInspectable const& args);

		void Update();

		void NotifyLiveRegionChanged();

		winrt::TextBlock _textContainer;

		winrt::IObservableVector<winrt::IInspectable>::VectorChanged_revoker _vectorChangedRevoker;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct MetadataControl : MetadataControlT<MetadataControl, implementation::MetadataControl>
	{
	};
}
