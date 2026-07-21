#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsExpander.h"
#if __has_include("SettingsExpander.g.cpp")
#include "SettingsExpander.g.cpp"
#endif
#include "SettingsExpanderAutomationPeer.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::HeaderProperty =
        winrt::DependencyProperty::Register(
            L"Header",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::DescriptionProperty =
        winrt::DependencyProperty::Register(
            L"Description",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::HeaderIconProperty =
        winrt::DependencyProperty::Register(
            L"HeaderIcon",
            winrt::xaml_typename<winrt::IconElement>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::ContentProperty =
        winrt::DependencyProperty::Register(
            L"Content",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::ItemsHeaderProperty =
        winrt::DependencyProperty::Register(
            L"ItemsHeader",
            winrt::xaml_typename<winrt::UIElement>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::ItemsFooterProperty =
        winrt::DependencyProperty::Register(
            L"ItemsFooter",
            winrt::xaml_typename<winrt::UIElement>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::IsExpandedProperty =
        winrt::DependencyProperty::Register(
            L"IsExpanded",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(winrt::box_value(false), [](auto& d, auto& e)
            {
                auto self = winrt::get_self<SettingsExpander>(d.template as<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>())->get_strong();
                self->OnIsExpandedPropertyChanged(
                    winrt::unbox_value<bool>(e.OldValue()), winrt::unbox_value<bool>(e.NewValue()));
            }));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::ItemsProperty =
        winrt::DependencyProperty::Register(
            L"Items",
            winrt::xaml_typename<winrt::IVector<winrt::IInspectable>>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr, &SettingsExpander::OnItemsConnectedPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::ItemsSourceProperty =
        winrt::DependencyProperty::Register(
            L"ItemsSource",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr, &SettingsExpander::OnItemsConnectedPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::ItemTemplateProperty =
        winrt::DependencyProperty::Register(
            L"ItemTemplate",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> SettingsExpander::ItemContainerStyleSelectorProperty =
        winrt::DependencyProperty::Register(
            L"ItemContainerStyleSelector",
            winrt::xaml_typename<winrt::StyleSelector>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsExpander>(),
            winrt::PropertyMetadata(nullptr));

    SettingsExpander::SettingsExpander() : _itemsRepeater{ nullptr }
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        Items(winrt::single_threaded_vector<winrt::IInspectable>());
    }

    void SettingsExpander::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();
        SetAccessibleName();

        if (_itemsRepeater)
        {
            _elementPreparedRevoker.revoke();
        }

        _itemsRepeater = GetTemplateChild(PART_ItemsRepeater).try_as<winrt::ItemsRepeater>();

        if (_itemsRepeater)
        {
            _elementPreparedRevoker = _itemsRepeater.ElementPrepared(winrt::auto_revoke, { get_weak(), &SettingsExpander::ItemsRepeater_ElementPrepared });

            // Update it's source based on our current items properties.
            OnItemsConnectedPropertyChanged(*this, nullptr); // Can't get it to accept type here? (DependencyPropertyChangedEventArgs)EventArgs.Empty
        }
    }

    void SettingsExpander::SetAccessibleName()
    {
        if (auto name = winrt::AutomationProperties::GetName(*this); name.empty())
        {
            if (auto headerString = Header().try_as<winrt::hstring>(); headerString && !headerString->empty())
            {
                winrt::AutomationProperties::SetName(*this, headerString.value());
            }
        }
    }

    winrt::AutomationPeer SettingsExpander::OnCreateAutomationPeer()
    {
        return winrt::make<winrt::XamlToolkit::WinUI::Controls::implementation::SettingsExpanderAutomationPeer>(*this);
    }

    void SettingsExpander::OnIsExpandedChanged([[maybe_unused]] bool oldValue, bool newValue)
    {
        if (auto peer = winrt::FrameworkElementAutomationPeer::FromElement(*this).try_as<winrt::XamlToolkit::WinUI::Controls::SettingsExpanderAutomationPeer>())
        {
            peer.RaiseExpandedChangedEvent(newValue);
        }
    }

    void SettingsExpander::OnItemsConnectedPropertyChanged(winrt::DependencyObject const& dependencyObject, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto expander = dependencyObject.try_as<class_type>(); expander)
        {
            auto impl = winrt::get_self<SettingsExpander>(expander)->get_strong();
            if (impl->_itemsRepeater) 
            {
                auto datasource = expander.ItemsSource();

                if (datasource == nullptr)
                {
                    datasource = expander.Items();
                }

                impl->_itemsRepeater.ItemsSource(datasource);
            }
        }
    }

    void SettingsExpander::OnIsExpandedPropertyChanged(bool oldValue, bool newValue)
    {
        OnIsExpandedChanged(oldValue, newValue);

        if (newValue)
        {
            Expanded.invoke(*this, nullptr);
        }
        else
        {
            Collapsed.invoke(*this, nullptr);
        }
    }

    void SettingsExpander::ItemsRepeater_ElementPrepared([[maybe_unused]] winrt::ItemsRepeater const& sender, winrt::ItemsRepeaterElementPreparedEventArgs const& args)
    {
        if (auto styleSelector = ItemContainerStyleSelector()) 
        {
            auto element = args.Element().try_as<winrt::FrameworkElement>();
            if (element && element.ReadLocalValue(winrt::FrameworkElement::StyleProperty()) == winrt::DependencyProperty::UnsetValue()) {
                // TODO: Get item from args.Index?
                element.Style(styleSelector.SelectStyle(nullptr, element));
            }
        }
    }
}
