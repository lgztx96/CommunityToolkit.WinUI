#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <string_view>
#endif
#include "SegmentedItem.h"
#if __has_include("SegmentedItem.g.cpp")
#include "SegmentedItem.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	IconElement SegmentedItem::Icon() const { return winrt::unbox_value<IconElement>(GetValue(IconProperty)); }

	void SegmentedItem::Icon(IconElement const& value) { SetValue(IconProperty, value); }

	SegmentedItem::SegmentedItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		RegisterPropertyChangedCallback(UIElement::VisibilityProperty(), { this, &SegmentedItem::OnVisibilityChanged });
	}

	void SegmentedItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		UpdateVisualStates();
	}

	void SegmentedItem::OnVisibilityChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		// If the parent is a Segmented control with an EqualPanel,
		// we need to invalidate measure to update the layout.
		if (auto segmented = Parent().try_as<winrt::XamlToolkit::WinUI::Controls::Segmented>())
		{
			if (auto panel = segmented.ItemsPanelRoot().try_as<Panel>())
			{
				panel.InvalidateMeasure();
			}
		}
	}

	void SegmentedItem::OnContentChanged(IInspectable const& oldContent, IInspectable const& newContent)
	{
		base_type::OnContentChanged(oldContent, newContent);
		UpdateVisualStates();
	}

	void SegmentedItem::OnIconPropertyChanged([[maybe_unused]] IconElement const& oldValue, [[maybe_unused]] IconElement const& newValue)
	{
		UpdateVisualStates();
	}

	void SegmentedItem::UpdateOrientation(Orientation orientation)
	{
		_isVertical = orientation == Orientation::Vertical;
		UpdateVisualStates();
	}

	void SegmentedItem::UpdateVisualStates()
	{
		std::wstring_view contentState;

		const bool iconIsNull = (Icon() == nullptr);
		const bool contentIsNull = (Content() == nullptr);

		if (!iconIsNull && !contentIsNull)
		{
			contentState = _isVertical ? IconTopState : IconLeftState;
		}
		else if (!iconIsNull && contentIsNull)
		{
			contentState = IconOnlyState;
		}
		else
		{
			contentState = ContentOnlyState;
		}

		// Update visual states
		VisualStateManager::GoToState(*this, contentState, true);
		VisualStateManager::GoToState(*this, _isVertical ? VerticalState : HorizontalState, true);
	}
}
