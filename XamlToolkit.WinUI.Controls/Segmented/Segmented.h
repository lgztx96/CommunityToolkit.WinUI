#pragma once

#include "Segmented.g.h"
#include "SegmentedItem.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt 
{
	using namespace Windows::System;
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	template <typename D, typename... I>
	using SegmentedT = Segmented_base<D, I...>;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct Segmented : SegmentedT<Segmented>
	{
		Segmented();

		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OrientationProperty;

		winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const;

		void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value) const;

		void PrepareContainerForItemOverride(DependencyObject const& element, IInspectable const& item);

		DependencyObject GetContainerForItemOverride();

		bool IsItemItsOwnContainerOverride(IInspectable const& item);

		void OnApplyTemplate();

		void Segmented_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void OnItemsChanged(IInspectable const& e);

		enum class MoveDirection
		{
			Next,
			Previous
		};

		bool MoveFocus(int adjustment);

		XamlToolkit::WinUI::Controls::SegmentedItem GetCurrentContainerItem();

		void OnSelectedIndexChanged(DependencyObject const& sender, DependencyProperty const& dp);

		static void OnOrientationChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& dp);

	private:
		int _internalSelectedIndex = -1;
		bool _hasLoaded = false;

		UIElement::PreviewKeyDown_revoker _previewKeyDownRevoker;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct Segmented : SegmentedT<Segmented, implementation::Segmented>
	{
	};
}
