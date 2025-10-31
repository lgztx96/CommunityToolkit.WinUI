#include "pch.h"
#include "HeaderedTreeView.h"
#if __has_include("HeaderedTreeView.g.cpp")
#include "HeaderedTreeView.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	HeaderedTreeView::HeaderedTreeView()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		DefaultStyleResourceUri(Uri(L"ms-appx:///CommunityToolkit.WinUI.Controls/Themes/Generic.xaml"));
	}

	void HeaderedTreeView::OnFooterChanged([[maybe_unused]] IInspectable const& oldValue, [[maybe_unused]] IInspectable const& newValue) {}

	void HeaderedTreeView::OnHeaderChanged([[maybe_unused]] IInspectable const& oldValue, [[maybe_unused]] IInspectable const& newValue) {}

	void HeaderedTreeView::OnFooterChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = winrt::get_self<HeaderedTreeView>(d.as<class_type>())->get_strong();
		control->OnFooterChanged(e.OldValue(), e.NewValue());
	}

	void HeaderedTreeView::OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = winrt::get_self<HeaderedTreeView>(d.as<class_type>())->get_strong();
		control->OnHeaderChanged(e.OldValue(), e.NewValue());
	}
}
