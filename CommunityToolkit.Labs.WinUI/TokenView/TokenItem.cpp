#include "pch.h"
#include "TokenItem.h"
#if __has_include("TokenItem.g.cpp")
#include "TokenItem.g.cpp"
#endif
#include "TokenItemRemovingEventArgs.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	TokenItem::TokenItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void TokenItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		_removeButtonClickRevoker.revoke();

		_tokenItemRemoveButton = GetTemplateChild(TokenItemRemoveButtonName).try_as<ButtonBase>();

		if (_tokenItemRemoveButton != nullptr)
		{
			_tokenItemRemoveButton.Click({ get_weak(), &TokenItem::TokenItemRemoveButton_Click });
		}

		IconChanged();
		ContentChanged();
		IsRemoveableChanged();
	}

	void TokenItem::TokenItemRemoveButton_Click([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (IsRemoveable())
		{
			Removing.invoke(*this, winrt::make<TokenItemRemovingEventArgs>(Content(), *this));
		}
	}

	void TokenItem::OnContentChanged(IInspectable const& oldContent, IInspectable const& newContent)
	{
		base_type::OnContentChanged(oldContent, newContent);
		ContentChanged();
	}

	void TokenItem::ContentChanged()
	{
		if (Content() != nullptr)
		{
			VisualStateManager::GoToState(*this, IconLeftState, true);
		}
		else
		{
			VisualStateManager::GoToState(*this, IconOnlyState, true);
		}
	}

	void TokenItem::OnIconPropertyChanged([[maybe_unused]] IconElement const& oldValue, [[maybe_unused]] IconElement const& newValue)
	{
		IconChanged();
	}

	void TokenItem::IconChanged()
	{
		if (Icon() != nullptr)
		{
			VisualStateManager::GoToState(*this, IconLeftState, true);
		}
		else
		{
			VisualStateManager::GoToState(*this, ContentOnlyState, true);
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
			VisualStateManager::GoToState(*this, RemoveButtonVisibleState, true);
		}
		else
		{
			VisualStateManager::GoToState(*this, RemoveButtonNotVisibleState, true);
		}
	}
}
