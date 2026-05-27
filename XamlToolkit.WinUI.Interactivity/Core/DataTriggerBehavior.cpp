#include "pch.h"
#include "winrt_module_imports.h"
#include "DataTriggerBehavior.h"
#include "DataBindingHelper.h"
#include "ResourceHelper.h"
#include "TypeConverterHelper.h"
#include "../Interaction.h"
#if __has_include("DataTriggerBehavior.g.cpp")
#include "DataTriggerBehavior.g.cpp"
#endif

namespace
{
    winrt::hstring ComparisonConditionToString(winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType value)
    {
        switch (value)
        {
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::Equal:
            return L"Equal";
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::NotEqual:
            return L"NotEqual";
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThan:
            return L"LessThan";
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThanOrEqual:
            return L"LessThanOrEqual";
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThan:
            return L"GreaterThan";
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThanOrEqual:
            return L"GreaterThanOrEqual";
        }

        return L"Unknown";
    }
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> DataTriggerBehavior::BindingProperty = winrt::DependencyProperty::Register(
        L"Binding",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr, &DataTriggerBehavior::OnValueChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> DataTriggerBehavior::ComparisonConditionProperty = winrt::DependencyProperty::Register(
        L"ComparisonCondition",
        winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::Equal), &DataTriggerBehavior::OnValueChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> DataTriggerBehavior::ValueProperty = winrt::DependencyProperty::Register(
        L"Value",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr, &DataTriggerBehavior::OnValueChanged));
     
    winrt::IInspectable DataTriggerBehavior::Binding() const
    {
        return GetValue(BindingProperty());
    }

    void DataTriggerBehavior::Binding(winrt::IInspectable const& value)
    {
        SetValue(BindingProperty(), value);
    }

    winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType DataTriggerBehavior::ComparisonCondition()
    {
        auto value = GetValue(ComparisonConditionProperty());
        return winrt::unbox_value_or<winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType>(value, winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::Equal);
    }

    void DataTriggerBehavior::ComparisonCondition(winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType const& value)
    {
        SetValue(ComparisonConditionProperty(), winrt::box_value(value));
    }

    winrt::IInspectable DataTriggerBehavior::Value() const
    {
        return GetValue(ValueProperty());
    }

    void DataTriggerBehavior::Value(winrt::IInspectable const& value)
    {
        SetValue(ValueProperty(), value);
    }

    bool DataTriggerBehavior::Compare(
        winrt::IInspectable const& leftOperand,
        winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType const& operatorType,
        winrt::IInspectable const& rightOperand)
    {
        auto convertedRightOperand = rightOperand;
        if (leftOperand != nullptr && rightOperand != nullptr)
        {
            convertedRightOperand = ConvertRightOperand(leftOperand, rightOperand);
        }

        if (leftOperand != nullptr && convertedRightOperand != nullptr)
        {
            return EvaluateComparable(leftOperand, operatorType, convertedRightOperand);
        }

        switch (operatorType)
        {
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::Equal:
            return leftOperand == convertedRightOperand || (leftOperand != nullptr && convertedRightOperand != nullptr && DataTriggerBehavior::ValueToString(leftOperand) == DataTriggerBehavior::ValueToString(convertedRightOperand));

        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::NotEqual:
            return !(leftOperand == convertedRightOperand || (leftOperand != nullptr && convertedRightOperand != nullptr && DataTriggerBehavior::ValueToString(leftOperand) == DataTriggerBehavior::ValueToString(convertedRightOperand)));

        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThan:
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThanOrEqual:
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThan:
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThanOrEqual:
            {
                auto message = ResourceHelper::Format(winrt::XamlToolkit::WinUI::Interactivity::ResourceHelper::InvalidOperands(), TypeDisplayName(leftOperand), TypeDisplayName(convertedRightOperand), ComparisonConditionToString(operatorType));
                throw winrt::hresult_invalid_argument(message);
            }
        }

        return false;
    }

    bool DataTriggerBehavior::EvaluateComparable(
        winrt::IInspectable const& leftOperand,
        winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType const& operatorType,
        winrt::IInspectable const& rightOperand)
    {
        int comparison = CompareComparable(leftOperand, rightOperand);
        switch (operatorType)
        {
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::Equal:
            return comparison == 0;
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::NotEqual:
            return comparison != 0;
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThan:
            return comparison < 0;
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThanOrEqual:
            return comparison <= 0;
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThan:
            return comparison > 0;
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThanOrEqual:
            return comparison >= 0;
        }

        return false;
    }

