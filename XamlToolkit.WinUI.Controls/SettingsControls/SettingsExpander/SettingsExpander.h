#pragma once

#include "SettingsExpander.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct SettingsExpander : SettingsExpanderT<SettingsExpander>
    {
        static constexpr auto PART_ItemsRepeater = L"PART_ItemsRepeater";
        winrt::ItemsRepeater::ElementPrepared_revoker _elementPreparedRevoker;
        winrt::ItemsRepeater _itemsRepeater;

        wil::untyped_event<winrt::Windows::Foundation::IInspectable> Expanded;

        wil::untyped_event<winrt::Windows::Foundation::IInspectable> Collapsed;

        SettingsExpander();

        void OnApplyTemplate();

        void SetAccessibleName();

        winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer OnCreateAutomationPeer();

        void OnIsExpandedChanged(bool oldValue, bool newValue);

        void OnIsExpandedPropertyChanged(bool oldValue, bool newValue);

        static const wil::single_threaded_property<winrt::DependencyProperty> HeaderProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> DescriptionProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> HeaderIconProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ContentProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ItemsHeaderProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ItemsFooterProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> IsExpandedProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ItemsProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ItemsSourceProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ItemTemplateProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ItemContainerStyleSelectorProperty;

        winrt::IInspectable Header() { return GetValue(HeaderProperty); }
        void Header(winrt::IInspectable const& value) { SetValue(HeaderProperty, value); }

        winrt::IInspectable Description() { return GetValue(DescriptionProperty); }
        void Description(winrt::IInspectable const& value) { SetValue(DescriptionProperty, value); }

        winrt::IconElement HeaderIcon() { return winrt::unbox_value<winrt::IconElement>(GetValue(HeaderIconProperty)); }
        void HeaderIcon(winrt::IconElement const& value) { SetValue(HeaderIconProperty, value); }

        winrt::IInspectable Content() { return winrt::unbox_value<winrt::IInspectable>(GetValue(ContentProperty)); }
        void Content(winrt::IInspectable const& value) { SetValue(ContentProperty, value); }

        winrt::UIElement ItemsHeader() { return winrt::unbox_value<winrt::UIElement>(GetValue(ItemsHeaderProperty)); }
        void ItemsHeader(winrt::UIElement const& value) { SetValue(ItemsHeaderProperty, value); }

        winrt::UIElement ItemsFooter() { return winrt::unbox_value<winrt::UIElement>(GetValue(ItemsFooterProperty)); }
        void ItemsFooter(winrt::UIElement const& value) { SetValue(ItemsFooterProperty, value); }

        bool IsExpanded() { return winrt::unbox_value<bool>(GetValue(IsExpandedProperty)); }
        void IsExpanded(bool value) { SetValue(IsExpandedProperty, winrt::box_value(value)); }

        static void OnItemsConnectedPropertyChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args);

        winrt::IVector<winrt::IInspectable> Items() { return winrt::unbox_value<winrt::IVector<winrt::IInspectable>>(GetValue(ItemsProperty)); }
        void Items(winrt::IVector<winrt::IInspectable> const& value) { SetValue(ItemsProperty, value); }

        winrt::IInspectable ItemsSource() { return GetValue(ItemsSourceProperty); }
        void ItemsSource(winrt::IInspectable const& value) { SetValue(ItemsSourceProperty, value); }

        winrt::IInspectable ItemTemplate() { return GetValue(ItemTemplateProperty); }
        void ItemTemplate(winrt::IInspectable const& value) { SetValue(ItemTemplateProperty, value); }

        winrt::StyleSelector ItemContainerStyleSelector() { return winrt::unbox_value<winrt::StyleSelector>(GetValue(ItemContainerStyleSelectorProperty)); }
        void ItemContainerStyleSelector(winrt::StyleSelector const& value) { SetValue(ItemContainerStyleSelectorProperty, value); }

        void ItemsRepeater_ElementPrepared(winrt::ItemsRepeater const& sender, winrt::ItemsRepeaterElementPreparedEventArgs const& args);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SettingsExpander : SettingsExpanderT<SettingsExpander, implementation::SettingsExpander>
    {
    };
}
