#pragma once

#include "TokenView.g.h"
#include "TokenItem.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
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
	using namespace Microsoft::UI::Xaml::Data;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	enum MoveDirection
	{
		Next,
		Previous
	};

	struct TokenView : TokenViewT<TokenView>
	{
		static constexpr std::wstring_view TokenViewScrollViewerName = L"ScrollViewer";
		static constexpr std::wstring_view TokenViewScrollBackButtonName = L"ScrollBackButton";
		static constexpr std::wstring_view TokenViewScrollForwardButtonName = L"ScrollForwardButton";
		
		TokenView();

		wil::untyped_event<winrt::XamlToolkit::Labs::WinUI::TokenItemRemovingEventArgs> TokenItemRemoving;

		winrt::DependencyObject GetContainerForItemOverride() { return winrt::make<TokenItem>(); }

		bool IsItemItsOwnContainerOverride(winrt::IInspectable const& item);

		void OnApplyTemplate();

		void PrepareContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item);

		virtual void OnIsWrappedPropertyChanged(bool oldValue, bool newValue);

		virtual void OnCanRemoveTokensPropertyChanged(bool oldValue, bool newValue);

		static const wil::single_threaded_property<winrt::DependencyProperty> IsWrappedProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> CanRemoveTokensProperty;

		bool IsWrapped() const
		{
			return winrt::unbox_value<bool>(GetValue(IsWrappedProperty()));
		}

		void IsWrapped(bool value)
		{
			SetValue(IsWrappedProperty(), winrt::box_value(value));
		}

		bool CanRemoveTokens() const
		{
			return winrt::unbox_value<bool>(GetValue(CanRemoveTokensProperty()));
		}

		void CanRemoveTokens(bool value)
		{
			SetValue(CanRemoveTokensProperty(), winrt::box_value(value));
		}

	private:
		bool RemoveItem();

		void UpdateScrollButtonsVisibility();

		winrt::XamlToolkit::Labs::WinUI::TokenItem GetCurrentContainerItem();

		void SelectedIndex_PropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		void OnCanRemoveTokensChanged() {}

#pragma region Events
		void TokenView_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		void ScrollTabBackButton_Click(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void ScrollTabForwardButton_Click(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void TokenViewScroller_ViewChanging(winrt::IInspectable const& sender, winrt::ScrollViewerViewChangingEventArgs const& e);

		void ScrollViewer_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void Token_Removing(winrt::IInspectable const& sender, winrt::XamlToolkit::Labs::WinUI::TokenItemRemovingEventArgs const& e);

		// void Token_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void OnIsWrappedChanged();

		void TokenView_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);
#pragma endregion

#pragma region ItemsSource
		//void OnItemsChanged(winrt::IInspectable const& e);

		//void ItemsSource_PropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);
#pragma endregion

#pragma region Selection
		bool MoveFocus(MoveDirection direction);
#pragma endregion

	private:
		int _internalSelectedIndex = -1;

		winrt::ScrollViewer _tokenViewScroller{ nullptr };
		winrt::ButtonBase _tokenViewScrollBackButton{ nullptr };
		winrt::ButtonBase _tokenViewScrollForwardButton{ nullptr };

		winrt::UIElement::PreviewKeyDown_revoker _previewKeyDownRevoker;
		winrt::Control::SizeChanged_revoker _sizeChangedRevoker;
		winrt::FrameworkElement::Loaded_revoker _tokenViewScrollerLoadedRevoker;
		winrt::ButtonBase::Click_revoker _scrollBackButtonClickRevoker;
		winrt::ButtonBase::Click_revoker _scrollForwardButtonClickRevoker;
		winrt::ScrollViewer::ViewChanging_revoker _scrollViewChangingClickRevoker;
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct TokenView : TokenViewT<TokenView, implementation::TokenView>
	{
	};
}
