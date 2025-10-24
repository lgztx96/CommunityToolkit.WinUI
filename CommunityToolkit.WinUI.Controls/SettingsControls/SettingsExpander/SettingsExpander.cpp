#include "pch.h"
#include "SettingsExpander.h"
#if __has_include("SettingsExpander.g.cpp")
#include "SettingsExpander.g.cpp"
#endif
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include "SettingsExpanderAutomationPeer.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace Microsoft::UI::Xaml::Input;

	SettingsExpander::SettingsExpander() : _itemsRepeater{ nullptr }, _elementPreparedToken{ 0 }
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		Items(winrt::single_threaded_vector<IInspectable>());
	}

	void SettingsExpander::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		SetAccessibleName();

		if (_itemsRepeater != nullptr)
		{
			if (_elementPreparedToken) _itemsRepeater.ElementPrepared(_elementPreparedToken);
		}

		_itemsRepeater = GetTemplateChild(PART_ItemsRepeater).try_as<ItemsRepeater>();

		if (_itemsRepeater != nullptr)
		{
			_elementPreparedToken = _itemsRepeater.ElementPrepared({ this, &SettingsExpander::ItemsRepeater_ElementPrepared });

			// Update it's source based on our current items properties.
			OnItemsConnectedPropertyChanged(*this, nullptr); // Can't get it to accept type here? (DependencyPropertyChangedEventArgs)EventArgs.Empty
		}
	}

	void SettingsExpander::SetAccessibleName()
	{
		if (auto name = AutomationProperties::GetName(*this); name.empty())
		{
			if (auto headerString = Header().try_as<hstring>(); headerString && !headerString->empty())
			{
				AutomationProperties::SetName(*this, headerString.value());
			}
		}
	}

	AutomationPeer SettingsExpander::OnCreateAutomationPeer()
	{
		return winrt::make<winrt::CommunityToolkit::WinUI::Controls::implementation::SettingsExpanderAutomationPeer>(*this);
	}

	void SettingsExpander::OnIsExpandedChanged([[maybe_unused]] bool oldValue, bool newValue)
	{
		if (auto peer = FrameworkElementAutomationPeer::FromElement(*this).try_as<winrt::CommunityToolkit::WinUI::Controls::SettingsExpanderAutomationPeer>())
		{
			peer.RaiseExpandedChangedEvent(newValue);
		}
	}

	void SettingsExpander::OnItemsConnectedPropertyChanged(DependencyObject const& dependencyObject, [[maybe_unused]] DependencyPropertyChangedEventArgs const& args)
	{
		if (auto expander = dependencyObject.try_as<class_type>(); expander)
		{
			auto impl = winrt::get_self<SettingsExpander>(expander)->get_strong();
			if (impl->_itemsRepeater != nullptr) {
				auto datasource = expander.ItemsSource();

				if (datasource == nullptr)
				{
					datasource = expander.Items();
				}

				impl->_itemsRepeater.ItemsSource(datasource);
			}
		}
	}

	void SettingsExpander::ItemsRepeater_ElementPrepared([[maybe_unused]] ItemsRepeater const& sender, ItemsRepeaterElementPreparedEventArgs const& args)
	{
		if (ItemContainerStyleSelector() != nullptr) {
			auto element = args.Element().try_as<FrameworkElement>();
			if (element && element.ReadLocalValue(FrameworkElement::StyleProperty()) == DependencyProperty::UnsetValue()) {
				// TODO: Get item from args.Index?
				element.Style(ItemContainerStyleSelector().SelectStyle(nullptr, element));
			}
		}
	}
}
