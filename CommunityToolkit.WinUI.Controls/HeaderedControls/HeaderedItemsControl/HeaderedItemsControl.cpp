#include "pch.h"
#include "HeaderedItemsControl.h"
#if __has_include("HeaderedItemsControl.g.cpp")
#include "HeaderedItemsControl.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    HeaderedItemsControl::HeaderedItemsControl()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

	void HeaderedItemsControl::OnFooterChanged([[maybe_unused]] IInspectable const& oldValue, [[maybe_unused]] IInspectable const& newValue) {}

	void HeaderedItemsControl::OnHeaderChanged([[maybe_unused]] IInspectable const& oldValue, [[maybe_unused]] IInspectable const& newValue) {}

	void HeaderedItemsControl::OnFooterChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = winrt::get_self<HeaderedItemsControl>(d.as<class_type>())->get_strong();
		control->OnFooterChanged(e.OldValue(), e.NewValue());
	}

	void HeaderedItemsControl::OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto control = winrt::get_self<HeaderedItemsControl>(d.as<class_type>())->get_strong();
		control->OnHeaderChanged(e.OldValue(), e.NewValue());
	}
}
