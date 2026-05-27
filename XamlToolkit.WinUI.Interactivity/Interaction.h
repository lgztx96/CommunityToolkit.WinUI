#pragma once

#include "Interaction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// Defines a <see cref="BehaviorCollection"/> attached property and provides a method for executing an <seealso cref="ActionCollection"/>.
    /// </summary>
    struct Interaction
    {
        Interaction() = delete;

        /// <summary>
        /// Gets or sets the <see cref="BehaviorCollection"/> associated with a specified object.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> BehaviorsProperty;

        /// <summary>
        /// Gets the <see cref="BehaviorCollection"/> associated with a specified object.
        /// </summary>
        /// <param name="obj">The <see cref="DependencyObject"/> from which to retrieve the <see cref="BehaviorCollection"/>.</param>
        /// <returns>A <see cref="BehaviorCollection"/> containing the behaviors associated with the specified object.</returns>
        static winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection GetBehaviors(winrt::DependencyObject const& obj);

        /// <summary>
        /// Sets the <see cref="BehaviorCollection"/> associated with a specified object.
        /// </summary>
        /// <param name="obj">The <see cref="DependencyObject"/> on which to set the <see cref="BehaviorCollection"/>.</param>
        /// <param name="value">The <see cref="BehaviorCollection"/> associated with the object.</param>
        static void SetBehaviors(
            winrt::DependencyObject const& obj,
            winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection const& value);

        /// <summary>
        /// Executes all actions in the <see cref="ActionCollection"/> and returns their results.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> which will be passed on to the action.</param>
        /// <param name="actions">The set of actions to execute.</param>
        /// <param name="parameter">The value of this parameter is determined by the calling behavior.</param>
        /// <returns>Returns the results of the actions.</returns>
        static winrt::IIterable<winrt::IInspectable> ExecuteActions(
            winrt::IInspectable const& sender,
            winrt::XamlToolkit::WinUI::Interactivity::ActionCollection const& actions,
            winrt::IInspectable const& parameter);

    private:
        static void OnBehaviorsChanged(
            winrt::DependencyObject const& sender,
            winrt::DependencyPropertyChangedEventArgs const& args);

        static void FrameworkElement_Loaded(
            winrt::IInspectable const& sender,
            winrt::RoutedEventArgs const& e);

        static void FrameworkElement_Unloaded(
            winrt::IInspectable const& sender,
            winrt::RoutedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct Interaction : InteractionT<Interaction, implementation::Interaction>
    {
    };
}
