#include "pch.h"
#include "Segmented.h"
#if __has_include("Segmented.g.cpp")
#include "Segmented.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	Segmented::Segmented()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		RegisterPropertyChangedCallback(Selector::SelectedIndexProperty(), { this, &Segmented::OnSelectedIndexChanged });
	}

	DependencyObject Segmented::GetContainerForItemOverride()
	{
		return winrt::make<CommunityToolkit::WinUI::Controls::implementation::SegmentedItem>();
	}

	bool Segmented::IsItemItsOwnContainerOverride(IInspectable const& item)
	{
		return static_cast<bool>(item.try_as<CommunityToolkit::WinUI::Controls::SegmentedItem>());
	}

	void Segmented::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		if (!_hasLoaded)
		{
			SelectedIndex(-1);
			SelectedIndex(_internalSelectedIndex);
			_hasLoaded = true;
		}

		if (_previewKeyDownEventToken) PreviewKeyDown(_previewKeyDownEventToken);
		_previewKeyDownEventToken = PreviewKeyDown({ this, &Segmented::Segmented_PreviewKeyDown });
	}

	void Segmented::PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item)
	{
		base_type::PrepareContainerForItemOverride(element, item);
		if (auto segmentedItem = element.try_as<CommunityToolkit::WinUI::Controls::SegmentedItem>())
		{
			_loadedEventToken = segmentedItem.Loaded({ this, &Segmented::SegmentedItem_Loaded });
		}
	}

	void Segmented::Segmented_PreviewKeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		switch (e.Key())
		{
		case VirtualKey::Left: e.Handled(MoveFocus(MoveDirection::Previous)); break;
		case VirtualKey::Right: e.Handled(MoveFocus(MoveDirection::Next)); break;
		}
	}

	void Segmented::SegmentedItem_Loaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e) const
	{
		if (auto segmentedItem = sender.try_as<CommunityToolkit::WinUI::Controls::SegmentedItem>())
		{
			if (_loadedEventToken) segmentedItem.Loaded(_loadedEventToken);
		}
	}

	void Segmented::OnItemsChanged(IInspectable const& e)
	{
		base_type::OnItemsChanged(e);
	}

	bool Segmented::MoveFocus(MoveDirection direction)
	{
		bool retVal = false;
		auto currentContainerItem = GetCurrentContainerItem();

		if (currentContainerItem != nullptr)
		{
			auto currentItem = ItemFromContainer(currentContainerItem);
			uint32_t previousIndex;
			Items().IndexOf(currentItem, previousIndex);
			auto index = previousIndex;

			if (direction == MoveDirection::Previous)
			{
				if (previousIndex > 0)
				{
					index -= 1;
				}
				else
				{
					retVal = true;
				}
			}
			else if (direction == MoveDirection::Next)
			{
				if (previousIndex < Items().Size() - 1)
				{
					index += 1;
				}
			}

			// Only do stuff if the index is actually changing
			if (index != previousIndex)
			{
				if (auto newItem = ContainerFromIndex(index).try_as<CommunityToolkit::WinUI::Controls::SegmentedItem>())
				{
					newItem.Focus(FocusState::Keyboard);
					retVal = true;
				}
			}
		}

		return retVal;
	}

	CommunityToolkit::WinUI::Controls::SegmentedItem Segmented::GetCurrentContainerItem()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot() != nullptr)
		{
			return FocusManager::GetFocusedElement(XamlRoot()).try_as<CommunityToolkit::WinUI::Controls::SegmentedItem>();
		}
		else
		{
			return FocusManager::GetFocusedElement().try_as<CommunityToolkit::WinUI::Controls::SegmentedItem>();
		}
	}

	void Segmented::OnSelectedIndexChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		// This is a workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/8257
		if (_internalSelectedIndex == -1 && SelectedIndex() > -1)
		{
			// We catch the correct SelectedIndex and save it.
			_internalSelectedIndex = SelectedIndex();
		}
	}
}
