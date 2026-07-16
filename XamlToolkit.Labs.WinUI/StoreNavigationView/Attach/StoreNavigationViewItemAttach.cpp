#include "pch.h"
#include "winrt_module_imports.h"
#include "StoreNavigationViewItemAttach.h"
#if __has_include("StoreNavigationViewItemAttach.g.cpp")
#include "StoreNavigationViewItemAttach.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	winrt::IInspectable StoreNavigationViewItemAttach::GetSelectedIcon(winrt::DependencyObject const& obj)
	{
		return obj.GetValue(SelectedIconProperty());
	}

	void StoreNavigationViewItemAttach::SetSelectedIcon(winrt::DependencyObject const& obj, winrt::IInspectable const& value)
	{
		obj.SetValue(SelectedIconProperty(), value);
	}

	bool StoreNavigationViewItemAttach::GetShowNotificationDot(winrt::DependencyObject const& obj)
	{
		return winrt::unbox_value<bool>(obj.GetValue(ShowNotificationDotProperty()));
	}

	void StoreNavigationViewItemAttach::SetShowNotificationDot(winrt::DependencyObject const& obj, bool value)
	{
		obj.SetValue(ShowNotificationDotProperty(), winrt::box_value(value));
	}

	winrt::IInspectable StoreNavigationViewItemAttach::GetUnselectedIcon(winrt::DependencyObject const& obj)
	{
		return obj.GetValue(UnselectedIconProperty());
	}

	void StoreNavigationViewItemAttach::SetUnselectedIcon(winrt::DependencyObject const& obj, winrt::IInspectable const& value)
	{
		obj.SetValue(UnselectedIconProperty(), value);
	}

	winrt::Visibility StoreNavigationViewItemAttach::GetStaticIconVisibility(winrt::DependencyObject const& obj)
	{
		return winrt::unbox_value<Visibility>(obj.GetValue(StaticIconVisibilityProperty()));
	}

	void StoreNavigationViewItemAttach::SetStaticIconVisibility(winrt::DependencyObject const& obj, winrt::Visibility const& value)
	{
		obj.SetValue(StaticIconVisibilityProperty(), winrt::box_value(value));
	}
}
