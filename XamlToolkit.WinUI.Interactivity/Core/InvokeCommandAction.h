#pragma once

#include "InvokeCommandAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Data;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// Executes a specified <see cref="ICommand"/> when invoked.
    /// </summary>
    struct InvokeCommandAction : InvokeCommandActionT<InvokeCommandAction>
    {
        InvokeCommandAction() = default;

        /// <summary>
        /// Identifies the <seealso cref="Command"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> CommandProperty;
        /// <summary>
        /// Identifies the <seealso cref="CommandParameter"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> CommandParameterProperty;
        /// <summary>
        /// Identifies the <seealso cref="InputConverter"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> InputConverterProperty;
        /// <summary>
        /// Identifies the <seealso cref="InputConverterParameter"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> InputConverterParameterProperty;
        /// <summary>
        /// Identifies the <seealso cref="InputConverterLanguage"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> InputConverterLanguageProperty;

        /// <summary>
        /// Gets or sets the command this action should invoke. This is a dependency property.
        /// </summary>
        winrt::ICommand Command() const;
        void Command(winrt::ICommand const& value);

        /// <summary>
        /// Gets or sets the parameter that is passed to <see cref="ICommand.Execute"/>.
        /// If this is not set, the parameter from the <seealso cref="Execute"/> method will be used.
        /// This is an optional dependency property.
        /// </summary>
        winrt::IInspectable CommandParameter() const;
        void CommandParameter(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the converter that is run on the parameter from the <seealso cref="Execute"/> method.
        /// This is an optional dependency property.
        /// </summary>
        winrt::IValueConverter InputConverter() const;
        void InputConverter(winrt::IValueConverter const& value);

        /// <summary>
        /// Gets or sets the parameter that is passed to the <see cref="IValueConverter.Convert"/>
        /// method of <see cref="InputConverter"/>.
        /// This is an optional dependency property.
        /// </summary>
        winrt::IInspectable InputConverterParameter() const;
        void InputConverterParameter(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the language that is passed to the <see cref="IValueConverter.Convert"/>
        /// method of <see cref="InputConverter"/>.
        /// This is an optional dependency property.
        /// </summary>
        winrt::hstring InputConverterLanguage() const;
        void InputConverterLanguage(winrt::hstring const& value);

        /// <summary>
        /// Executes the action.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> that is passed to the action by the behavior. Generally this is <seealso cref="IBehavior.AssociatedObject"/> or a target object.</param>
        /// <param name="parameter">The value of this parameter is determined by the caller.</param>
        /// <returns>True if the command is successfully executed; else false.</returns>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter);
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct InvokeCommandAction : InvokeCommandActionT<InvokeCommandAction, implementation::InvokeCommandAction>
    {
    };
}
