#pragma once

#include "SegmentedItem.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	template <typename D, typename... I>
	using SegmentedItemT = SegmentedItem_base<D, I...>;

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

		void OnVisibilityChanged(DependencyObject const& sender, DependencyProperty const& dp);

		void OnContentChanged(IInspectable const& oldContent, IInspectable const& newContent);

		void OnIconPropertyChanged(IconElement const& oldValue, IconElement const& newValue);

		void UpdateOrientation(Orientation orientation);

		void UpdateVisualStates();

		IconElement Icon() const;

		void Icon(IconElement const& value);

		static inline const wil::single_threaded_property<DependencyProperty> IconProperty = DependencyProperty::Register(
			L"Icon",
			winrt::xaml_typename<IconElement>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(nullptr, [](auto& d, auto& e)
				{
					auto self = winrt::get_self<SegmentedItem>(d.template as<class_type>())->get_strong();
					self->OnIconPropertyChanged(winrt::unbox_value<IconElement>(e.OldValue()), winrt::unbox_value<IconElement>(e.NewValue()));
				}));

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
