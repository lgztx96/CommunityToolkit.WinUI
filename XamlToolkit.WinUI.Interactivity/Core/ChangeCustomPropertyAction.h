#pragma once

#include "ChangeCustomPropertyAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Data;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// An action that will change a specified property (via PropertyPath/ICustomProperty) to a specified value when invoked.
    /// </summary>
    struct ChangeCustomPropertyAction : ChangeCustomPropertyActionT<ChangeCustomPropertyAction>
    {
        ChangeCustomPropertyAction() = default;

        /// <summary>
        /// Identifies the <seealso cref="PropertyName"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> PropertyNameProperty;
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
        winrt::PropertyPath PropertyName() const;
        void PropertyName(winrt::PropertyPath const& value);

        /// <summary>
        /// Gets or sets the value to set. This is a dependency property.
        /// </summary>
        winrt::IInspectable Value() const;
        void Value(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the object whose property will be changed.
        /// If <seealso cref="TargetObject"/> is not set or cannot be resolved, the sender of <seealso cref="Execute"/> will be used. This is a dependency property.
        /// </summary>
        winrt::IInspectable TargetObject() const;
        void TargetObject(winrt::IInspectable const& value);

        /// <summary>
        /// Executes the action.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> that is passed to the action by the behavior. Generally this is <seealso cref="IBehavior.AssociatedObject"/> or a target object.</param>
        /// <param name="parameter">The value of this parameter is determined by the caller.</param>
        /// <returns>True if updating the property value succeeds; else false.</returns>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter);

    private:
        void UpdatePropertyValue(winrt::IInspectable const& targetObject, winrt::hstring const& propertyPath);
        void ValidateProperty(winrt::hstring const& targetTypeName, winrt::ICustomProperty const& property, winrt::hstring const& propertyPath);
        winrt::IInspectable GetConvertedValue(winrt::ICustomProperty const& property) const;
        static winrt::IInspectable GetDefaultValue(winrt::Windows::UI::Xaml::Interop::TypeName const& typeName);
        static winrt::hstring InspectableToString(winrt::IInspectable const& value);
        static winrt::hstring TypeNameToString(winrt::Windows::UI::Xaml::Interop::TypeName const& typeName);
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct ChangeCustomPropertyAction : ChangeCustomPropertyActionT<ChangeCustomPropertyAction, implementation::ChangeCustomPropertyAction>
    {
    };
}