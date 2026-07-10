#include "pch.h"
#include "winrt_module_imports.h"
#include "HeaderedTreeView.h"
#if __has_include("HeaderedTreeView.g.cpp")
#include "HeaderedTreeView.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	HeaderedTreeView::HeaderedTreeView()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		DefaultStyleResourceUri(winrt::Uri(L"ms-appx:///XamlToolkit.WinUI.Controls/Themes/Generic.xaml"));
	}

	void HeaderedTreeView::OnFooterChanged([[maybe_unused]] winrt::IInspectable const& oldValue, [[maybe_unused]] winrt::IInspectable const& newValue) {}

	void HeaderedTreeView::OnHeaderChanged([[maybe_unused]] winrt::IInspectable const& oldValue, [[maybe_unused]] winrt::IInspectable const& newValue) {}

	void HeaderedTreeView::OnFooterChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto control = winrt::get_self<HeaderedTreeView>(d.as<class_type>())->get_strong();
		control->OnFooterChanged(e.OldValue(), e.NewValue());
	}

	void HeaderedTreeView::OnHeaderChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto control = winrt::get_self<HeaderedTreeView>(d.as<class_type>())->get_strong();
		control->OnHeaderChanged(e.OldValue(), e.NewValue());
	}
}