    int DataTriggerBehavior::CompareComparable(winrt::IInspectable const& leftOperand, winrt::IInspectable const& rightOperand)
    {
        if (auto left = leftOperand.try_as<winrt::IReference<int32_t>>())
        {
            if (auto right = rightOperand.try_as<winrt::IReference<int32_t>>())
            {
                return left.Value() < right.Value() ? -1 : (left.Value() > right.Value() ? 1 : 0);
            }
        }
        if (auto left = leftOperand.try_as<winrt::IReference<double>>())
        {
            if (auto right = rightOperand.try_as<winrt::IReference<double>>())
            {
                return left.Value() < right.Value() ? -1 : (left.Value() > right.Value() ? 1 : 0);
            }
        }
        if (auto left = leftOperand.try_as<winrt::IReference<float>>())
        {
            if (auto right = rightOperand.try_as<winrt::IReference<float>>())
            {
                return left.Value() < right.Value() ? -1 : (left.Value() > right.Value() ? 1 : 0);
            }
        }
        if (auto left = leftOperand.try_as<winrt::IReference<bool>>())
        {
            if (auto right = rightOperand.try_as<winrt::IReference<bool>>())
            {
                return left.Value() == right.Value() ? 0 : (left.Value() ? 1 : -1);
            }
        }
        if (auto left = leftOperand.try_as<winrt::IReference<winrt::hstring>>())
        {
            if (auto right = rightOperand.try_as<winrt::IReference<winrt::hstring>>())
            {
                return std::wstring_view(left.Value()).compare(right.Value());
            }
        }
        if (auto leftStringable = leftOperand.try_as<winrt::IStringable>())
        {
            auto left = leftStringable.ToString();
            auto right = DataTriggerBehavior::ValueToString(rightOperand);
            if (!right.empty())
            {
                return std::wstring_view(left).compare(right.c_str());
            }
        }

        return 1;
    }

    winrt::IInspectable DataTriggerBehavior::ConvertRightOperand(winrt::IInspectable const& leftOperand, winrt::IInspectable const& rightOperand)
    {
        auto rightAsString = ValueToString(rightOperand);
        if (!rightAsString.empty())
        {
            try
            {
                return winrt::XamlBindingHelper::ConvertValue(InferTypeName(leftOperand), winrt::box_value(rightAsString));
            }
            catch (winrt::hresult_error const&)
            {
                auto converted = winrt::XamlToolkit::WinUI::Interactivity::TypeConverterHelper::Convert(rightAsString, TypeDisplayName(InferTypeName(leftOperand)));
                if (converted != nullptr)
                {
                    return converted;
                }
            }
        }

        return rightOperand;
    }

    winrt::Windows::UI::Xaml::Interop::TypeName DataTriggerBehavior::InferTypeName(winrt::IInspectable const& value)
    {
        using winrt::Windows::UI::Xaml::Interop::TypeKind;
        if (value.try_as<winrt::IReference<bool>>() != nullptr) return { L"Boolean", TypeKind::Primitive };
        if (value.try_as<winrt::IReference<int32_t>>() != nullptr) return { L"Int32", TypeKind::Primitive };
        if (value.try_as<winrt::IReference<uint32_t>>() != nullptr) return { L"UInt32", TypeKind::Primitive };
        if (value.try_as<winrt::IReference<int64_t>>() != nullptr) return { L"Int64", TypeKind::Primitive };
        if (value.try_as<winrt::IReference<uint64_t>>() != nullptr) return { L"UInt64", TypeKind::Primitive };
        if (value.try_as<winrt::IReference<float>>() != nullptr) return { L"Single", TypeKind::Primitive };
        if (value.try_as<winrt::IReference<double>>() != nullptr) return { L"Double", TypeKind::Primitive };
        if (value.try_as<winrt::IReference<winrt::hstring>>() != nullptr) return { L"String", TypeKind::Primitive };
        return { winrt::get_class_name(value), TypeKind::Metadata };
    }

    winrt::hstring DataTriggerBehavior::TypeDisplayName(winrt::IInspectable const& value)
    {
        if (value == nullptr)
        {
            return L"null";
        }

        return TypeDisplayName(InferTypeName(value));
    }

    winrt::hstring DataTriggerBehavior::TypeDisplayName(winrt::Windows::UI::Xaml::Interop::TypeName const& typeName)
    {
        return typeName.Name.empty() ? winrt::hstring{ L"Object" } : typeName.Name;
    }

    winrt::hstring DataTriggerBehavior::ValueToString(winrt::IInspectable const& value)
    {
        if (value == nullptr)
        {
            return {};
        }
        if (auto stringRef = value.try_as<winrt::IReference<winrt::hstring>>())
        {
            return stringRef.Value();
        }
        if (auto stringable = value.try_as<winrt::IStringable>())
        {
            return stringable.ToString();
        }
        return {};
    }

    void DataTriggerBehavior::OnValueChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        auto dataTriggerBehavior = winrt::get_self<DataTriggerBehavior>(dependencyObject.as<class_type>());
        if (dataTriggerBehavior->AssociatedObject() == nullptr)
        {
            return;
        }

        // TODO: Consider introducing an internal interface (e.g. IBindingRefreshable)
        // to allow actions to explicitly refresh their bindings, since C++/WinRT
        // does not support reflection-based binding updates like C#.
        winrt::XamlToolkit::WinUI::Interactivity::DataBindingHelper::RefreshDataBindingsOnActions(dataTriggerBehavior->Actions());

        if (DataTriggerBehavior::Compare(dataTriggerBehavior->Binding(), dataTriggerBehavior->ComparisonCondition(), dataTriggerBehavior->Value()))
        {
            implementation::Interaction::ExecuteActions(dataTriggerBehavior->AssociatedObject(), dataTriggerBehavior->Actions(), args);
        }
    }
}
