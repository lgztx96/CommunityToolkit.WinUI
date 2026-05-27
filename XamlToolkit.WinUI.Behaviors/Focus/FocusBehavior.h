#pragma once

#include "FocusBehavior.g.h"
#include "../BehaviorBase.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <vector>
#include <utility>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Dispatching;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    /// <summary>
    /// This behavior sets the focus on the first control of Targets which accepts it.
    /// The focus will be set following the Targets order. The first control being ready
    /// and accepting the focus will receive it.
    /// The focus can be set to another control with a higher priority if it loads before FocusEngagementTimeout.
    /// The focus can be set to another control if some controls will be loaded/unloaded later.
    /// </summary>
    struct FocusBehavior : FocusBehaviorT<FocusBehavior>, BehaviorBase<FocusBehavior, winrt::UIElement>
    {
        FocusBehavior();

        /// <summary>
        /// Gets or sets the ordered list of controls which should receive the focus when the associated object is loaded.
        /// </summary>
        winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList Targets();
        void Targets(winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList const& value);

        /// <summary>
        /// Gets or sets the timeout before the FocusBehavior stops trying to set the focus to a control with
        /// a higher priority.
        /// </summary>
        winrt::TimeSpan FocusEngagementTimeout() const
        {
            return winrt::unbox_value<winrt::TimeSpan>(GetValue(FocusEngagementTimeoutProperty()));
        }
        void FocusEngagementTimeout(winrt::TimeSpan const& value)
        {
            SetValue(FocusEngagementTimeoutProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> TargetsProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> FocusEngagementTimeoutProperty;

    protected:
        /// <summary>
        /// Called when the associated object has been loaded.
        /// </summary>
        void OnAssociatedObjectLoaded() override;

        /// <summary>
        /// Uninitializes the behavior from the associated object.
        /// </summary>
        /// <returns>true if uninitialization succeeded; otherwise false.</returns>
        bool Uninitialize() override;

    private:
        static void OnTargetsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& args);

        void ApplyFocus();
        void Stop(winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList const& targets);
        void OnControlLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);
        void OnTargetControlChanged(winrt::IInspectable const& sender, winrt::IInspectable const& e);
        void OnContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);
        void OnEngagementTimerTick(winrt::IInspectable const& sender, winrt::IInspectable const& e);

        winrt::DispatcherQueueTimer _timer{ nullptr };

        std::vector<std::pair<winrt::weak_ref<winrt::XamlToolkit::WinUI::Behaviors::FocusTarget>, winrt::event_token>> _controlChangedTokens;
        std::vector<std::pair<winrt::weak_ref<winrt::Control>, winrt::event_token>> _controlLoadedTokens;
        std::vector<std::pair<winrt::weak_ref<winrt::ListViewBase>, winrt::event_token>> _containerContentChangingTokens;
    };
}

namespace winrt::XamlToolkit::WinUI::Behaviors::factory_implementation
{
    struct FocusBehavior : FocusBehaviorT<FocusBehavior, implementation::FocusBehavior>
    {
    };
}
