#include "pch.h"
#include "winrt_module_imports.h"
#include "TabbedCommandBarItem.h"
#if __has_include("TabbedCommandBarItem.g.cpp")
#include "TabbedCommandBarItem.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	TabbedCommandBarItem::TabbedCommandBarItem()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		DefaultStyleResourceUri(winrt::Uri(L"ms-appx:///XamlToolkit.WinUI.Controls/Themes/Generic.xaml"));
	}

	void TabbedCommandBarItem::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		if (_commandAlignmentChangedToken)
		{
			UnregisterPropertyChangedCallback(CommandAlignmentProperty(), _commandAlignmentChangedToken);
			_commandAlignmentChangedToken = 0;
		}

		_primaryItemsControl = GetTemplateChild(PrimaryItemsControlPartName).try_as<winrt::ItemsControl>();
		if (_primaryItemsControl)
		{
			_primaryItemsControl.HorizontalAlignment(CommandAlignment());

			_commandAlignmentChangedToken = RegisterPropertyChangedCallback(CommandAlignmentProperty(), [](const auto& sender, const auto& dp)
			{
				if (auto item = sender.template try_as<class_type>())
				{
					auto self = winrt::get_self<TabbedCommandBarItem>(item)->get_strong();
					self->_primaryItemsControl.HorizontalAlignment(winrt::unbox_value<winrt::HorizontalAlignment>(sender.GetValue(dp)));
				}
			});
		}

		if (_overflowAlignmentChangedToken)
		{
			UnregisterPropertyChangedCallback(OverflowButtonAlignmentProperty(), _overflowAlignmentChangedToken);
			_overflowAlignmentChangedToken = 0;
		}

		_moreButton = GetTemplateChild(MoreButtonPartName).try_as<winrt::Button>();
		if (_moreButton)
		{
			_moreButton.HorizontalAlignment(OverflowButtonAlignment());
			_overflowAlignmentChangedToken = RegisterPropertyChangedCallback(OverflowButtonAlignmentProperty(), [](const auto& sender, const auto& dp)
			{
				if (auto item = sender.template try_as<class_type>())
				{
					auto self = winrt::get_self<TabbedCommandBarItem>(item)->get_strong();
					self->_moreButton.HorizontalAlignment(winrt::unbox_value<winrt::HorizontalAlignment>(sender.GetValue(dp)));
				}
			});
		}
	}
}
