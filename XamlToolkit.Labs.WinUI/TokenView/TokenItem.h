#pragma once

#include "TokenItem.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct TokenItem : TokenItemT<TokenItem>
	{
		static constexpr std::wstring_view IconLeftState = L"IconLeft";
		static constexpr std::wstring_view IconOnlyState = L"IconOnly";
		static constexpr std::wstring_view ContentOnlyState = L"ContentOnly";
		static constexpr std::wstring_view RemoveButtonVisibleState = L"RemoveButtonVisible";
		static constexpr std::wstring_view RemoveButtonNotVisibleState = L"RemoveButtonNotVisible";
		static constexpr std::wstring_view TokenItemRemoveButtonName = L"PART_RemoveButton";

		TokenItem();

		void OnApplyTemplate();

		void OnContentChanged(winrt::IInspectable const& oldContent, winrt::IInspectable const& newContent);

		virtual void OnIconPropertyChanged(winrt::IconElement const& oldValue, winrt::IconElement const& newValue);

		virtual void OnIsRemoveablePropertyChanged(bool oldValue, bool newValue);

		wil::untyped_event<winrt::XamlToolkit::Labs::WinUI::TokenItemRemovingEventArgs> Removing;

		static const wil::single_threaded_property<winrt::DependencyProperty> IsRemoveableProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> IconProperty;

		bool IsRemoveable() const
		{
			return winrt::unbox_value<bool>(GetValue(IsRemoveableProperty));
		}

		void IsRemoveable(bool value)
		{
			SetValue(IsRemoveableProperty, winrt::box_value(value));
		}

		winrt::IconElement Icon() const
		{
			return GetValue(IconProperty).try_as<winrt::IconElement>();
		}

		void Icon(winrt::IconElement const& value)
		{
			SetValue(IconProperty, value);
		}

	private:
		void TokenItemRemoveButton_Click(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void ContentChanged();

		void IconChanged();

		void IsRemoveableChanged();

		winrt::ButtonBase _tokenItemRemoveButton{ nullptr };

		winrt::ButtonBase::Click_revoker _removeButtonClickRevoker;
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct TokenItem : TokenItemT<TokenItem, implementation::TokenItem>
	{
	};
}
