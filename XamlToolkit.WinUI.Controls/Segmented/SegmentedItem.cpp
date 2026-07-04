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
	const wil::single_threaded_property<winrt::DependencyProperty> SegmentedItem::IconProperty =
		winrt::DependencyProperty::Register(
			L"Icon",
			winrt::xaml_typename<winrt::IconElement>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, [](auto& d, auto& e)
			{
				auto self = winrt::get_self<SegmentedItem>(d.template as<class_type>())->get_strong();
				self->OnIconPropertyChanged(winrt::unbox_value<winrt::IconElement>(e.OldValue()), winrt::unbox_value<winrt::IconElement>(e.NewValue()));
			}));

	winrt::IconElement SegmentedItem::Icon() const { return winrt::unbox_value<winrt::IconElement>(GetValue(IconProperty())); }

	void SegmentedItem::Icon(winrt::IconElement const& value) { SetValue(IconProperty(), value); }

	SegmentedItem::SegmentedItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		RegisterPropertyChangedCallback(winrt::UIElement::VisibilityProperty(), { this, &SegmentedItem::OnVisibilityChanged });
	}

	void SegmentedItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		UpdateVisualStates();
	}

	void SegmentedItem::OnVisibilityChanged([[maybe_unused]] winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyProperty const& dp)
	{
		// If the parent is a Segmented control with an EqualPanel,
		// we need to invalidate measure to update the layout.
		if (auto segmented = Parent().try_as<winrt::XamlToolkit::WinUI::Controls::Segmented>())
		{
			if (auto panel = segmented.ItemsPanelRoot().try_as<winrt::Panel>())
			{
				panel.InvalidateMeasure();
			}
		}
	}

	void SegmentedItem::OnContentChanged(winrt::IInspectable const& oldContent, winrt::IInspectable const& newContent)
	{
		base_type::OnContentChanged(oldContent, newContent);
		UpdateVisualStates();
	}

	void SegmentedItem::OnIconPropertyChanged([[maybe_unused]] winrt::IconElement const& oldValue, [[maybe_unused]] winrt::IconElement const& newValue)
	{
		UpdateVisualStates();
	}

	void SegmentedItem::UpdateOrientation(winrt::Orientation orientation)
	{
		_isVertical = orientation == winrt::Orientation::Vertical;
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
		winrt::VisualStateManager::GoToState(*this, contentState, true);
		winrt::VisualStateManager::GoToState(*this, _isVertical ? VerticalState : HorizontalState, true);
	}
}
