#pragma once

#include "RibbonCollapsibleGroup.g.h"
#include "RibbonGroup.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::System;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct RibbonCollapsibleGroup : RibbonCollapsibleGroupT<RibbonCollapsibleGroup, implementation::RibbonGroup>
    {
        static constexpr std::wstring_view VisibleContentContainerTemplatePart = L"VisibleContentContainer";
        static constexpr std::wstring_view CollapsedButtonTemplatePart = L"CollapsedButton";
        static constexpr std::wstring_view CollapsedFlyoutTemplatePart = L"CollapsedFlyout";
        static constexpr std::wstring_view CollapsedContentPresenterTemplatePart = L"CollapsedContentPresenter";

        static const wil::single_threaded_property<winrt::DependencyProperty> IconSourceProperty;

        winrt::IconSource IconSource() const
        {
            return GetValue(IconSourceProperty).try_as<winrt::IconSource>();
        }

        void IconSource(winrt::IconSource const& value)
        {
            SetValue(IconSourceProperty, value);
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> StateProperty;

        winrt::Visibility State() const
        {
            return winrt::unbox_value<winrt::Visibility>(GetValue(StateProperty));
        }

        void State(winrt::Visibility const& value)
        {
            SetValue(StateProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> AutoCloseFlyoutProperty;

        bool AutoCloseFlyout() const
        {
            return winrt::unbox_value<bool>(GetValue(AutoCloseFlyoutProperty));
        }

        void AutoCloseFlyout(bool value)
        {
            SetValue(AutoCloseFlyoutProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> PriorityProperty;

        int Priority() const
        {
            return winrt::unbox_value<int>(GetValue(PriorityProperty));
        }

        void Priority(int value)
        {
            SetValue(PriorityProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> CollapsedAccessKeyProperty;

        winrt::hstring CollapsedAccessKey() const
        {
            return winrt::unbox_value<winrt::hstring>(GetValue(CollapsedAccessKeyProperty));
        }

        void CollapsedAccessKey(winrt::hstring const& value)
        {
            SetValue(CollapsedAccessKeyProperty, winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> RequestedWidthsProperty;

        winrt::XamlToolkit::Labs::WinUI::DoubleVector RequestedWidths() const
        {
            return GetValue(RequestedWidthsProperty).try_as<winrt::XamlToolkit::Labs::WinUI::DoubleVector>();
        }

        void RequestedWidths(winrt::XamlToolkit::Labs::WinUI::DoubleVector const& value)
        {
            SetValue(RequestedWidthsProperty, value);
        }

        RibbonCollapsibleGroup();

        void OnApplyTemplate();

    private:
        static void OnStatePropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const&);

        static void OnRequestedWidthsChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

        template<typename T>
        T Get(std::wstring_view templatePart) { return GetTemplateChild(templatePart).try_as<T>(); }

        void OnFlyoutOpened(winrt::IInspectable const& sender, winrt::IInspectable const& e);

        void OnFlyoutPointerReleased(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void OnFlyoutKeyUp(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

        void AutoCollapseFlyout(bool eventHasBeenHandled, winrt::IInspectable const& originalSource);

        bool DoesRoutedEventOriginateFromAFlyoutHost(winrt::UIElement source);

        void UpdateState();

        winrt::ContentControl _visibleContentContainer{ nullptr };
        winrt::ContentControl _collapsedContentContainer{ nullptr };
        winrt::Button _collapsedButton{ nullptr };
        winrt::Flyout _collapsedFlyout{ nullptr };

        winrt::IInspectable _contaionerPointerEventHandler{ nullptr };
        winrt::IInspectable _contaionerKeyEventHandler{ nullptr };

        winrt::Flyout::Opened_revoker _flyoutOpenedRevoker;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct RibbonCollapsibleGroup : RibbonCollapsibleGroupT<RibbonCollapsibleGroup, implementation::RibbonCollapsibleGroup>
    {
    };
}
