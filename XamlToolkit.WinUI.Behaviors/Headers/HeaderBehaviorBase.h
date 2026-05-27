#pragma once

#include "../BehaviorBase.h"
#include "../Helper.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Hosting;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors
{
    // From Doc: https://learn.microsoft.com/windows/windows-app-sdk/api/winrt/microsoft.ui.xaml.controls.canvas.zindex
    constexpr int CanvasZIndexMax = 1'000'000;

    /// <summary>
    /// Base class helper for header behaviors which manipulate an element within a viewport of a ListViewBase based control.
    /// </summary>
    template <typename D>
    struct HeaderBehaviorBase : BehaviorBase<D, winrt::FrameworkElement>
    {
    protected:
        /// <summary>
        /// The ScrollViewer associated with the ListViewBase control.
        /// </summary>
        winrt::ScrollViewer _scrollViewer{ nullptr };

        /// <summary>
        /// The CompositionPropertySet associated with the ScrollViewer.
        /// </summary>
        winrt::CompositionPropertySet _scrollProperties{ nullptr };

        /// <summary>
        /// The CompositionPropertySet associated with the animation.
        /// </summary>
        winrt::CompositionPropertySet _animationProperties{ nullptr };

        /// <summary>
        /// The Visual associated with the header element.
        /// </summary>
        winrt::Visual _headerVisual{ nullptr };

        /// <summary>
        /// Attaches the behavior to the associated object.
        /// </summary>
        /// <returns>true if attaching succeeded; otherwise false.</returns>
        virtual bool Initialize() override
        {
            return AssignAnimation();
        }

        /// <summary>
        /// Detaches the behavior from the associated object.
        /// </summary>
        /// <returns>true if detaching succeeded; otherwise false.</returns>
        virtual bool Uninitialize() override
        {
            RemoveAnimation();
            return true;
        }

        /// <summary>
        /// Uses Composition API to get the UIElement and sets an ExpressionAnimation.
        /// </summary>
        /// <remarks>
        /// If this method returns true, you should have access to all protected fields with assigned components to use.
        /// </remarks>
        /// <returns>true if the assignment was successful; otherwise, false.</returns>
        virtual bool AssignAnimation()
        {
            StopAnimation();

            // Double-check that we have an element associated with us (we should) and that it has size
            if (!static_cast<D*>(this)->AssociatedObject() || static_cast<D*>(this)->AssociatedObject().RenderSize().Height == 0)
            {
                return false;
            }

            if (!_scrollViewer)
            {
                // TODO: We probably want checks which provide better guidance if we detect we're not attached correctly?
                _scrollViewer = FindAscendant<winrt::ScrollViewer>(static_cast<D*>(this)->AssociatedObject());
            }

            if (!_scrollViewer)
            {
                return false;
            }

            auto itemsControl = FindAscendant<winrt::ItemsControl>(static_cast<D*>(this)->AssociatedObject());
            if (itemsControl && itemsControl.ItemsPanelRoot())
            {
                // This appears to be important to force the items within the ScrollViewer of an ItemsControl behind our header element.
                winrt::Canvas::SetZIndex(itemsControl.ItemsPanelRoot(), -1);
            }
            else
            {
                // If we're not part of a collection panel, then we're probably just in the ScrollViewer,
                // And we should ensure our 'header' element is on top of any other content within the ScrollViewer.
                winrt::Canvas::SetZIndex(static_cast<D*>(this)->AssociatedObject(), CanvasZIndexMax);
            }

            if (!_scrollProperties)
            {
                _scrollProperties = winrt::ElementCompositionPreview::GetScrollViewerManipulationPropertySet(_scrollViewer);
            }

            if (!_scrollProperties)
            {
                return false;
            }

            if (!_headerVisual)
            {
                _headerVisual = winrt::ElementCompositionPreview::GetElementVisual(static_cast<D*>(this)->AssociatedObject());
            }

            if (!_headerVisual)
            {
                return false;
            }

            // TODO: Not sure if we need to provide an option to turn these events off, as FadeHeaderBehavior didn't use these two, unlike QuickReturn/Sticky did...
            _sizeChangedRevoker = static_cast<D*>(this)->AssociatedObject().SizeChanged(winrt::auto_revoke, { this, &HeaderBehaviorBase::OnScrollHeaderSizeChanged });
            _gotFocusRevoker = _scrollViewer.GotFocus(winrt::auto_revoke, { this, &HeaderBehaviorBase::OnScrollViewerGotFocus });

            if (!_animationProperties)
            {
                _animationProperties = _scrollProperties.Compositor().CreatePropertySet();
            }

            return true;
        }

        /// <summary>
        /// Stop the animation of the UIElement.
        /// </summary>
        virtual void StopAnimation() = 0;

        /// <summary>
        /// Remove the animation from the UIElement.
        /// </summary>
        virtual void RemoveAnimation()
        {
            _sizeChangedRevoker.revoke();
            _gotFocusRevoker.revoke();

            StopAnimation();
        }

    private:
        void OnScrollHeaderSizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
        {
            AssignAnimation();
        }

        void OnScrollViewerGotFocus(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
        {
            auto scroller = sender.try_as<winrt::ScrollViewer>();
            if (!scroller || !static_cast<D*>(this)->AssociatedObject()) return;

            auto focusedElement = winrt::FocusManager::GetFocusedElement(scroller.XamlRoot());
            if (!focusedElement) return;

            auto element = focusedElement.try_as<winrt::UIElement>();
            // To prevent Popups (Flyouts...) from triggering the autoscroll, we check if the focused element has a valid parent.
            // Popups have no parents, whereas a normal Item would have the ListView as a parent.
            if (!element || !winrt::VisualTreeHelper::GetParent(element))
            {
                return;
            }

            auto point = element.TransformToVisual(scroller).TransformPoint(winrt::Point{ 0, 0 });
            if (point.Y < static_cast<D*>(this)->AssociatedObject().ActualHeight())
            {
                scroller.ChangeView(nullptr, scroller.VerticalOffset() - (static_cast<D*>(this)->AssociatedObject().ActualHeight() - point.Y), nullptr, false);
            }
        }

        winrt::FrameworkElement::SizeChanged_revoker _sizeChangedRevoker;
        winrt::ScrollViewer::GotFocus_revoker _gotFocusRevoker;
    };
}
