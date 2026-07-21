#include "pch.h"
#include "winrt_module_imports.h"
#include "Segmented.h"
#include "../SettingsControls/Helpers/ControlHelper.h"
#if __has_include("Segmented.g.cpp")
#include "Segmented.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> Segmented::OrientationProperty =
		winrt::DependencyProperty::Register(
			L"Orientation",
			winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::Orientation>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata
			{
				winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal),
				&Segmented::OnOrientationChanged
			});

	winrt::Microsoft::UI::Xaml::Controls::Orientation Segmented::Orientation() const
	{
		return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(OrientationProperty()));
	}

	void Segmented::Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value) const
	{
		SetValue(OrientationProperty(), winrt::box_value(value));
	}

	Segmented::Segmented()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		RegisterPropertyChangedCallback(winrt::Selector::SelectedIndexProperty(), { this, &Segmented::OnSelectedIndexChanged });
	}

	void Segmented::PrepareContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item)
	{
		base_type::PrepareContainerForItemOverride(element, item);
		if (auto segmentedItem = element.try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>())
		{
			auto itemImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Controls::implementation::SegmentedItem>(segmentedItem);
			itemImpl->UpdateOrientation(Orientation());
		}
	}

	winrt::DependencyObject Segmented::GetContainerForItemOverride()
	{
		return winrt::make<winrt::XamlToolkit::WinUI::Controls::implementation::SegmentedItem>();
	}

	bool Segmented::IsItemItsOwnContainerOverride(winrt::IInspectable const& item)
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
			if (const auto items = Items(); 
				items && _internalSelectedIndex != -1 && _internalSelectedIndex < static_cast<int>(items.Size()))
			{
				SelectedItem(items.GetAt(_internalSelectedIndex));
			}

			_hasLoaded = true;
		}

		_previewKeyDownRevoker = PreviewKeyDown(winrt::auto_revoke, { this, &Segmented::Segmented_PreviewKeyDown });
	}

	void Segmented::Segmented_PreviewKeyDown([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
	{
		int dir = 0;

		if (FlowDirection() == winrt::FlowDirection::RightToLeft)
		{
			if (e.Key() == winrt::Windows::System::VirtualKey::Left)
				dir = 1;
			else if (e.Key() == winrt::Windows::System::VirtualKey::Right)
				dir = -1;
		}

		if (dir == 0)
		{
			switch (e.Key())
			{
			case winrt::Windows::System::VirtualKey::Left:
			case winrt::Windows::System::VirtualKey::Up:
				dir = -1;
				break;

			case winrt::Windows::System::VirtualKey::Right:
			case winrt::Windows::System::VirtualKey::Down:
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

	void Segmented::OnItemsChanged(winrt::IInspectable const& e)
	{
		base_type::OnItemsChanged(e);
	}

	bool Segmented::MoveFocus(int adjustment)
	{
		auto currentContainerItem = GetCurrentContainerItem();

		if (currentContainerItem)
		{
			auto currentItem = ItemFromContainer(currentContainerItem);

			const auto items = Items();
			uint32_t previousIndex;
			items.IndexOf(currentItem, previousIndex);

			// Apply the adjustment, with a clamp
			const auto index = std::clamp<int>(previousIndex + adjustment, 0, items.Size());

			// Only do stuff if the index is actually changing
			if (index != static_cast<int>(previousIndex))
			{
				if (auto newItem = ContainerFromIndex(index).try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>())
				{
					newItem.Focus(winrt::FocusState::Keyboard);
					return true;
				}
			}
		}

		return false;
	}

	winrt::XamlToolkit::WinUI::Controls::SegmentedItem Segmented::GetCurrentContainerItem()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot())
		{
			return winrt::FocusManager::GetFocusedElement(XamlRoot()).try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>();
		}
		else
		{
			return winrt::FocusManager::GetFocusedElement().try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>();
		}
	}

	void Segmented::OnSelectedIndexChanged([[maybe_unused]] winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyProperty const& dp)
	{
		// This is a workaround for https://github.com/microsoft/microsoft-ui-xaml/issues/8257
		const auto selectedIndex = SelectedIndex();
		if (_internalSelectedIndex == -1 && selectedIndex > -1)
		{
			// We catch the correct SelectedIndex and save it.
			_internalSelectedIndex = selectedIndex;
		}
	}

	void Segmented::OnOrientationChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& dp)
	{
		auto segmented = sender.as<winrt::XamlToolkit::WinUI::Controls::Segmented>();
		uint32_t size = segmented.Items().Size();

		for (uint32_t i = 0; i < size; i++)
		{
			if (auto item = segmented.ContainerFromIndex(i).try_as<winrt::XamlToolkit::WinUI::Controls::SegmentedItem>())
			{
				auto itemImpl = winrt::get_self<winrt::XamlToolkit::WinUI::Controls::implementation::SegmentedItem>(item);
				itemImpl->UpdateOrientation(segmented.Orientation());
			}
		}
	}
}
