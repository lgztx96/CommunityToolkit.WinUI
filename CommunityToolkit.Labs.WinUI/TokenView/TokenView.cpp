#include "pch.h"
#include "TokenView.h"
#if __has_include("TokenView.g.cpp")
#include "TokenView.g.cpp"
#endif
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include "../ControlHelpers.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Data;

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	TokenView::TokenView()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		// Container Generation Hooks
		//RegisterPropertyChangedCallback(ItemsControl::ItemsSourceProperty(), { get_weak(), &TokenView::ItemsSource_PropertyChanged });
		RegisterPropertyChangedCallback(Selector::SelectedIndexProperty(), { get_weak(), &TokenView::SelectedIndex_PropertyChanged });
	}

	bool TokenView::IsItemItsOwnContainerOverride(IInspectable const& item)
	{
		return item.try_as<TokenItem>() != nullptr;
	}

	void TokenView::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		SelectedIndex(_internalSelectedIndex);
		_sizeChangedRevoker = SizeChanged(winrt::auto_revoke, { get_weak(), &TokenView::TokenView_SizeChanged });
		if (_tokenViewScroller != nullptr)
		{
			_tokenViewScrollerLoadedRevoker.revoke();
		}

		_tokenViewScroller = GetTemplateChild(TokenViewScrollViewerName).try_as<ScrollViewer>();

		if (_tokenViewScroller != nullptr)
		{
			_tokenViewScrollerLoadedRevoker = _tokenViewScroller.Loaded(winrt::auto_revoke, { get_weak(), &TokenView::ScrollViewer_Loaded });
		}

		_previewKeyDownRevoker = PreviewKeyDown(winrt::auto_revoke, { get_weak(), &TokenView::TokenView_PreviewKeyDown });
		OnIsWrappedChanged();
	}

	void TokenView::PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item)
	{
		base_type::PrepareContainerForItemOverride(element, item);

		if (auto tokenItem = element.try_as<winrt::CommunityToolkit::Labs::WinUI::TokenItem>())
		{
			//auto tokenLoadedToken = std::make_shared<winrt::event_token>();
			//*tokenLoadedToken = tokenItem.Loaded([=](auto& s, auto& e) { tokenItem.Loaded(*tokenLoadedToken); Token_Loaded(s, e); });

			tokenItem.Removing({ get_weak(), &TokenView::Token_Removing });

			if (tokenItem.IsRemoveable() != true && tokenItem.ReadLocalValue(TokenItem::IsRemoveableProperty()) == DependencyProperty::UnsetValue())
			{
				Binding isRemovableBinding;
				isRemovableBinding.Source(*this);
				isRemovableBinding.Path(PropertyPath(L"CanRemoveTokens"));
				isRemovableBinding.Mode(BindingMode::OneWay);

				tokenItem.SetBinding(TokenItem::IsRemoveableProperty(), isRemovableBinding);
			}
		}
	}

	void TokenView::OnIsWrappedPropertyChanged([[maybe_unused]] bool oldValue, [[maybe_unused]] bool newValue)
	{
		OnIsWrappedChanged();
	}

	void TokenView::OnCanRemoveTokensPropertyChanged([[maybe_unused]] bool oldValue, [[maybe_unused]] bool newValue)
	{
		OnCanRemoveTokensChanged();
	}

	bool TokenView::RemoveItem()
	{
		if (auto currentContainerItem = GetCurrentContainerItem().try_as<winrt::CommunityToolkit::Labs::WinUI::TokenItem>();
			currentContainerItem && currentContainerItem.IsRemoveable())
		{
			uint32_t index;
			auto items = Items();
			if (items.IndexOf(currentContainerItem, index))
			{
				items.RemoveAt(index);
				return true;
			}
		}

		return false;
	}

	void TokenView::UpdateScrollButtonsVisibility()
	{
		if (_tokenViewScrollForwardButton != nullptr && _tokenViewScroller != nullptr)
		{
			if (_tokenViewScroller.ScrollableWidth() > 0)
			{
				_tokenViewScrollForwardButton.Visibility(Visibility::Visible);
			}
			else
			{
				_tokenViewScrollForwardButton.Visibility(Visibility::Collapsed);
			}
		}
	}

	winrt::CommunityToolkit::Labs::WinUI::TokenItem TokenView::GetCurrentContainerItem()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot() != nullptr)
		{
			return FocusManager::GetFocusedElement(XamlRoot()).try_as<winrt::CommunityToolkit::Labs::WinUI::TokenItem>();
		}
		else
		{
			return FocusManager::GetFocusedElement().try_as<winrt::CommunityToolkit::Labs::WinUI::TokenItem>();
		}
	}

	void TokenView::SelectedIndex_PropertyChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		// This is a workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/8257
		if (_internalSelectedIndex == -1 && SelectedIndex() > -1)
		{
			// We catch the correct SelectedIndex and save it.
			_internalSelectedIndex = SelectedIndex();
		}
	}
}
