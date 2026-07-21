#include "pch.h"
#include "winrt_module_imports.h"
#include "TokenView.h"
#if __has_include("TokenView.g.cpp")
#include "TokenView.g.cpp"
#endif
#include "../ControlHelpers.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> TokenView::IsWrappedProperty =
		winrt::DependencyProperty::Register(
			L"IsWrapped",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::TokenView>(),
			winrt::PropertyMetadata(winrt::box_value(false), [](auto& d, auto& e)
			{
				if (auto tokenView = d.template try_as<winrt::XamlToolkit::Labs::WinUI::TokenView>())
				{
					auto self = winrt::get_self<TokenView>(tokenView)->get_strong();
					auto oldValue = winrt::unbox_value<bool>(e.OldValue());
					auto newValue = winrt::unbox_value<bool>(e.NewValue());
					self->OnIsWrappedPropertyChanged(oldValue, newValue);
				}
			}));

	const wil::single_threaded_property<winrt::DependencyProperty> TokenView::CanRemoveTokensProperty =
		winrt::DependencyProperty::Register(
			L"CanRemoveTokens",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::TokenView>(),
			winrt::PropertyMetadata(winrt::box_value(false), [](auto& d, auto& e)
			{
				if (auto tokenView = d.template try_as<winrt::XamlToolkit::Labs::WinUI::TokenView>())
				{
					auto self = winrt::get_self<TokenView>(tokenView)->get_strong();
					auto oldValue = winrt::unbox_value<bool>(e.OldValue());
					auto newValue = winrt::unbox_value<bool>(e.NewValue());
					self->OnCanRemoveTokensPropertyChanged(oldValue, newValue);
				}
			}));

	TokenView::TokenView()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		// Container Generation Hooks
		//RegisterPropertyChangedCallback(winrt::ItemsControl::ItemsSourceProperty(), { get_weak(), &TokenView::ItemsSource_PropertyChanged });
		RegisterPropertyChangedCallback(winrt::Selector::SelectedIndexProperty(), { get_weak(), &TokenView::SelectedIndex_PropertyChanged });
	}

	bool TokenView::IsItemItsOwnContainerOverride(winrt::IInspectable const& item)
	{
		return static_cast<bool>(item.try_as<TokenItem>());
	}

	void TokenView::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		SelectedIndex(_internalSelectedIndex);
		_sizeChangedRevoker = SizeChanged(winrt::auto_revoke, { get_weak(), &TokenView::TokenView_SizeChanged });
		if (_tokenViewScroller)
		{
			_tokenViewScrollerLoadedRevoker.revoke();
		}

		_tokenViewScroller = GetTemplateChild(TokenViewScrollViewerName).try_as<winrt::ScrollViewer>();

		if (_tokenViewScroller)
		{
			_tokenViewScrollerLoadedRevoker = _tokenViewScroller.Loaded(winrt::auto_revoke, { get_weak(), &TokenView::ScrollViewer_Loaded });
		}

		_previewKeyDownRevoker = PreviewKeyDown(winrt::auto_revoke, { get_weak(), &TokenView::TokenView_PreviewKeyDown });
		OnIsWrappedChanged();
	}

	void TokenView::PrepareContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item)
	{
		base_type::PrepareContainerForItemOverride(element, item);

		if (auto tokenItem = element.try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>())
		{
			//auto tokenLoadedToken = std::make_shared<winrt::event_token>();
			//*tokenLoadedToken = tokenItem.Loaded([=](auto& s, auto& e) { tokenItem.Loaded(*tokenLoadedToken); Token_Loaded(s, e); });

			tokenItem.Removing({ get_weak(), &TokenView::Token_Removing });

			if (!tokenItem.IsRemoveable() && tokenItem.ReadLocalValue(TokenItem::IsRemoveableProperty()) == winrt::DependencyProperty::UnsetValue())
			{
				winrt::Binding isRemovableBinding;
				isRemovableBinding.Source(*this);
				isRemovableBinding.Path(winrt::PropertyPath(L"CanRemoveTokens"));
				isRemovableBinding.Mode(winrt::BindingMode::OneWay);

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
		if (auto currentContainerItem = GetCurrentContainerItem().try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>();
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
		if (_tokenViewScrollForwardButton && _tokenViewScroller)
		{
			if (_tokenViewScroller.ScrollableWidth() > 0)
			{
				_tokenViewScrollForwardButton.Visibility(winrt::Visibility::Visible);
			}
			else
			{
				_tokenViewScrollForwardButton.Visibility(winrt::Visibility::Collapsed);
			}
		}
	}

	winrt::XamlToolkit::Labs::WinUI::TokenItem TokenView::GetCurrentContainerItem()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
		{
			return winrt::FocusManager::GetFocusedElement(XamlRoot()).try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>();
		}
		else
		{
			return winrt::FocusManager::GetFocusedElement().try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>();
		}
	}

	void TokenView::SelectedIndex_PropertyChanged([[maybe_unused]] winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyProperty const& dp)
	{
		// This is a workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/8257
		if (_internalSelectedIndex == -1 && SelectedIndex() > -1)
		{
			// We catch the correct SelectedIndex and save it.
			_internalSelectedIndex = SelectedIndex();
		}
	}
}
