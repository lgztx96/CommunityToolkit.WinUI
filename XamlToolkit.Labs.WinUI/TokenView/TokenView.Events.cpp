#include "pch.h"
#include "winrt_module_imports.h"
#include "../XamlToolkit.WinUI/common.h"
#include "TokenItemRemovingEventArgs.h"
#include "TokenView.h"

namespace winrt
{
    using namespace XamlToolkit::WinUI;
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::System;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    void TokenView::TokenView_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
    {
        UpdateScrollButtonsVisibility();
    }

    void TokenView::ScrollTabBackButton_Click([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (_tokenViewScroller)
        {
            _tokenViewScroller.ChangeView(_tokenViewScroller.HorizontalOffset() - _tokenViewScroller.ViewportWidth(), nullptr, nullptr);
        }
    }

    void TokenView::ScrollTabForwardButton_Click([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (_tokenViewScroller)
        {
            _tokenViewScroller.ChangeView(_tokenViewScroller.HorizontalOffset() + _tokenViewScroller.ViewportWidth(), nullptr, nullptr);
        }
    }

    void TokenView::TokenViewScroller_ViewChanging([[maybe_unused]] winrt::IInspectable const& sender, winrt::ScrollViewerViewChangingEventArgs const& e)
    {
        if (_tokenViewScrollBackButton)
        {
            if (e.FinalView().HorizontalOffset() < 1)
            {
                _tokenViewScrollBackButton.Visibility(winrt::Visibility::Collapsed);
            }
            else if (e.FinalView().HorizontalOffset() > 1)
            {
                _tokenViewScrollBackButton.Visibility(winrt::Visibility::Visible);
            }
        }

        if (_tokenViewScrollForwardButton)
        {
            if (_tokenViewScroller)
            {
                if (e.FinalView().HorizontalOffset() > _tokenViewScroller.ScrollableWidth() - 1)
                {
                    _tokenViewScrollForwardButton.Visibility(winrt::Visibility::Collapsed);
                }
                else if (e.FinalView().HorizontalOffset() < _tokenViewScroller.ScrollableWidth() - 1)
                {
                    _tokenViewScrollForwardButton.Visibility(winrt::Visibility::Visible);
                }
            }
        }
    }

    void TokenView::ScrollViewer_Loaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (_tokenViewScroller)
        {
            _tokenViewScrollerLoadedRevoker.revoke();
            _scrollViewChangingClickRevoker.revoke();
        }

        if (_tokenViewScrollBackButton)
        {
            _scrollBackButtonClickRevoker.revoke();
        }

        if (_tokenViewScrollForwardButton)
        {
            _scrollForwardButtonClickRevoker.revoke();
        }

        if (_tokenViewScroller)
        {
            _scrollViewChangingClickRevoker = _tokenViewScroller.ViewChanging(winrt::auto_revoke, { get_weak(), &TokenView::TokenViewScroller_ViewChanging });
            _tokenViewScrollBackButton = winrt::DependencyObjectEx::FindDescendant(_tokenViewScroller, TokenViewScrollBackButtonName).try_as<winrt::ButtonBase>();
            _tokenViewScrollForwardButton = winrt::DependencyObjectEx::FindDescendant(_tokenViewScroller, TokenViewScrollForwardButtonName).try_as<winrt::ButtonBase>();
        }

        if (_tokenViewScrollBackButton)
        {
            _scrollBackButtonClickRevoker = _tokenViewScrollBackButton.Click(winrt::auto_revoke, { get_weak(), &TokenView::ScrollTabBackButton_Click });
        }

        if (_tokenViewScrollForwardButton)
        {
            _scrollForwardButtonClickRevoker = _tokenViewScrollForwardButton.Click(winrt::auto_revoke, { get_weak(), &TokenView::ScrollTabForwardButton_Click });
        }

        UpdateScrollButtonsVisibility();
    }

    void TokenView::Token_Removing([[maybe_unused]] winrt::IInspectable const& sender, winrt::XamlToolkit::Labs::WinUI::TokenItemRemovingEventArgs const& e)
    {
        if (auto item = ItemFromContainer(e.TokenItem()))
        {
            auto args = winrt::make_self<TokenItemRemovingEventArgs>(item, e.TokenItem());
            TokenItemRemoving.invoke(*this, *args);

            if (auto itemsSource = ItemsSource().try_as<winrt::IVector<winrt::IInspectable>>())
            {
                uint32_t index;
                if (itemsSource.IndexOf(item, index))
                {
                    itemsSource.RemoveAt(index);
                }
            }
            else
            {
                if (_tokenViewScroller)
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

    //void TokenView::Token_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e)
    //{
    //    if (auto token = sender.try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>())
    //    {
    //        token.Loaded -= Token_Loaded;
    //    }
    //}

    void TokenView::OnIsWrappedChanged()
    {
        if (_tokenViewScroller)
        {
            if (IsWrapped())
            {
                _tokenViewScroller.HorizontalScrollBarVisibility(winrt::ScrollBarVisibility::Disabled);
            }
            else
            {
                _tokenViewScroller.HorizontalScrollBarVisibility(winrt::ScrollBarVisibility::Hidden);
            }
        }
    }

    void TokenView::TokenView_PreviewKeyDown([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
    {
        switch (e.Key())
        {
            case winrt::VirtualKey::Left: e.Handled(MoveFocus(MoveDirection::Previous)); break;
            case winrt::VirtualKey::Right: e.Handled(MoveFocus(MoveDirection::Next)); break;
            case winrt::VirtualKey::Back:
            case winrt::VirtualKey::Delete: e.Handled(RemoveItem()); break;
        }
    }
}
