#pragma once

#include "ChangeDependencyPropertyAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Data;
    using namespace Microsoft::UI::Xaml::Markup;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// An action that will change a specified dependency property to a specified value when invoked.
    /// </summary>
    struct ChangeDependencyPropertyAction : ChangeDependencyPropertyActionT<ChangeDependencyPropertyAction>
    {
        ChangeDependencyPropertyAction() = default;

        /// <summary>
        /// Identifies the <seealso cref="TargetObject"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> TargetObjectProperty;

        /// <summary>
        /// Identifies the <seealso cref="Value"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;

        /// <summary>
        /// Gets or sets the name of the property to change. This is a dependency property.
        /// </summary>
        wil::single_threaded_rw_property<winrt::DependencyProperty> Property{ nullptr };

        /// <summary>
        /// Gets or sets the value to set. This is a dependency property.
        /// </summary>
        winrt::IInspectable Value() const;
        void Value(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the object whose property will be changed.
        /// If <seealso cref="TargetObject"/> is not set or cannot be resolved, the sender of <seealso cref="Execute"/> will be used. This is a dependency property.
        /// </summary>
        winrt::DependencyObject TargetObject() const;
        void TargetObject(winrt::DependencyObject const& value);

        /// <summary>
        /// Executes the action.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> that is passed to the action by the behavior. Generally this is <seealso cref="IBehavior.AssociatedObject"/> or a target object.</param>
        /// <param name="parameter">The value of this parameter is determined by the caller.</param>
        /// <returns>True if updating the property value succeeds; else false.</returns>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter);

    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct ChangeDependencyPropertyAction : ChangeDependencyPropertyActionT<ChangeDependencyPropertyAction, implementation::ChangeDependencyPropertyAction>
    {
    };
}
