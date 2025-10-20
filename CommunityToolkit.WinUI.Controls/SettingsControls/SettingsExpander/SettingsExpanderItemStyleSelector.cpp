#include "pch.h"
#include "SettingsExpanderItemStyleSelector.h"
#if __has_include("SettingsExpanderItemStyleSelector.g.cpp")
#include "SettingsExpanderItemStyleSelector.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	Style SettingsExpanderItemStyleSelector::SelectStyleCore(
		[[maybe_unused]] winrt::Windows::Foundation::IInspectable const& item, DependencyObject const& container)
	{
		if (auto card = container.try_as<CommunityToolkit::WinUI::Controls::SettingsCard>(); card && card.IsClickEnabled())
		{
			return ClickableStyle;
		}
		else
		{
			return DefaultStyle;
		}
	}
}
