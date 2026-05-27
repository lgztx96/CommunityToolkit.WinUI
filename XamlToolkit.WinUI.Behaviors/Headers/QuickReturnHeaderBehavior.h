#pragma once

#include "QuickReturnHeaderBehavior.g.h"
#include "HeaderBehaviorBase.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// Performs an animation on a ListView or GridView Header to make it quick return using composition.
    /// </summary>
    struct QuickReturnHeaderBehavior : QuickReturnHeaderBehaviorT<QuickReturnHeaderBehavior>, HeaderBehaviorBase<QuickReturnHeaderBehavior>
    {
        QuickReturnHeaderBehavior() = default;

        /// <summary>
        /// Show the header
        /// </summary>
        void Show();

    protected:
        /// <summary>
        /// Uses Composition API to get the UIElement and sets an ExpressionAnimation.
        /// </summary>
        /// <returns>true if the assignment was successful; otherwise, false.</returns>
        bool AssignAnimation() override;

        /// <summary>
        /// Stops the animation.
        /// </summary>
        void StopAnimation() override;

        /// <summary>
        /// Remove the animation from the UIElement.
        /// </summary>
        void RemoveAnimation() override;

    private:
        void OnViewChanged(winrt::IInspectable const& sender, winrt::ScrollViewerViewChangedEventArgs const& e);
        double _headerPosition{ 0.0 };

        winrt::ScrollViewer::ViewChanged_revoker _viewChangedRevoker;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct QuickReturnHeaderBehavior : QuickReturnHeaderBehaviorT<QuickReturnHeaderBehavior, implementation::QuickReturnHeaderBehavior>
    {
    };
}
