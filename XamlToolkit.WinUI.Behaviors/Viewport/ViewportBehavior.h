#pragma once

#include "ViewportBehavior.g.h"
#include "../BehaviorBase.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// A class for listening to an element enter or exit the ScrollViewer viewport
    /// </summary>
    struct ViewportBehavior : ViewportBehaviorT<ViewportBehavior>, BehaviorBase<ViewportBehavior, winrt::FrameworkElement>
    {
        ViewportBehavior() = default;

        /// <summary>
        /// Gets a value indicating whether associated element is in the ScrollViewer viewport
        /// </summary>
        bool IsInViewport() const
        {
            return winrt::unbox_value<bool>(GetValue(IsInViewportProperty()));
        }
        void IsInViewport(bool value)
        {
            SetValue(IsInViewportProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Gets a value indicating whether associated element is fully in the ScrollViewer viewport
        /// </summary>
        bool IsFullyInViewport() const
        {
            return winrt::unbox_value<bool>(GetValue(IsFullyInViewportProperty()));
        }
        void IsFullyInViewport(bool value)
        {
            SetValue(IsFullyInViewportProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets a value indicating whether this behavior will remain attached after the associated element enters the viewport. When false, the behavior will remove itself after entering.
        /// </summary>
        bool IsAlwaysOn() const
        {
            return winrt::unbox_value<bool>(GetValue(IsAlwaysOnProperty()));
        }
        void IsAlwaysOn(bool value)
        {
            SetValue(IsAlwaysOnProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> IsInViewportProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> IsFullyInViewportProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> IsAlwaysOnProperty;

        /// <summary>
        /// Associated element fully enter the ScrollViewer viewport event
        /// </summary>
        wil::untyped_event<winrt::IInspectable> EnteredViewport;

        /// <summary>
        /// Associated element fully exit the ScrollViewer viewport event
        /// </summary>
        wil::untyped_event<winrt::IInspectable> ExitedViewport;

        /// <summary>
        /// Associated element enter the ScrollViewer viewport event
        /// </summary>
        wil::untyped_event<winrt::IInspectable> EnteringViewport;

        /// <summary>
        /// Associated element exit the ScrollViewer viewport event
        /// </summary>
        wil::untyped_event<winrt::IInspectable> ExitingViewport;

    protected:
        /// <summary>
        /// Called after the behavior is attached to the AssociatedObject.
        /// </summary>
        /// <remarks>
        /// Override this to hook up functionality to the AssociatedObject
        /// </remarks>
        void OnAttached() override;

        /// <summary>
        /// Called when the behavior is being detached from its AssociatedObject.
        /// </summary>
        /// <remarks>
        /// Override this to unhook functionality from the AssociatedObject
        /// </remarks>
        void OnDetaching() override;

    private:
        /// <summary>
        /// Event tracking the state of the object as it moves into and out of the viewport
        /// </summary>
        /// <param name="d">DependencyObject</param>
        /// <param name="args">EventArgs</param>
        static void OnIsInViewportChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& args);

        /// <summary>
        /// Event tracking when the object is fully within the viewport or not
        /// </summary>
        /// <param name="d">DependencyObject</param>
        /// <param name="args">EventArgs</param>
        static void OnIsFullyInViewportChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& args);

        void OnViewChanged(winrt::IInspectable const& sender, winrt::ScrollViewerViewChangedEventArgs const& e);
        void Init();
        void OnAssociatedObjectLoadedHandler(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        winrt::ScrollViewer _hostScrollViewer{ nullptr };

        winrt::ScrollViewer::ViewChanged_revoker _viewChangedRevoker;
        winrt::FrameworkElement::Loaded_revoker _loadedRevoker;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct ViewportBehavior : ViewportBehaviorT<ViewportBehavior, implementation::ViewportBehavior>
    {
    };
}
