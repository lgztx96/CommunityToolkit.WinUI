#include "pch.h"
#include "../CommunityToolkit.WinUI/common.h"
#include "TokenItemRemovingEventArgs.h"
#include "TokenView.h"
#include <winrt/Microsoft.UI.Xaml.Input.h>

using namespace winrt;
using namespace CommunityToolkit::WinUI;
using namespace Microsoft::UI::Xaml;
using namespace Windows::System;

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	void TokenView::TokenView_SizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		UpdateScrollButtonsVisibility();
	}

	void TokenView::ScrollTabBackButton_Click([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (_tokenViewScroller != nullptr)
		{
			_tokenViewScroller.ChangeView(_tokenViewScroller.HorizontalOffset() - _tokenViewScroller.ViewportWidth(), nullptr, nullptr);
		}
	}

	void TokenView::ScrollTabForwardButton_Click([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (_tokenViewScroller != nullptr)
		{
			_tokenViewScroller.ChangeView(_tokenViewScroller.HorizontalOffset() + _tokenViewScroller.ViewportWidth(), nullptr, nullptr);
		}
	}

	void TokenView::TokenViewScroller_ViewChanging([[maybe_unused]] IInspectable const& sender, ScrollViewerViewChangingEventArgs const& e)
	{
		if (_tokenViewScrollBackButton != nullptr)
		{
			if (e.FinalView().HorizontalOffset() < 1)
			{
				_tokenViewScrollBackButton.Visibility(Visibility::Collapsed);
			}
			else if (e.FinalView().HorizontalOffset() > 1)
			{
				_tokenViewScrollBackButton.Visibility(Visibility::Visible);
			}
		}

		if (_tokenViewScrollForwardButton != nullptr)
		{
			if (_tokenViewScroller != nullptr)
			{
				if (e.FinalView().HorizontalOffset() > _tokenViewScroller.ScrollableWidth() - 1)
				{
					_tokenViewScrollForwardButton.Visibility(Visibility::Collapsed);
				}
				else if (e.FinalView().HorizontalOffset() < _tokenViewScroller.ScrollableWidth() - 1)
				{
					_tokenViewScrollForwardButton.Visibility(Visibility::Visible);
				}
			}
		}
	}

	void TokenView::ScrollViewer_Loaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (_tokenViewScroller != nullptr)
		{
			_tokenViewScrollerLoadedRevoker.revoke();
			_scrollViewChangingClickRevoker.revoke();
		}

		if (_tokenViewScrollBackButton != nullptr)
		{
			_scrollBackButtonClickRevoker.revoke();
		}

		if (_tokenViewScrollForwardButton != nullptr)
		{
			_scrollForwardButtonClickRevoker.revoke();
		}

		if (_tokenViewScroller != nullptr)
		{
			_scrollViewChangingClickRevoker = _tokenViewScroller.ViewChanging(winrt::auto_revoke, { get_weak(), &TokenView::TokenViewScroller_ViewChanging });
			_tokenViewScrollBackButton = DependencyObjectEx::FindDescendant(_tokenViewScroller, TokenViewScrollBackButtonName).try_as<ButtonBase>();
			_tokenViewScrollForwardButton = DependencyObjectEx::FindDescendant(_tokenViewScroller, TokenViewScrollForwardButtonName).try_as<ButtonBase>();
		}

		if (_tokenViewScrollBackButton != nullptr)
		{
			_scrollBackButtonClickRevoker = _tokenViewScrollBackButton.Click(winrt::auto_revoke, { get_weak(), &TokenView::ScrollTabBackButton_Click });
		}

		if (_tokenViewScrollForwardButton != nullptr)
		{
			_scrollForwardButtonClickRevoker = _tokenViewScrollForwardButton.Click(winrt::auto_revoke, { get_weak(), &TokenView::ScrollTabForwardButton_Click });
		}

		UpdateScrollButtonsVisibility();
	}

	void TokenView::Token_Removing([[maybe_unused]] IInspectable const& sender, winrt::CommunityToolkit::Labs::WinUI::TokenItemRemovingEventArgs const& e)
	{
		if (auto item = ItemFromContainer(e.TokenItem()))
		{
			auto args = winrt::make<TokenItemRemovingEventArgs>(item, e.TokenItem());
			TokenItemRemoving.invoke(*this, args);

			if (auto itemsSource = ItemsSource().try_as<winrt::Windows::Foundation::Collections::IVector<IInspectable>>())
			{
				uint32_t index;
				if (itemsSource.IndexOf(item, index))
				{
					itemsSource.RemoveAt(index);
				}
			}
			else
			{
				if (_tokenViewScroller != nullptr)
				{
					_tokenViewScroller.UpdateLayout();
				}

				if (auto items = Items())
				{
					uint32_t index;
					if (items.IndexOf(item, index))
					{
						items.RemoveAt(index);
					}
				}
			}
		}

		UpdateScrollButtonsVisibility();
	}

	//void TokenView::Token_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	//{
	//    if (auto token = sender.try_as<winrt::CommunityToolkit::Labs::WinUI::TokenItem>())
	//    {
	//        token.Loaded -= Token_Loaded;
	//    }
	//}

	void TokenView::OnIsWrappedChanged()
	{
		if (_tokenViewScroller != nullptr)
		{
			if (IsWrapped())
			{
				_tokenViewScroller.HorizontalScrollBarVisibility(ScrollBarVisibility::Disabled);
			}
			else
			{
				_tokenViewScroller.HorizontalScrollBarVisibility(ScrollBarVisibility::Hidden);
			}
		}
	}

	void TokenView::TokenView_PreviewKeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		switch (e.Key())
		{
		case VirtualKey::Left: e.Handled(MoveFocus(MoveDirection::Previous)); break;
		case VirtualKey::Right: e.Handled(MoveFocus(MoveDirection::Next)); break;
		case VirtualKey::Back:
		case VirtualKey::Delete: e.Handled(RemoveItem()); break;
		}
	}
}