#pragma once

#include "Segmented.g.h"
#include "SegmentedItem.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
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
	struct Segmented : SegmentedT<Segmented>
	{
		Segmented();

		static const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty;

		winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const;

		void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value) const;

		void PrepareContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item);

		winrt::DependencyObject GetContainerForItemOverride();

		bool IsItemItsOwnContainerOverride(winrt::IInspectable const& item);

		void OnApplyTemplate();

		void Segmented_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void OnItemsChanged(winrt::IInspectable const& e);

		bool MoveFocus(int adjustment);

		winrt::XamlToolkit::WinUI::Controls::SegmentedItem GetCurrentContainerItem();

		void OnSelectedIndexChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		static void OnOrientationChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& dp);

	private:
		int _internalSelectedIndex = -1;
		bool _hasLoaded = false;

		winrt::UIElement::PreviewKeyDown_revoker _previewKeyDownRevoker;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct Segmented : SegmentedT<Segmented, implementation::Segmented>
	{
	};
}
