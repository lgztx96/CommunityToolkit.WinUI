#include "pch.h"
#include "TabbedCommandBar.h"
#if __has_include("TabbedCommandBar.g.cpp")
#include "TabbedCommandBar.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	TabbedCommandBar::TabbedCommandBar()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		DefaultStyleResourceUri(Uri(L"ms-appx:///CommunityToolkit.WinUI.Controls/Themes/Generic.xaml"));

		SelectionChanged({ this, &TabbedCommandBar::SelectedItemChanged });
		Loaded({ this, &TabbedCommandBar::TabbedCommandBar_Loaded });
	}

	void TabbedCommandBar::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		if (_tabbedCommandBarContent != nullptr)
		{
			_tabbedCommandBarContent.Content(nullptr);
		}

		// Get TabbedCommandBarContent first, since setting SelectedItem requires it
		_tabbedCommandBarContent = GetTemplateChild(L"PART_TabbedCommandBarContent").try_as<ContentControl>();
		_tabbedCommandBarContentBorder = GetTemplateChild(L"PART_TabbedCommandBarContentBorder").try_as<Border>();
		_tabChangedStoryboard = GetTemplateChild(L"TabChangedStoryboard").try_as<Storyboard>();

		// TODO: We could maybe optimize and use a lower-level Loaded event for what's hosting the MenuItems
		// to set SelectedItem, but then we may have to pull in another template part, so think we're OK
		// to do the Loaded event at the top level.
	}

	void TabbedCommandBar::TabbedCommandBar_Loaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		// We need to select the item after the template is realized, otherwise the SelectedItem's
		// DataTemplate bindings don't properly navigate the visual tree.
		SelectedItem(MenuItems().Size() > 0 ? MenuItems().GetAt(0) : nullptr);
	}

	void TabbedCommandBar::SelectedItemChanged(NavigationView const& sender, [[maybe_unused]] NavigationViewSelectionChangedEventArgs const& args)
	{
		auto item = sender.SelectedItem().try_as<winrt::CommunityToolkit::WinUI::Controls::TabbedCommandBarItem>();
		if (item == nullptr || item.Visibility() == Visibility::Collapsed)
		{
			// If the item is now hidden, select the first item instead.
			// I can't think of any way that the visibiltiy would be null
			// and still be selectable, but let's handle it just in case.
			sender.SelectedItem(sender.MenuItems().Size() > 0 ? sender.MenuItems().GetAt(0) : nullptr);
			return;
		}

		// Remove the visibility PropertyChanged handler from the
		// previously selected item
		if (_previousSelectedItem != nullptr)
		{
			_previousSelectedItem.UnregisterPropertyChangedCallback(UIElement::VisibilityProperty(), _visibilityChangedToken);
		}

		// Register a new visibility PropertyChangedcallback for the
		// currently selected item
		_previousSelectedItem = item;
		_visibilityChangedToken =
			_previousSelectedItem.RegisterPropertyChangedCallback(UIElement::VisibilityProperty(), { this, &TabbedCommandBar::SelectedItemVisibilityChanged });

		// Set the TabbedCommandBar background and start the transition animation
		if (_tabChangedStoryboard) _tabChangedStoryboard.Begin();
	}

	void TabbedCommandBar::SelectedItemVisibilityChanged(DependencyObject const& sender, DependencyProperty const& dp)
	{
		// If the item is not visible, default to the first tab
		if (auto vis = sender.GetValue(dp).try_as<enum Visibility>(); vis && vis == Visibility::Collapsed)
		{
			// FIXME: This will cause WinUI to throw an exception if run
			// when the tabs overflow
			SelectedItem(MenuItems().Size() > 0 ? MenuItems().GetAt(0) : nullptr);
		}
	}
}
