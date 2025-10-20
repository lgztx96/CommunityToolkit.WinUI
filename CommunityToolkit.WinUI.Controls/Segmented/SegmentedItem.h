#pragma once

#include "SegmentedItem.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	template <typename D, typename... I>
	using SegmentedItemT = SegmentedItem_base<D, I...>;

	struct SegmentedItem : SegmentedItemT<SegmentedItem>
	{
		static constexpr auto IconLeftState = L"IconLeft";
		static constexpr auto IconOnlyState = L"IconOnly";
		static constexpr auto ContentOnlyState = L"ContentOnly";

		SegmentedItem();

		void OnApplyTemplate();

		void OnContentChanged(IInspectable const& oldContent, IInspectable const& newContent);

		void ContentChanged();

		void OnIconPropertyChanged(IconElement const& oldValue, IconElement const& newValue);

		void OnIconChanged();

		IconElement Icon();

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
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct SegmentedItem : SegmentedItemT<SegmentedItem, implementation::SegmentedItem>
	{
	};
}
