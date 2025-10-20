#include "pch.h"
#include "SegmentedItem.h"
#if __has_include("SegmentedItem.g.cpp")
#include "SegmentedItem.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	SegmentedItem::SegmentedItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void SegmentedItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		OnIconChanged();
		ContentChanged();
	}

	void SegmentedItem::OnContentChanged(IInspectable const& oldContent, IInspectable const& newContent)
	{
		base_type::OnContentChanged(oldContent, newContent);
		ContentChanged();
	}

	void SegmentedItem::ContentChanged()
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

	void SegmentedItem::OnIconPropertyChanged([[maybe_unused]] IconElement const& oldValue, [[maybe_unused]] IconElement const& newValue)
	{
		OnIconChanged();
	}

	void SegmentedItem::OnIconChanged()
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

	IconElement SegmentedItem::Icon() { return winrt::unbox_value<IconElement>(GetValue(IconProperty)); }
	void SegmentedItem::Icon(IconElement const& value) { SetValue(IconProperty, value); }
}
