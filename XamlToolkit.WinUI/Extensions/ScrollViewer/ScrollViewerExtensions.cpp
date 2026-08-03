#include "pch.h"
#include "winrt_module_imports.h"
#include "ScrollViewerExtensions.h"
#if __has_include("ScrollViewerExtensions.g.cpp")
#include "ScrollViewerExtensions.g.cpp"
#endif

#include "../../common.h"

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ScrollViewerExtensions::HorizontalScrollBarMarginProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"HorizontalScrollBarMargin",
            winrt::xaml_typename<winrt::Thickness>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::ScrollViewerExtensions>(),
            PropertyMetadata(nullptr, &ScrollViewerExtensions::OnHorizontalScrollBarMarginPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> ScrollViewerExtensions::VerticalScrollBarMarginProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"VerticalScrollBarMargin",
            winrt::xaml_typename<winrt::Thickness>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::ScrollViewerExtensions>(),
            PropertyMetadata(nullptr, &ScrollViewerExtensions::OnVerticalScrollBarMarginPropertyChanged));

    void ScrollViewerExtensions::OnHorizontalScrollBarMarginPropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto baseElement = sender.try_as<winrt::FrameworkElement>())
        {
            // If it didn't work it means that we need to wait for the component to be loaded before getting its ScrollViewer
            if (ChangeHorizontalScrollBarMarginProperty(baseElement))
            {
                return;
            }

            // We need to wait for the component to be loaded before getting its ScrollViewer
            //baseElement.Loaded -= ChangeHorizontalScrollBarMarginProperty;

            if (!baseElement.IsLoaded())
            {
                auto token = std::make_shared<winrt::event_token>();
                *token = baseElement.Loaded([token](auto&& sender, auto&&)
                {
                    auto element = sender.template as<winrt::FrameworkElement>();
                    ChangeHorizontalScrollBarMarginProperty(element);
                    element.Loaded(*token);
                });
            }
        }
    }

    bool ScrollViewerExtensions::ChangeHorizontalScrollBarMarginProperty(winrt::FrameworkElement const& sender)
    {
        auto scrollViewer = sender.try_as<winrt::ScrollViewer>();

        if (!scrollViewer)
        {
            scrollViewer = DependencyObjectEx::FindDescendant<winrt::ScrollViewer>(sender);
        }

        if (!scrollViewer)
        {
            return false;
        }

        // Last scrollbar with "HorizontalScrollBar" as name is our target to set its margin and avoid it overlapping the header
        winrt::ScrollBar scrollBar = nullptr;

        for (const auto& descendant : DependencyObjectEx::FindDescendants(scrollViewer))
        {
            if (auto bar = descendant.try_as<winrt::ScrollBar>())
            {
                if (bar.Name() == L"HorizontalScrollBar")
                {
                    scrollBar = bar;
                }
            }
        }

        if (!scrollBar)
        {
            return false;
        }

        auto newMargin = GetHorizontalScrollBarMargin(sender);
        scrollBar.Margin(newMargin);

        return true;
    }

    void ScrollViewerExtensions::OnVerticalScrollBarMarginPropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto baseElement = sender.try_as<winrt::FrameworkElement>())
        {
            // We try to update the value, if it works we may exit
            if (ChangeVerticalScrollBarMarginProperty(baseElement))
            {
                return;
            }

            // If it didn't work it means that we need to wait for the component to be loaded before getting its ScrollViewer
            // baseElement.Loaded -= ChangeVerticalScrollBarMarginProperty;

            if (!baseElement.IsLoaded())
            {
                auto token = std::make_shared<winrt::event_token>();
                *token = baseElement.Loaded([token](auto&& sender, auto&&)
                {
                    auto element = sender.template as<winrt::FrameworkElement>();
                    ChangeVerticalScrollBarMarginProperty(element);
                    element.Loaded(*token);
                });
            }
        }
    }

    bool ScrollViewerExtensions::ChangeVerticalScrollBarMarginProperty(winrt::FrameworkElement const& sender)
    {
        auto scrollViewer = sender.try_as<winrt::ScrollViewer>();

        if (!scrollViewer)
        {
            scrollViewer = DependencyObjectEx::FindDescendant<winrt::ScrollViewer>(sender);
        }

        if (!scrollViewer)
        {
            return false;
        }

        // Last scrollbar with "VerticalScrollBar" as name is our target to set its margin and avoid it overlapping the header
        winrt::ScrollBar scrollBar = nullptr;

        for (const auto& descendant : DependencyObjectEx::FindDescendants(scrollViewer))
        {
            if (auto bar = descendant.try_as<winrt::ScrollBar>())
            {
                if (bar.Name() == L"VerticalScrollBar")
                {
                    scrollBar = bar;
                }
            }
        }

        if (!scrollBar)
        {
            return false;
        }

        auto newMargin = GetVerticalScrollBarMargin(sender);
        scrollBar.Margin(newMargin);

        return true;
    }
}
