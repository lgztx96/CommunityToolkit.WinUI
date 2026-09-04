#pragma once

#include "DataTriggerBehavior.g.h"
#include "../Trigger.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
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
    using namespace Windows::UI::Xaml::Interop;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// A behavior that performs actions when the bound data meets a specified condition.
    /// </summary>
    struct DataTriggerBehavior : DataTriggerBehaviorT<DataTriggerBehavior, winrt::XamlToolkit::WinUI::Interactivity::implementation::Trigger>
    {
        DataTriggerBehavior() = default;

        /// <summary>
        /// Identifies the <seealso cref="Binding"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> BindingProperty;
        /// <summary>
        /// Identifies the <seealso cref="ComparisonCondition"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ComparisonConditionProperty;
        /// <summary>
        /// Identifies the <seealso cref="Value"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> ValueProperty;

        /// <summary>
        /// Gets or sets the bound object that the <see cref="DataTriggerBehavior"/> will listen to. This is a dependency property.
        /// </summary>
        winrt::IInspectable Binding() const;
        void Binding(winrt::IInspectable const& value);

        /// <summary>
        /// Gets or sets the type of comparison to be performed between <see cref="Binding"/> and <see cref="Value"/>. This is a dependency property.
        /// </summary>
        ComparisonConditionType ComparisonCondition();
        void ComparisonCondition(ComparisonConditionType const& value);

        /// <summary>
        /// Gets or sets the value to be compared with the value of <see cref="Binding"/>. This is a dependency property.
        /// </summary>
        winrt::IInspectable Value() const;
        void Value(winrt::IInspectable const& value);

    private:
        static bool Compare(
            winrt::IInspectable const& leftOperand,
            ComparisonConditionType const& operatorType,
            winrt::IInspectable const& rightOperand);

        static bool EvaluateComparable(
            winrt::IInspectable const& leftOperand,
            ComparisonConditionType const& operatorType,
            winrt::IInspectable const& rightOperand);

        static int CompareComparable(winrt::IInspectable const& leftOperand, winrt::IInspectable const& rightOperand);
        static winrt::IInspectable ConvertRightOperand(winrt::IInspectable const& leftOperand, winrt::IInspectable const& rightOperand);
        static winrt::TypeName InferTypeName(winrt::IInspectable const& value);
        static winrt::hstring TypeDisplayName(winrt::IInspectable const& value);
        static winrt::hstring TypeDisplayName(winrt::TypeName const& typeName);
        static winrt::hstring ValueToString(winrt::IInspectable const& value);
        static void OnValueChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args);
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct DataTriggerBehavior : DataTriggerBehaviorT<DataTriggerBehavior, implementation::DataTriggerBehavior>
    {
    };
}
