#pragma once

#include "SettingsExpander.g.h"

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::Foundation::Collections;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;

    template <typename D, typename... I>
    using SettingsExpanderT = SettingsExpander_base<D, I...>;

    struct SettingsExpander : SettingsExpanderT<SettingsExpander>
    {
        static constexpr auto PART_ItemsRepeater = L"PART_ItemsRepeater";
        ItemsRepeater::ElementPrepared_revoker _elementPreparedRevoker;
        ItemsRepeater _itemsRepeater;

        wil::untyped_event<winrt::Windows::Foundation::IInspectable> Expanded;

        wil::untyped_event<winrt::Windows::Foundation::IInspectable> Collapsed;

        SettingsExpander();

        void OnApplyTemplate();

        void SetAccessibleName();

        winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer OnCreateAutomationPeer();

        void OnIsExpandedChanged(bool oldValue, bool newValue);

        static inline const wil::single_threaded_property<DependencyProperty> HeaderProperty = DependencyProperty::Register(
            L"Header",
            winrt::xaml_typename<IInspectable>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> DescriptionProperty = DependencyProperty::Register(
            L"Description",
            winrt::xaml_typename<IInspectable>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> HeaderIconProperty = DependencyProperty::Register(
            L"HeaderIcon",
            winrt::xaml_typename<IconElement>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> ContentProperty = DependencyProperty::Register(
            L"Content",
            winrt::xaml_typename<IInspectable>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> ItemsHeaderProperty = DependencyProperty::Register(
            L"ItemsHeader",
            winrt::xaml_typename<UIElement>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> ItemsFooterProperty = DependencyProperty::Register(
            L"ItemsFooter",
            winrt::xaml_typename<UIElement>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr));

        static inline const wil::single_threaded_property<DependencyProperty> IsExpandedProperty = DependencyProperty::Register(
            L"IsExpanded",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(false), [](auto& d, auto& e)
            { 
                auto self = winrt::get_self<SettingsExpander>(d.template as<class_type>())->get_strong();
                self->OnIsExpandedPropertyChanged(
                    winrt::unbox_value<bool>(e.OldValue()), winrt::unbox_value<bool>(e.NewValue()));
            }));

        IInspectable Header() { return GetValue(HeaderProperty); }
        void Header(IInspectable const& value) { SetValue(HeaderProperty, value); }


        IInspectable Description() { return GetValue(DescriptionProperty); }
        void Description(IInspectable const& value) { SetValue(DescriptionProperty, value); }

        IconElement HeaderIcon() { return winrt::unbox_value<IconElement>(GetValue(HeaderIconProperty)); }
        void HeaderIcon(IconElement const& value) { SetValue(HeaderIconProperty, value); }

        IInspectable Content() { return winrt::unbox_value<IInspectable>(GetValue(ContentProperty)); }
        void Content(IInspectable const& value) { SetValue(ContentProperty, value); }

        UIElement ItemsHeader() { return winrt::unbox_value<UIElement>(GetValue(ItemsHeaderProperty)); }
        void ItemsHeader(UIElement const& value) { SetValue(ItemsHeaderProperty, value); }

        UIElement ItemsFooter() { return winrt::unbox_value<UIElement>(GetValue(ItemsFooterProperty)); }
        void ItemsFooter(UIElement const& value) { SetValue(ItemsFooterProperty, value); }

        bool IsExpanded() { return winrt::unbox_value<bool>(GetValue(IsExpandedProperty)); }
        void IsExpanded(bool value) { SetValue(IsExpandedProperty, winrt::box_value(value)); }

        void OnIsExpandedPropertyChanged(bool oldValue, bool newValue)
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

        static void OnItemsConnectedPropertyChanged(DependencyObject const& dependencyObject, DependencyPropertyChangedEventArgs const& args);

        IVector<IInspectable> Items() { return winrt::unbox_value<IVector<IInspectable>>(GetValue(ItemsProperty)); }
        void Items(IVector<IInspectable> const& value) { SetValue(ItemsProperty, value); }

        static inline const wil::single_threaded_property<DependencyProperty> ItemsProperty =
            DependencyProperty::Register(L"Items", winrt::xaml_typename<IVector<IInspectable>>(), winrt::xaml_typename<class_type>(), 
                PropertyMetadata(nullptr, OnItemsConnectedPropertyChanged));

        IInspectable ItemsSource() { return GetValue(ItemsSourceProperty); }
        void ItemsSource(IInspectable const& value) { SetValue(ItemsSourceProperty, value); }

        static inline const wil::single_threaded_property<DependencyProperty> ItemsSourceProperty =
            DependencyProperty::Register(L"ItemsSource", winrt::xaml_typename<IInspectable>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, OnItemsConnectedPropertyChanged));


        IInspectable ItemTemplate() { return GetValue(ItemTemplateProperty); }
        void ItemTemplate(IInspectable const& value) { SetValue(ItemTemplateProperty, value); }

            static inline const wil::single_threaded_property<DependencyProperty> ItemTemplateProperty =
            DependencyProperty::Register(L"ItemTemplate", winrt::xaml_typename<IInspectable>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

        StyleSelector ItemContainerStyleSelector() { return winrt::unbox_value<StyleSelector>(GetValue(ItemContainerStyleSelectorProperty)); }
        void ItemContainerStyleSelector(StyleSelector const& value) { SetValue(ItemContainerStyleSelectorProperty, value); }

            static inline const wil::single_threaded_property<DependencyProperty> ItemContainerStyleSelectorProperty =
            DependencyProperty::Register(L"ItemContainerStyleSelector", winrt::xaml_typename<StyleSelector>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

        void ItemsRepeater_ElementPrepared(ItemsRepeater const& sender, ItemsRepeaterElementPreparedEventArgs const& args);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct SettingsExpander : SettingsExpanderT<SettingsExpander, implementation::SettingsExpander>
    {
    };
}
