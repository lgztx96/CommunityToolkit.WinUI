#pragma once

#include "Ribbon.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct Ribbon : RibbonT<Ribbon>
    {
        static constexpr std::wstring_view PanelTemplatePart = L"Panel";
        static constexpr std::wstring_view ScrollViewerTemplatePart = L"ScrollViewer";
        static constexpr std::wstring_view ScrollDecrementButtonTempatePart = L"ScrollDecrementButton";
        static constexpr std::wstring_view ScrollIncrementButtonTempatePart = L"ScrollIncrementButton";
        static constexpr std::wstring_view ScrollButtonGroupNameTemplatePart = L"ScrollButtonGroup";
        static constexpr std::wstring_view NoButtonsStateTemplatePart = L"NoButtons";
        static constexpr std::wstring_view DecrementButtonStateTemplatePart = L"DecrementButton";
        static constexpr std::wstring_view IncrementButtonStateTemplatePart = L"IncrementButton";
        static constexpr std::wstring_view BothButtonsStateTemplatePart = L"BothButtons";
        static constexpr std::wstring_view OptionsGroupNameTemplatePart = L"OptionsGroup";
        static constexpr std::wstring_view OptionsVisibleStateTemplatePart = L"OptionsVisible";
        static constexpr std::wstring_view OptionsCollapsedStateTemplatePart = L"OptionsCollapsed";

        Ribbon();

        void OnApplyTemplate();

        winrt::IVector<winrt::UIElement> Items() const { return _items; }

        static const wil::single_threaded_property<winrt::DependencyProperty> ScrollStepProperty;

        double ScrollStep() const { return winrt::unbox_value<double>(GetValue(ScrollStepProperty)); }

        void ScrollStep(double value) { SetValue(ScrollStepProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> OptionsFlyoutProperty;

        winrt::FlyoutBase OptionsFlyout() const { return GetValue(OptionsFlyoutProperty).try_as<winrt::FlyoutBase>(); }

        void OptionsFlyout(winrt::FlyoutBase const& value) { SetValue(OptionsFlyoutProperty, value); }

        static const wil::single_threaded_property<winrt::DependencyProperty> OptionsAccessibleNameProperty;

        winrt::hstring OptionsAccessibleName() const { return winrt::unbox_value<winrt::hstring>(GetValue(OptionsAccessibleNameProperty)); }

        void OptionsAccessibleName(winrt::hstring const& value) { SetValue(OptionsAccessibleNameProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> OptionsAccessKeyProperty;

        winrt::hstring OptionsAccessKey() const { return winrt::unbox_value<winrt::hstring>(GetValue(OptionsAccessKeyProperty)); }

        void OptionsAccessKey(winrt::hstring const& value) { SetValue(OptionsAccessKeyProperty, winrt::box_value(value)); }

    private:
        static void OnOptionsFlyoutPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        void OnItemsVectorChanged(winrt::IObservableVector<winrt::UIElement> const& sender, winrt::IVectorChangedEventArgs const& args);

        void OnViewChanged(winrt::IInspectable const& sender, winrt::ScrollViewerViewChangedEventArgs const& e);

        void OnSizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

        void UpdateScrollButtonsState();

        void OnDecrementScrollViewer(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        void OnIncrementScrollViewer(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        void UpdateOptionsFlyoutState();

        winrt::Panel _panel{ nullptr };
        winrt::ScrollViewer _scrollViewer{ nullptr };
        winrt::ButtonBase _decrementButton{ nullptr };
        winrt::ButtonBase _incrementButton{ nullptr };
        winrt::IObservableVector<winrt::UIElement> _items{ nullptr };

        winrt::Panel::SizeChanged_revoker _panelSizeChangedRevoker;
        winrt::ButtonBase::Click_revoker _incrementButtonClickRevoker;
        winrt::ButtonBase::Click_revoker _decrementButtonClickRevoker;
        winrt::ScrollViewer::ViewChanged_revoker _scrollViewerViewChangedRevoker;
        winrt::ScrollViewer::SizeChanged_revoker _scrollViewerSizeChangedRevoker;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct Ribbon : RibbonT<Ribbon, implementation::Ribbon>
    {
    };
}
