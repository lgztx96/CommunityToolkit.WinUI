#include "pch.h"
#include "winrt_module_imports.h"
#include "NavigationViewItemAttach.h"
#if __has_include("NavigationViewItemAttach.g.cpp")
#include "NavigationViewItemAttach.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	winrt::IInspectable NavigationViewItemAttach::GetSelectedIcon(winrt::DependencyObject const& obj)
	{
		return obj.GetValue(SelectedIconProperty());
	}

	void NavigationViewItemAttach::SetSelectedIcon(winrt::DependencyObject const& obj, winrt::IInspectable const& value)
	{
		obj.SetValue(SelectedIconProperty(), value);
	}

	bool NavigationViewItemAttach::GetShowNotificationDot(winrt::DependencyObject const& obj)
	{
		return winrt::unbox_value<bool>(obj.GetValue(ShowNotificationDotProperty()));
	}

	void NavigationViewItemAttach::SetShowNotificationDot(winrt::DependencyObject const& obj, bool value)
	{
		obj.SetValue(ShowNotificationDotProperty(), winrt::box_value(value));
	}

	winrt::IInspectable NavigationViewItemAttach::GetUnselectedIcon(winrt::DependencyObject const& obj)
	{
		return obj.GetValue(UnselectedIconProperty());
	}

	void NavigationViewItemAttach::SetUnselectedIcon(winrt::DependencyObject const& obj, winrt::IInspectable const& value)
	{
		obj.SetValue(UnselectedIconProperty(), value);
	}

	winrt::Visibility NavigationViewItemAttach::GetStaticIconVisibility(winrt::DependencyObject const& obj)
	{
		return winrt::unbox_value<Visibility>(obj.GetValue(StaticIconVisibilityProperty()));
	}

	void NavigationViewItemAttach::SetStaticIconVisibility(winrt::DependencyObject const& obj, winrt::Visibility const& value)
	{
		obj.SetValue(StaticIconVisibilityProperty(), winrt::box_value(value));
	}
}
