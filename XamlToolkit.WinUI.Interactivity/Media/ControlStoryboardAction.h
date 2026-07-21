#pragma once

#include "ControlStoryboardAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media::Animation;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// An action that will change the state of the specified <seealso cref="Storyboard"/> when executed.
    /// </summary>
    struct ControlStoryboardAction : ControlStoryboardActionT<ControlStoryboardAction>
    {
        ControlStoryboardAction() = default;

        /// <summary>
        /// Identifies the <seealso cref="ControlStoryboardOption"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ControlStoryboardOptionProperty;
        /// <summary>
        /// Identifies the <seealso cref="Storyboard"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> StoryboardProperty;

        /// <summary>
        /// Gets or sets the action to execute on the <see cref="Storyboard"/>. This is a dependency property.
        /// </summary>
        winrt::XamlToolkit::WinUI::Interactivity::ControlStoryboardOption ControlStoryboardOption() const;
        void ControlStoryboardOption(winrt::XamlToolkit::WinUI::Interactivity::ControlStoryboardOption const& value);

        /// <summary>
        /// Gets or sets the targeted <see cref="Storyboard"/>. This is a dependency property.
        /// </summary>
        winrt::Storyboard Storyboard() const;
        void Storyboard(winrt::Storyboard const& value);

        /// <summary>
        /// Executes the action.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> that is passed to the action by the behavior. Generally this is <seealso cref="IBehavior.AssociatedObject"/> or a target object.</param>
        /// <param name="parameter">The value of this parameter is determined by the caller.</param>
        /// <returns>True if the specified operation is invoked successfully; else false.</returns>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter);

    private:
        static void OnStoryboardChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        bool _isPaused{ false };
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct ControlStoryboardAction : ControlStoryboardActionT<ControlStoryboardAction, implementation::ControlStoryboardAction>
    {
    };
}
