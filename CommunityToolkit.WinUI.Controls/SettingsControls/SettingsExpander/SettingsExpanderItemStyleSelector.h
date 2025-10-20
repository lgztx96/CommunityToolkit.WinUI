#pragma once

#include "SettingsExpanderItemStyleSelector.g.h"

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;

	struct SettingsExpanderItemStyleSelector : SettingsExpanderItemStyleSelectorT<SettingsExpanderItemStyleSelector>
	{
		wil::single_threaded_rw_property<Style> DefaultStyle{ nullptr };

		wil::single_threaded_rw_property<Style> ClickableStyle{ nullptr };

		SettingsExpanderItemStyleSelector() = default;

		Style SelectStyleCore(winrt::Windows::Foundation::IInspectable const& item, DependencyObject const& container);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct SettingsExpanderItemStyleSelector : SettingsExpanderItemStyleSelectorT<SettingsExpanderItemStyleSelector, implementation::SettingsExpanderItemStyleSelector>
	{
	};
}
