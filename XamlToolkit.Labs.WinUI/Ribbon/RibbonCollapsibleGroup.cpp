#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "RibbonCollapsibleGroup.h"
#if __has_include("RibbonCollapsibleGroup.g.cpp")
#include "RibbonCollapsibleGroup.g.cpp"
#endif
#include "DoubleVector.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> RibbonCollapsibleGroup::IconSourceProperty =
        winrt::DependencyProperty::Register(
            L"IconSource",
            winrt::xaml_typename<winrt::IconSource>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>(),
            winrt::PropertyMetadata{ nullptr });

    const wil::single_threaded_property<winrt::DependencyProperty> RibbonCollapsibleGroup::StateProperty =
        winrt::DependencyProperty::Register(
            L"State",
            winrt::xaml_typename<winrt::Visibility>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>(),
            winrt::PropertyMetadata{ winrt::box_value(winrt::Visibility::Visible), &RibbonCollapsibleGroup::OnStatePropertyChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> RibbonCollapsibleGroup::AutoCloseFlyoutProperty =
        winrt::DependencyProperty::Register(
            L"AutoCloseFlyout",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>(),
            winrt::PropertyMetadata{ winrt::box_value(true) });

    const wil::single_threaded_property<winrt::DependencyProperty> RibbonCollapsibleGroup::PriorityProperty =
        winrt::DependencyProperty::Register(
            L"Priority",
            winrt::xaml_typename<int>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>(),
            winrt::PropertyMetadata{ winrt::box_value(0) });

    const wil::single_threaded_property<winrt::DependencyProperty> RibbonCollapsibleGroup::CollapsedAccessKeyProperty =
        winrt::DependencyProperty::Register(
            L"CollapsedAccessKey",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>(),
            winrt::PropertyMetadata{ winrt::box_value(L"") });

    const wil::single_threaded_property<winrt::DependencyProperty> RibbonCollapsibleGroup::RequestedWidthsProperty =
        winrt::DependencyProperty::Register(
            L"RequestedWidths",
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::DoubleVector>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::RibbonCollapsibleGroup>(),
            winrt::PropertyMetadata{ nullptr, &RibbonCollapsibleGroup::OnRequestedWidthsChanged });

    RibbonCollapsibleGroup::RibbonCollapsibleGroup()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        _contaionerPointerEventHandler = winrt::box_value(winrt::PointerEventHandler({ this, &RibbonCollapsibleGroup::OnFlyoutPointerReleased }));
        _contaionerKeyEventHandler = winrt::box_value(winrt::KeyEventHandler({ this, &RibbonCollapsibleGroup::OnFlyoutKeyUp }));
    }

    void RibbonCollapsibleGroup::OnApplyTemplate()
    {
        if (_collapsedFlyout)
        {
            _flyoutOpenedRevoker.revoke();
        }

        if (_collapsedContentContainer)
        {
            _collapsedContentContainer.RemoveHandler(winrt::UIElement::PointerReleasedEvent(), _contaionerPointerEventHandler);
            _collapsedContentContainer.RemoveHandler(winrt::UIElement::KeyUpEvent(), _contaionerKeyEventHandler);
        }

        _visibleContentContainer = Get<winrt::ContentControl>(VisibleContentContainerTemplatePart);
        _collapsedContentContainer = Get<winrt::ContentControl>(CollapsedContentPresenterTemplatePart);
        _collapsedButton = Get<winrt::Button>(CollapsedButtonTemplatePart);
        _collapsedFlyout = Get<winrt::Flyout>(CollapsedFlyoutTemplatePart);

        if (_collapsedFlyout)
        {
            _flyoutOpenedRevoker = _collapsedFlyout.Opened(winrt::auto_revoke, { this, &RibbonCollapsibleGroup::OnFlyoutOpened });
        }

        if (_collapsedContentContainer)
        {
            _collapsedContentContainer.AddHandler(winrt::UIElement::PointerReleasedEvent(), _contaionerPointerEventHandler, true);
            _collapsedContentContainer.AddHandler(winrt::UIElement::KeyUpEvent(), _contaionerKeyEventHandler, true);
        }

        UpdateState();
    }

    void RibbonCollapsibleGroup::OnRequestedWidthsChanged([[maybe_unused]] winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto newValue = e.NewValue().try_as<winrt::XamlToolkit::Labs::WinUI::DoubleVector>())
        {
            auto vector = winrt::get_self<DoubleVector>(newValue)->get_strong();
            std::ranges::sort(vector->get_container());
        }
    }

    void RibbonCollapsibleGroup::OnFlyoutOpened([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& e)
    {
        if (_collapsedContentContainer) _collapsedContentContainer.Focus(winrt::FocusState::Programmatic);
    }

    void RibbonCollapsibleGroup::OnFlyoutPointerReleased([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        AutoCollapseFlyout(e.Handled(), e.OriginalSource());
    }

    void RibbonCollapsibleGroup::OnFlyoutKeyUp([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
    {
        if (e.Key() != winrt::VirtualKey::Enter && e.Key() != winrt::VirtualKey::Space)
        {
            return;
        }

        AutoCollapseFlyout(e.Handled(), e.OriginalSource());
    }

    void RibbonCollapsibleGroup::AutoCollapseFlyout(bool eventHasBeenHandled, winrt::IInspectable const& originalSource)
    {
        // We only consider events which have been processed since it usually means
        // that a control has processed the event and that the click is not in an
        // empty/non-interactive area.
        if (eventHasBeenHandled &&
            AutoCloseFlyout() &&
            _collapsedFlyout &&
            _collapsedFlyout.IsOpen() &&
            !DoesRoutedEventOriginateFromAFlyoutHost(originalSource.try_as<winrt::UIElement>()))
        {
            _collapsedFlyout.Hide();
        }
    }

    bool RibbonCollapsibleGroup::DoesRoutedEventOriginateFromAFlyoutHost(winrt::UIElement source)
    {
        if (_collapsedContentContainer == nullptr)
        {
            return false;
        }

        while (source && source != _collapsedContentContainer)
        {
            if (source.try_as<winrt::DropDownButton>() ||
                source.try_as<winrt::ComboBox>() ||
                source.try_as<winrt::ComboBoxItem>())
            {
                return true;
            }

            if (auto buttonSource = source.try_as<winrt::Button>(); buttonSource && buttonSource.Flyout())
            {
                return true;
            }

            if (auto frameworkSource = source.try_as<winrt::FrameworkElement>())
            {
                if (winrt::FlyoutBase::GetAttachedFlyout(frameworkSource))
                {
                    return true;
                }
            }

            source = winrt::VisualTreeHelper::GetParent(source).try_as<winrt::UIElement>();
        }

        return false;
    }

    void RibbonCollapsibleGroup::OnStatePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto group = winrt::get_self<implementation::RibbonCollapsibleGroup>(d.as<class_type>());
        group->UpdateState();
    }

    void RibbonCollapsibleGroup::UpdateState()
    {
        switch (State())
        {
        case winrt::Visibility::Visible:
            if (_collapsedFlyout)
            {
                _collapsedFlyout.Hide();
            }

            if (_collapsedContentContainer && _visibleContentContainer)
            {
                _collapsedContentContainer.Content(nullptr);
                _visibleContentContainer.Content(Content());
            }

            if (_collapsedButton && _visibleContentContainer)
            {
                _collapsedButton.Visibility(winrt::Visibility::Collapsed);
                _visibleContentContainer.Visibility(winrt::Visibility::Visible);
            }
            break;

        case winrt::Visibility::Collapsed:
            if (_collapsedContentContainer && _visibleContentContainer)
            {
                _visibleContentContainer.Content(nullptr);
                _collapsedContentContainer.Content(Content());
            }

            if (_collapsedButton && _visibleContentContainer)
            {
                _visibleContentContainer.Visibility(winrt::Visibility::Collapsed);
                _collapsedButton.Visibility(winrt::Visibility::Visible);
            }
            break;

        default:
            throw winrt::hresult_invalid_argument(L"Invalid state");
        }
    }
}
