#include "pch.h"
#include "winrt_module_imports.h"
#include "TokenItem.h"
#if __has_include("TokenItem.g.cpp")
#include "TokenItem.g.cpp"
#endif
#include "TokenItemRemovingEventArgs.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> TokenItem::IsRemoveableProperty =
		winrt::DependencyProperty::Register(
			L"IsRemoveable",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::TokenItem>(),
			winrt::PropertyMetadata(winrt::box_value(false), [](auto& d, auto& e)
			{
				if (auto tokenItem = d.template try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>())
				{
					auto self = winrt::get_self<TokenItem>(tokenItem)->get_strong();
					auto oldValue = winrt::unbox_value<bool>(e.OldValue());
					auto newValue = winrt::unbox_value<bool>(e.NewValue());
					self->OnIsRemoveablePropertyChanged(oldValue, newValue);
				}
			}));

	const wil::single_threaded_property<winrt::DependencyProperty> TokenItem::IconProperty =
		winrt::DependencyProperty::Register(
			L"Icon",
			winrt::xaml_typename<winrt::IconElement>(),
			winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::TokenItem>(),
			winrt::PropertyMetadata(nullptr, [](auto& d, auto& e)
			{
				if (auto tokenItem = d.template try_as<winrt::XamlToolkit::Labs::WinUI::TokenItem>())
				{
					auto self = winrt::get_self<TokenItem>(tokenItem)->get_strong();
					auto oldValue = e.OldValue().try_as<winrt::IconElement>();
					auto newValue = e.NewValue().try_as<winrt::IconElement>();
					self->OnIconPropertyChanged(oldValue, newValue);
				}
			}));

	TokenItem::TokenItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void TokenItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		_removeButtonClickRevoker.revoke();

		_tokenItemRemoveButton = GetTemplateChild(TokenItemRemoveButtonName).try_as<winrt::ButtonBase>();

		if (_tokenItemRemoveButton)
		{
			_tokenItemRemoveButton.Click({ get_weak(), &TokenItem::TokenItemRemoveButton_Click });
		}

		IconChanged();
		ContentChanged();
		IsRemoveableChanged();
	}

	void TokenItem::TokenItemRemoveButton_Click([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		if (IsRemoveable())
		{
			Removing.invoke(*this, winrt::make<TokenItemRemovingEventArgs>(Content(), *this));
		}
	}

	void TokenItem::OnContentChanged(winrt::IInspectable const& oldContent, winrt::IInspectable const& newContent)
	{
		base_type::OnContentChanged(oldContent, newContent);
		ContentChanged();
	}

	void TokenItem::ContentChanged()
	{
		if (Content())
		{
			winrt::VisualStateManager::GoToState(*this, IconLeftState, true);
		}
		else
		{
			winrt::VisualStateManager::GoToState(*this, IconOnlyState, true);
		}
	}

	void TokenItem::OnIconPropertyChanged([[maybe_unused]] winrt::IconElement const& oldValue, [[maybe_unused]] winrt::IconElement const& newValue)
	{
		IconChanged();
	}

	void TokenItem::IconChanged()
	{
		if (Icon())
		{
			winrt::VisualStateManager::GoToState(*this, IconLeftState, true);
		}
		else
		{
			winrt::VisualStateManager::GoToState(*this, ContentOnlyState, true);
		}
	}

	void TokenItem::OnIsRemoveablePropertyChanged([[maybe_unused]] bool oldValue, [[maybe_unused]] bool newValue)
	{
		IsRemoveableChanged();
	}

	void TokenItem::IsRemoveableChanged()
	{
		if (IsRemoveable())
		{
			winrt::VisualStateManager::GoToState(*this, RemoveButtonVisibleState, true);
		}
		else
		{
			winrt::VisualStateManager::GoToState(*this, RemoveButtonNotVisibleState, true);
		}
	}
}
