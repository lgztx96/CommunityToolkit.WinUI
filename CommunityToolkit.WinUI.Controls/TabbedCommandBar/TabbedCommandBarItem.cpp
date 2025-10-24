#include "pch.h"
#include "TabbedCommandBarItem.h"
#if __has_include("TabbedCommandBarItem.g.cpp")
#include "TabbedCommandBarItem.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	TabbedCommandBarItem::TabbedCommandBarItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		DefaultStyleResourceUri(Uri(L"ms-appx:///CommunityToolkit.WinUI.Controls/Themes/Generic.xaml"));
	}

	void TabbedCommandBarItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		_primaryItemsControl = GetTemplateChild(L"PrimaryItemsControl").try_as<ItemsControl>();
		if (_primaryItemsControl)
		{
			_primaryItemsControl.HorizontalAlignment(CommandAlignment());
			RegisterPropertyChangedCallback(CommandAlignmentProperty, [](auto const& sender, auto const& dp)
				{
					if (auto item = sender.template try_as<class_type>())
					{
						auto self = winrt::get_self<TabbedCommandBarItem>(item)->get_strong();
						self->_primaryItemsControl.HorizontalAlignment(winrt::unbox_value<enum HorizontalAlignment>(sender.GetValue(dp)));
					}
				});
		}

		_moreButton = GetTemplateChild(L"MoreButton").try_as<Button>();
		if (_moreButton)
		{
			_moreButton.HorizontalAlignment(OverflowButtonAlignment());
			RegisterPropertyChangedCallback(OverflowButtonAlignmentProperty, [](auto const& sender, auto const& dp)
				{
					if (auto item = sender.template try_as<class_type>())
					{
						auto self = winrt::get_self<TabbedCommandBarItem>(item)->get_strong();
						self->_moreButton.HorizontalAlignment(winrt::unbox_value<enum HorizontalAlignment>(sender.GetValue(dp)));
					}
				});
		}
	}
}
