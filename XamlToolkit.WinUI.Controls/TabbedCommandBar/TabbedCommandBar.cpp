#include "pch.h"
#include "winrt_module_imports.h"
#include "TabbedCommandBar.h"
#if __has_include("TabbedCommandBar.g.cpp")
#include "TabbedCommandBar.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	TabbedCommandBar::TabbedCommandBar()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		DefaultStyleResourceUri(winrt::Uri(L"ms-appx:///XamlToolkit.WinUI.Controls/Themes/Generic.xaml"));

		SelectionChanged({ this, &TabbedCommandBar::SelectedItemChanged });
		Loaded({ this, &TabbedCommandBar::TabbedCommandBar_Loaded });
	}

	void TabbedCommandBar::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		if (_tabbedCommandBarContent)
		{
			_tabbedCommandBarContent.Content(nullptr);
		}

		// Get TabbedCommandBarContent first, since setting SelectedItem requires it
		_tabbedCommandBarContent = GetTemplateChild(ContentPartName).try_as<winrt::ContentControl>();
		_tabbedCommandBarContentBorder = GetTemplateChild(ContentBorderPartName).try_as<winrt::Border>();
		_tabChangedStoryboard = GetTemplateChild(TabChangedStoryboardPartName).try_as<winrt::Storyboard>();

		// TODO: We could maybe optimize and use a lower-level Loaded event for what's hosting the MenuItems
		// to set SelectedItem, but then we may have to pull in another template part, so think we're OK
		// to do the Loaded event at the top level.
	}

	void TabbedCommandBar::TabbedCommandBar_Loaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		// We need to select the item after the template is realized, otherwise the SelectedItem's
		// DataTemplate bindings don't properly navigate the visual tree.
		auto menuItems = MenuItems();
		SelectedItem(menuItems.Size() > 0 ? menuItems.GetAt(0) : nullptr);
	}

	void TabbedCommandBar::SelectedItemChanged(winrt::NavigationView const& sender, [[maybe_unused]] winrt::NavigationViewSelectionChangedEventArgs const& args)
	{
		auto item = sender.SelectedItem().try_as<winrt::XamlToolkit::WinUI::Controls::TabbedCommandBarItem>();
		if (item == nullptr || item.Visibility() == winrt::Visibility::Collapsed)
		{
			// If the item is now hidden, select the first item instead.
			// I can't think of any way that the visibiltiy would be null
			// and still be selectable, but let's handle it just in case.
			auto menuItems = sender.MenuItems();
			sender.SelectedItem(menuItems.Size() > 0 ? menuItems.GetAt(0) : nullptr);
			return;
		}

		// Remove the visibility PropertyChanged handler from the
		// previously selected item
		if (_previousSelectedItem)
		{
			_previousSelectedItem.UnregisterPropertyChangedCallback(winrt::UIElement::VisibilityProperty(), _visibilityChangedToken);
		}

		// Register a new visibility PropertyChangedcallback for the
		// currently selected item
		_previousSelectedItem = item;
		_visibilityChangedToken =
			_previousSelectedItem.RegisterPropertyChangedCallback(winrt::UIElement::VisibilityProperty(), { this, &TabbedCommandBar::SelectedItemVisibilityChanged });

		// Set the TabbedCommandBar background and start the transition animation
		if (_tabChangedStoryboard) _tabChangedStoryboard.Begin();
	}

	void TabbedCommandBar::SelectedItemVisibilityChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp)
	{
		// If the item is not visible, default to the first tab
		if (auto vis = sender.GetValue(dp).try_as<winrt::Visibility>(); vis && *vis == winrt::Visibility::Collapsed)
		{
			// FIXME: This will cause WinUI to throw an exception if run
			// when the tabs overflow
			auto menuItems = MenuItems();
			SelectedItem(menuItems.Size() > 0 ? menuItems.GetAt(0) : nullptr);
		}
	}
}
