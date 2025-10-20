#pragma once

#include "Segmented.g.h"
#include "SegmentedItem.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	template <typename D, typename... I>
	using SegmentedT = Segmented_base<D, I...>;
}

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace Windows::System;
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;

	struct Segmented : SegmentedT<Segmented>
	{
		int _internalSelectedIndex = -1;
		bool _hasLoaded = false;

		winrt::event_token _previewKeyDownEventToken{};
		winrt::event_token _loadedEventToken{};

		Segmented();

		DependencyObject GetContainerForItemOverride();

		bool IsItemItsOwnContainerOverride(IInspectable const& item);

		void OnApplyTemplate();

		void PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item);

		void Segmented_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void SegmentedItem_Loaded(IInspectable const& sender, RoutedEventArgs const& e) const;

		void OnItemsChanged(IInspectable const& e);

		enum class MoveDirection
		{
			Next,
			Previous
		};

		/// <summary>
		/// Adjust the selected item and range based on keyboard input.
		/// This is used to override the ListView behaviors for up/down arrow manipulation vs left/right for a horizontal control
		/// </summary>
		/// <param name="direction">direction to move the selection</param>
		/// <returns>True if the focus was moved, false otherwise</returns>
		bool MoveFocus(MoveDirection direction);

		CommunityToolkit::WinUI::Controls::SegmentedItem GetCurrentContainerItem();

		void OnSelectedIndexChanged(DependencyObject const& sender, DependencyProperty const& dp);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct Segmented : SegmentedT<Segmented, implementation::Segmented>
	{
	};
}
