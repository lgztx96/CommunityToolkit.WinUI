#pragma once

#include "SegmentedItem.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace winrt::Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SegmentedItem : SegmentedItemT<SegmentedItem>
	{
		static constexpr auto IconLeftState = L"IconLeft";
		static constexpr auto IconTopState = L"IconTop";
		static constexpr auto IconOnlyState = L"IconOnly";
		static constexpr auto ContentOnlyState = L"ContentOnly";
		static constexpr auto HorizontalState = L"Horizontal";
		static constexpr auto VerticalState = L"Vertical";

		SegmentedItem();

		void OnApplyTemplate();

		void OnVisibilityChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

		void OnContentChanged(winrt::IInspectable const& oldContent, winrt::IInspectable const& newContent);

		void OnIconPropertyChanged(winrt::IconElement const& oldValue, winrt::IconElement const& newValue);

		void UpdateOrientation(winrt::Orientation orientation);

		void UpdateVisualStates();

		winrt::IconElement Icon() const;

		void Icon(winrt::IconElement const& value);

		static const wil::single_threaded_property<winrt::DependencyProperty> IconProperty;

	private:
		bool _isVertical = false;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct SegmentedItem : SegmentedItemT<SegmentedItem, implementation::SegmentedItem>
	{
	};
}
