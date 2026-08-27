#include "pch.h"
#include "winrt_module_imports.h"
#include "ViewportBehavior.h"
#include "../Helper.h"
#if __has_include("ViewportBehavior.g.cpp")
#include "ViewportBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ViewportBehavior::IsInViewportProperty =
        winrt::DependencyProperty::Register(
            L"IsInViewport",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(false), &ViewportBehavior::OnIsInViewportChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> ViewportBehavior::IsFullyInViewportProperty =
        winrt::DependencyProperty::Register(
            L"IsFullyInViewport",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(false), &ViewportBehavior::OnIsFullyInViewportChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> ViewportBehavior::IsAlwaysOnProperty =
        winrt::DependencyProperty::Register(
            L"IsAlwaysOn",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(false)));

    void ViewportBehavior::OnAttached()
    {
        BehaviorBase::OnAttached();

        auto parent = winrt::VisualTreeHelper::GetParent(AssociatedObject());
        if (!parent)
        {
            _loadedRevoker = AssociatedObject().Loaded(winrt::auto_revoke, { this, &ViewportBehavior::OnAssociatedObjectLoadedHandler });
            return;
        }

        Init();
    }

    void ViewportBehavior::OnDetaching()
    {
        BehaviorBase::OnDetaching();

        _viewChangedRevoker.revoke();
        _hostScrollViewer = nullptr;
    }

    void ViewportBehavior::Init()
    {
        _hostScrollViewer = FindAscendant<winrt::ScrollViewer>(AssociatedObject());
        if (!_hostScrollViewer)
        {
            throw winrt::hresult_error(E_FAIL, L"This behavior can only be attached to an element which has a ScrollViewer as a parent.");
        }

        _viewChangedRevoker = _hostScrollViewer.ViewChanged(winrt::auto_revoke, { this, &ViewportBehavior::OnViewChanged });
    }

    void ViewportBehavior::OnAssociatedObjectLoadedHandler(
        [[maybe_unused]] winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        _loadedRevoker.revoke();
        Init();
    }

    void ViewportBehavior::OnViewChanged(
        [[maybe_unused]] winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::ScrollViewerViewChangedEventArgs const& e)
    {
        if (!_hostScrollViewer) return;

        auto fe = AssociatedObject();
        if (!fe) return;

        auto elementRect =
            fe.TransformToVisual(_hostScrollViewer)
            .TransformBounds(winrt::Rect
            {
                0.0f,
                0.0f,
                static_cast<float>(fe.ActualWidth()),
                static_cast<float>(fe.ActualHeight())
            });

        winrt::Rect viewport
        {
            0.0f,
            0.0f,
            static_cast<float>(_hostScrollViewer.ActualWidth()),
            static_cast<float>(_hostScrollViewer.ActualHeight())
        };

        bool intersects =
            !(elementRect.X > viewport.X + viewport.Width ||
                elementRect.X + elementRect.Width < viewport.X ||
                elementRect.Y > viewport.Y + viewport.Height ||
                elementRect.Y + elementRect.Height < viewport.Y);

        IsInViewport(intersects);

        if (intersects)
        {
            bool fully =
                elementRect.X >= viewport.X &&
                elementRect.Y >= viewport.Y &&
                elementRect.X + elementRect.Width <= viewport.X + viewport.Width &&
                elementRect.Y + elementRect.Height <= viewport.Y + viewport.Height;

            IsFullyInViewport(fully);
        }
        else
        {
            IsFullyInViewport(false);
        }
    }

    void ViewportBehavior::OnIsInViewportChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto behavior = d.try_as<winrt::XamlToolkit::WinUI::Behaviors::ViewportBehavior>())
        {
            auto impl = winrt::get_self<ViewportBehavior>(behavior);
            bool value = winrt::unbox_value<bool>(args.NewValue());

            if (value)
            {
                impl->EnteringViewport.invoke(behavior, nullptr);
            }
            else
            {
                impl->ExitedViewport.invoke(behavior, nullptr);
            }
        }
    }

    void ViewportBehavior::OnIsFullyInViewportChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto behavior = d.try_as<winrt::XamlToolkit::WinUI::Behaviors::ViewportBehavior>())
        {
            auto impl = winrt::get_self<ViewportBehavior>(behavior);
            bool value = winrt::unbox_value<bool>(args.NewValue());

            if (value)
            {
                impl->EnteredViewport.invoke(behavior, nullptr);

                if (!impl->IsAlwaysOn())
                {
                    // Remove behavior from the associated object
                    auto behaviors = winrt::XamlToolkit::WinUI::Interactivity::Interaction::GetBehaviors(impl->AssociatedObject());
                    
                    if (uint32_t index; behaviors.IndexOf(behavior, index)) {
                        behaviors.RemoveAt(index);
                    }
                }
            }
            else
            {
                impl->ExitingViewport.invoke(behavior, nullptr);
            }
        }
    }
}
