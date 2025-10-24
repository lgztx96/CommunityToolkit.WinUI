#pragma once

#include "TabbedCommandBar.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct TabbedCommandBar : TabbedCommandBarT<TabbedCommandBar>
	{
	private:
		ContentControl _tabbedCommandBarContent{ nullptr };
		Border _tabbedCommandBarContentBorder{ nullptr };
		Storyboard _tabChangedStoryboard{ nullptr };

		winrt::CommunityToolkit::WinUI::Controls::TabbedCommandBarItem _previousSelectedItem{ nullptr };
		int64_t _visibilityChangedToken;

	public:
		TabbedCommandBar();

		void OnApplyTemplate();

	private:
		void TabbedCommandBar_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		void SelectedItemChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);

		void SelectedItemVisibilityChanged(DependencyObject const& sender, DependencyProperty const& dp);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct TabbedCommandBar : TabbedCommandBarT<TabbedCommandBar, implementation::TabbedCommandBar>
	{
	};
}
