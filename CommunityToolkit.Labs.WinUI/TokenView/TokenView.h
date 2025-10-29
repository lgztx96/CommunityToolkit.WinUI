#pragma once

#include "TokenView.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include "TokenItem.h"

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	using namespace winrt::Microsoft::UI::Xaml::Input;
	using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;

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
		int _internalSelectedIndex = -1;

		ScrollViewer _tokenViewScroller{ nullptr };
		ButtonBase _tokenViewScrollBackButton{ nullptr };
		ButtonBase _tokenViewScrollForwardButton{ nullptr };

		UIElement::PreviewKeyDown_revoker _previewKeyDownRevoker;
		Control::SizeChanged_revoker _sizeChangedRevoker;
		FrameworkElement::Loaded_revoker _tokenViewScrollerLoadedRevoker;
		ButtonBase::Click_revoker _scrollBackButtonClickRevoker;
		ButtonBase::Click_revoker _scrollForwardButtonClickRevoker;
		ScrollViewer::ViewChanging_revoker _scrollViewChangingClickRevoker;

	public:
		TokenView();

		wil::untyped_event<TokenItemRemovingEventArgs> TokenItemRemoving;

		DependencyObject GetContainerForItemOverride() { return winrt::make<TokenItem>(); }

		bool IsItemItsOwnContainerOverride(IInspectable const& item);

		void OnApplyTemplate();

		void PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item);

		virtual void OnIsWrappedPropertyChanged(bool oldValue, bool newValue);

		virtual void OnCanRemoveTokensPropertyChanged(bool oldValue, bool newValue);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsWrappedProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"IsWrapped",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{
					winrt::box_value(false),
					[](auto& d, auto& e)
					{
						if (auto tokenView = d.template try_as<class_type>())
						{
							auto self = winrt::get_self<TokenView>(tokenView)->get_strong();
							auto oldValue = winrt::unbox_value<bool>(e.OldValue());
							auto newValue = winrt::unbox_value<bool>(e.NewValue());
							self->OnIsWrappedPropertyChanged(oldValue, newValue);
						}
					} });

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CanRemoveTokensProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CanRemoveTokens",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{
					winrt::box_value(false),
					 [](auto& d, auto& e)
					{
						if (auto tokenView = d.template try_as<class_type>())
						{
							auto self = winrt::get_self<TokenView>(tokenView)->get_strong();
							auto oldValue = winrt::unbox_value<bool>(e.OldValue());
							auto newValue = winrt::unbox_value<bool>(e.NewValue());
							self->OnCanRemoveTokensPropertyChanged(oldValue, newValue);
						}
					}
				}
			);

		bool IsWrapped() const
		{
			return winrt::unbox_value<bool>(GetValue(IsWrappedProperty));
		}

		void IsWrapped(bool value)
		{
			SetValue(IsWrappedProperty, winrt::box_value(value));
		}

		bool CanRemoveTokens() const
		{
			return winrt::unbox_value<bool>(GetValue(CanRemoveTokensProperty));
		}

		void CanRemoveTokens(bool value)
		{
			SetValue(CanRemoveTokensProperty, winrt::box_value(value));
		}

	private:
		bool RemoveItem();

		void UpdateScrollButtonsVisibility();

		winrt::CommunityToolkit::Labs::WinUI::TokenItem GetCurrentContainerItem();

		void SelectedIndex_PropertyChanged(DependencyObject const& sender, DependencyProperty const& dp);

		void OnCanRemoveTokensChanged() {}

#pragma region Events
		void TokenView_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		void ScrollTabBackButton_Click(IInspectable const& sender, RoutedEventArgs const& e);

		void ScrollTabForwardButton_Click(IInspectable const& sender, RoutedEventArgs const& e);

		void TokenViewScroller_ViewChanging(IInspectable const& sender, ScrollViewerViewChangingEventArgs const& e);

		void ScrollViewer_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		void Token_Removing(IInspectable const& sender, winrt::CommunityToolkit::Labs::WinUI::TokenItemRemovingEventArgs const& e);

		// void Token_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		void OnIsWrappedChanged();

		void TokenView_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);
#pragma endregion

#pragma region ItemsSource
		//void OnItemsChanged(IInspectable const& e);

		//void ItemsSource_PropertyChanged(DependencyObject const& sender, DependencyProperty const& dp);
#pragma endregion

#pragma region Selection
		bool MoveFocus(MoveDirection direction);
#pragma endregion
	};
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
	struct TokenView : TokenViewT<TokenView, implementation::TokenView>
	{
	};
}
