#include "pch.h"
#include "winrt_module_imports.h"
#include "Ribbon.h"
#if __has_include("Ribbon.g.cpp")
#include "Ribbon.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Ribbon::ScrollStepProperty =
        winrt::DependencyProperty::Register(
            L"ScrollStep",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::Ribbon>(),
            winrt::PropertyMetadata{ winrt::box_value(20.0) });

    const wil::single_threaded_property<winrt::DependencyProperty> Ribbon::OptionsFlyoutProperty =
        winrt::DependencyProperty::Register(
            L"OptionsFlyout",
            winrt::xaml_typename<winrt::FlyoutBase>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::Ribbon>(),
            winrt::PropertyMetadata{ nullptr, &Ribbon::OnOptionsFlyoutPropertyChanged });

    const wil::single_threaded_property<winrt::DependencyProperty> Ribbon::OptionsAccessibleNameProperty =
        winrt::DependencyProperty::Register(
            L"OptionsAccessibleName",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::Ribbon>(),
            winrt::PropertyMetadata{ winrt::box_value(L"") });

    const wil::single_threaded_property<winrt::DependencyProperty> Ribbon::OptionsAccessKeyProperty =
        winrt::DependencyProperty::Register(
            L"OptionsAccessKey",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::Ribbon>(),
            winrt::PropertyMetadata{ winrt::box_value(L"") });

    Ribbon::Ribbon()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        _items = winrt::single_threaded_observable_vector<winrt::UIElement>();
        _items.VectorChanged({ this, &Ribbon::OnItemsVectorChanged });
    }

    void Ribbon::OnApplyTemplate()
    {
        _panelSizeChangedRevoker.revoke();
        _decrementButtonClickRevoker.revoke();
        _incrementButtonClickRevoker.revoke();
        _scrollViewerViewChangedRevoker.revoke();
        _scrollViewerSizeChangedRevoker.revoke();

        _panel = GetTemplateChild(PanelTemplatePart).try_as<winrt::Panel>();
        if (_panel)
        {
            auto children = _panel.Children();
            for (const auto& item : _items)
            {
                children.Append(item);
            }

            _panelSizeChangedRevoker = _panel.SizeChanged(winrt::auto_revoke, { this, &Ribbon::OnSizeChanged });
        }

        _decrementButton = GetTemplateChild(ScrollDecrementButtonTempatePart).try_as<winrt::ButtonBase>();
        if (_decrementButton)
        {
            _decrementButtonClickRevoker = _decrementButton.Click(winrt::auto_revoke, { this, &Ribbon::OnDecrementScrollViewer });
        }

        _incrementButton = GetTemplateChild(ScrollIncrementButtonTempatePart).try_as<winrt::ButtonBase>();
        if (_incrementButton)
        {
            _incrementButtonClickRevoker = _incrementButton.Click(winrt::auto_revoke, { this, &Ribbon::OnIncrementScrollViewer });
        }

        _scrollViewer = GetTemplateChild(ScrollViewerTemplatePart).try_as<winrt::ScrollViewer>();
        if (_scrollViewer)
        {
            _scrollViewerViewChangedRevoker = _scrollViewer.ViewChanged(winrt::auto_revoke, { this, &Ribbon::OnViewChanged });
            _scrollViewerSizeChangedRevoker = _scrollViewer.SizeChanged(winrt::auto_revoke, { this, &Ribbon::OnSizeChanged });
            UpdateScrollButtonsState();
        }

        UpdateOptionsFlyoutState();
    }

    void Ribbon::OnItemsVectorChanged(
        winrt::IObservableVector<winrt::UIElement> const& sender,
        winrt::IVectorChangedEventArgs const& args)
    {
        if (_panel == nullptr)
            return;

        auto children = _panel.Children();
        auto change = args.CollectionChange();
        uint32_t index = args.Index();

        switch (change)
        {
        case winrt::CollectionChange::ItemInserted:
        {
            auto element = sender.GetAt(index);
            if (!element)
                throw winrt::hresult_invalid_argument(L"Item must not be null");
            children.InsertAt(index, element);
        }
        break;

        case winrt::CollectionChange::ItemRemoved:
        {
            children.RemoveAt(index);
        }
        break;

        case winrt::CollectionChange::ItemChanged:
        {
            auto element = sender.GetAt(index);
            if (!element)
                throw winrt::hresult_invalid_argument(L"Item must not be null");
            children.SetAt(index, element);
        }
        break;

        case winrt::CollectionChange::Reset:
        {
            children.Clear();
            for (auto const& item : sender)
            {
                children.Append(item);
            }
        }
        break;

        default:
            throw winrt::hresult_invalid_argument(L"Invalid CollectionChange value");
        }
    }

    void Ribbon::OnViewChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ScrollViewerViewChangedEventArgs const& e)
    {
        UpdateScrollButtonsState();
    }

    void Ribbon::OnSizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
    {
        UpdateScrollButtonsState();
    }

    void Ribbon::UpdateScrollButtonsState()
    {
        if (_scrollViewer == nullptr)
        {
            return;
        }

        if (_scrollViewer.ExtentWidth() <= _scrollViewer.ViewportWidth())
        {
            winrt::VisualStateManager::GoToState(*this, NoButtonsStateTemplatePart, true);
            return;
        }

        auto showDecrement = _scrollViewer.HorizontalOffset() >= 1;
        auto showIncrement = _scrollViewer.ExtentWidth() - _scrollViewer.HorizontalOffset() - _scrollViewer.ViewportWidth() >= 1;
        if (showDecrement && showIncrement)
        {
            winrt::VisualStateManager::GoToState(*this, BothButtonsStateTemplatePart, true);
        }
        else if (showDecrement)
        {
            winrt::VisualStateManager::GoToState(*this, DecrementButtonStateTemplatePart, true);
        }
        else if (showIncrement)
        {
            winrt::VisualStateManager::GoToState(*this, IncrementButtonStateTemplatePart, true);
        }
        else
        {
            winrt::VisualStateManager::GoToState(*this, NoButtonsStateTemplatePart, true);
        }
    }

    void Ribbon::OnDecrementScrollViewer([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (_scrollViewer) _scrollViewer.ChangeView(_scrollViewer.HorizontalOffset() - ScrollStep(), nullptr, nullptr);
    }

    void Ribbon::OnIncrementScrollViewer([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (_scrollViewer) _scrollViewer.ChangeView(_scrollViewer.HorizontalOffset() + ScrollStep(), nullptr, nullptr);
    }

    void Ribbon::OnOptionsFlyoutPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto ribbon = d.as<winrt::XamlToolkit::Labs::WinUI::Ribbon>();
        auto self = winrt::get_self<Ribbon>(ribbon);
        self->UpdateOptionsFlyoutState();
    }

    void Ribbon::UpdateOptionsFlyoutState()
    {
        winrt::VisualStateManager::GoToState(
            *this,
            OptionsFlyout() ? OptionsVisibleStateTemplatePart : OptionsCollapsedStateTemplatePart,
            true);
    }
}
