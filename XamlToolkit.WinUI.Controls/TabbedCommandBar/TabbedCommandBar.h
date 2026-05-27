#pragma once

#include "TabbedCommandBar.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct TabbedCommandBar : TabbedCommandBarT<TabbedCommandBar>
	{
	private:
		static constexpr auto ContentPartName = L"PART_TabbedCommandBarContent";
		static constexpr auto ContentBorderPartName = L"PART_TabbedCommandBarContentBorder";
		static constexpr auto TabChangedStoryboardPartName = L"TabChangedStoryboard";

		ContentControl _tabbedCommandBarContent{ nullptr };
		Border _tabbedCommandBarContentBorder{ nullptr };
		Storyboard _tabChangedStoryboard{ nullptr };

		winrt::XamlToolkit::WinUI::Controls::TabbedCommandBarItem _previousSelectedItem{ nullptr };
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

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct TabbedCommandBar : TabbedCommandBarT<TabbedCommandBar, implementation::TabbedCommandBar>
	{
	};
}
