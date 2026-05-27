#include "pch.h"
#include "winrt_module_imports.h"
#include "Segmented.h"
#include "../SettingsControls/Helpers/ControlHelper.h"
#if __has_include("Segmented.g.cpp")
#include "Segmented.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> Segmented::OrientationProperty =
		winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
			L"Orientation",
			winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::Orientation>(),
			winrt::xaml_typename<class_type>(),
			winrt::Microsoft::UI::Xaml::PropertyMetadata
			{
				winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal),
				&Segmented::OnOrientationChanged
			});

	winrt::Microsoft::UI::Xaml::Controls::Orientation Segmented::Orientation() const
	{
		return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(OrientationProperty));
	}

	void Segmented::Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value) const
	{
		SetValue(OrientationProperty, winrt::box_value(value));
	}

	Segmented::Segmented()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		RegisterPropertyChangedCallback(Selector::SelectedIndexProperty(), { this, &Segmented::OnSelectedIndexChanged });
	}

	void Segmented::PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item)
	{
		base_type::PrepareContainerForItemOverride(element, item);
		if (auto segmentedItem = element.try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>())
		{
			auto itemImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Controls::implementation::SegmentedItem>(segmentedItem);
			itemImpl->UpdateOrientation(Orientation());
		}
	}

	DependencyObject Segmented::GetContainerForItemOverride()
	{
		return winrt::make<winrt::XamlToolkit::WinUI::Controls::implementation::SegmentedItem>();
	}

	bool Segmented::IsItemItsOwnContainerOverride(IInspectable const& item)
	{
		return static_cast<bool>(item.try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>());
	}

	void Segmented::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		if (!_hasLoaded)
		{
			// https://github.com/CommunityToolkit/Windows/issues/698
			// https://github.com/CommunityToolkit/Windows/issues/843
			if (auto items = Items(); _internalSelectedIndex != -1 && _internalSelectedIndex < static_cast<int>(items.Size()))
			{
				SelectedItem(items.GetAt(_internalSelectedIndex));
			}

			_hasLoaded = true;
		}
		
		_previewKeyDownRevoker = PreviewKeyDown(winrt::auto_revoke, { this, &Segmented::Segmented_PreviewKeyDown });
	}

	void Segmented::Segmented_PreviewKeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		int dir = 0;

		if (FlowDirection() == FlowDirection::RightToLeft)
		{
			if (e.Key() == VirtualKey::Left)
				dir = 1;
			else if (e.Key() == VirtualKey::Right)
				dir = -1;
		}

		if (dir == 0)
		{
			switch (e.Key())
			{
			case VirtualKey::Left:
			case VirtualKey::Up:
				dir = -1;
				break;

			case VirtualKey::Right:
			case VirtualKey::Down:
				dir = 1;
				break;

			default:
				std::unreachable();
			}
		}

		if (dir != 0)
		{
			e.Handled(MoveFocus(dir));
		}
	}

	void Segmented::OnItemsChanged(IInspectable const& e)
	{
		base_type::OnItemsChanged(e);
	}

	bool Segmented::MoveFocus(int adjustment)
	{
		auto currentContainerItem = GetCurrentContainerItem();

		if (currentContainerItem != nullptr)
		{
			auto currentItem = ItemFromContainer(currentContainerItem);
			uint32_t previousIndex;
			Items().IndexOf(currentItem, previousIndex);

			// Apply the adjustment, with a clamp
			auto index = std::clamp<int>(previousIndex + adjustment, 0, Items().Size());

			// Only do stuff if the index is actually changing
			if (index != static_cast<int>(previousIndex))
			{
				if (auto newItem = ContainerFromIndex(index).try_as<XamlToolkit::WinUI::Controls::SegmentedItem>())
				{
					newItem.Focus(FocusState::Keyboard);
					return true;
				}
			}
		}

		return false;
	}

	winrt::XamlToolkit::WinUI::Controls::SegmentedItem Segmented::GetCurrentContainerItem()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot() != nullptr)
		{
			return FocusManager::GetFocusedElement(XamlRoot()).try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>();
		}
		else
		{
			return FocusManager::GetFocusedElement().try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>();
		}
	}

	void Segmented::OnSelectedIndexChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		// This is a workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/8257
		auto selectedIndex = SelectedIndex();
		if (_internalSelectedIndex == -1 && selectedIndex > -1)
		{
			// We catch the correct SelectedIndex and save it.
			_internalSelectedIndex = selectedIndex;
		}
	}

	void Segmented::OnOrientationChanged(DependencyObject const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& dp)
	{
		auto segmented = sender.as<winrt::XamlToolkit::WinUI::Controls::Segmented>();
		for (uint32_t i = 0; i < segmented.Items().Size(); i++)
		{
			if (auto item = segmented.ContainerFromIndex(i).try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>())
			{
				auto itemImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Controls::implementation::SegmentedItem>(item);
				itemImpl->UpdateOrientation(segmented.Orientation());
			}
		}
	}
}
