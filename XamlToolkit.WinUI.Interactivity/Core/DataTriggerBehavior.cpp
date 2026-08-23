#include "pch.h"
#include "winrt_module_imports.h"
#include "DataTriggerBehavior.h"
#include "DataBindingHelper.h"
#include "ResourceHelper.h"
#include "../Interaction.h"
#if __has_include("DataTriggerBehavior.g.cpp")
#include "DataTriggerBehavior.g.cpp"
#endif

namespace winrt
{
    static winrt::hstring to_hstring(winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType value)
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
        if (leftOperand && rightOperand)
        {
            convertedRightOperand = ConvertRightOperand(leftOperand, rightOperand);
        }

        if (leftOperand && convertedRightOperand)
        {
            return EvaluateComparable(leftOperand, operatorType, convertedRightOperand);
        }

        switch (operatorType)
        {
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::Equal:
            return leftOperand == convertedRightOperand || (leftOperand && convertedRightOperand && DataTriggerBehavior::ValueToString(leftOperand) == DataTriggerBehavior::ValueToString(convertedRightOperand));

        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::NotEqual:
            return !(leftOperand == convertedRightOperand || (leftOperand && convertedRightOperand && DataTriggerBehavior::ValueToString(leftOperand) == DataTriggerBehavior::ValueToString(convertedRightOperand)));

        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThan:
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::LessThanOrEqual:
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThan:
        case winrt::XamlToolkit::WinUI::Interactivity::ComparisonConditionType::GreaterThanOrEqual:
        {
            auto message = ResourceHelper::Format(
                winrt::XamlToolkit::WinUI::Interactivity::ResourceHelper::InvalidOperands(), 
                TypeDisplayName(leftOperand), 
                TypeDisplayName(convertedRightOperand), 
                winrt::to_hstring(operatorType));

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

    int DataTriggerBehavior::CompareComparable(
        winrt::IInspectable const& leftOperand,
        winrt::IInspectable const& rightOperand)
    {
        const auto leftPv = leftOperand.try_as<winrt::IPropertyValue>();
        const auto rightPv = rightOperand.try_as<winrt::IPropertyValue>();

        if (leftPv && rightPv && leftPv.Type() == rightPv.Type())
        {
            switch (leftPv.Type())
            {
                case winrt::PropertyType::UInt8:
                {
                    const auto left = leftPv.GetUInt8();
                    const auto right = rightPv.GetUInt8();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::Int16:
                {
                    const auto left = leftPv.GetInt16();
                    const auto right = rightPv.GetInt16();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::UInt16:
                {
                    const auto left = leftPv.GetUInt16();
                    const auto right = rightPv.GetUInt16();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::Int32:
                {
                    const auto left = leftPv.GetInt32();
                    const auto right = rightPv.GetInt32();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::UInt32:
                {
                    const auto left = leftPv.GetUInt32();
                    const auto right = rightPv.GetUInt32();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::Int64:
                {
                    const auto left = leftPv.GetInt64();
                    const auto right = rightPv.GetInt64();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::UInt64:
                {
                    const auto left = leftPv.GetUInt64();
                    const auto right = rightPv.GetUInt64();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::Single:
                {
                    const auto left = leftPv.GetSingle();
                    const auto right = rightPv.GetSingle();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::Double:
                {
                    const auto left = leftPv.GetDouble();
                    const auto right = rightPv.GetDouble();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::Char16:
                {
                    const auto left = leftPv.GetChar16();
                    const auto right = rightPv.GetChar16();
                    return left < right ? -1 : left > right ? 1 : 0;
                }

                case winrt::PropertyType::Boolean:
                {
                    const auto left = leftPv.GetBoolean();
                    const auto right = rightPv.GetBoolean();
                    return left == right ? 0 : left ? 1 : -1;
                }

                case winrt::PropertyType::String:
                {
                    const auto left = leftPv.GetString();
                    const auto right = rightPv.GetString();
                    return std::wstring_view(left).compare(right);
                }

                case winrt::PropertyType::OtherType:
                {
                    if (winrt::get_class_name(leftOperand) == winrt::get_class_name(rightOperand))
                    {
                        if (leftPv.IsNumericScalar())
                        {
                            const auto left = leftPv.GetInt32();
                            const auto right = rightPv.GetInt32();
                            return left == right ? 0 : left ? 1 : -1;
                        }
                    }
                }
            }
        }

        if (const auto leftStringable = leftOperand.try_as<winrt::IStringable>())
        {
            const auto left = leftStringable.ToString();
            const auto right = DataTriggerBehavior::ValueToString(rightOperand);

            if (!right.empty())
            {
                return std::wstring_view(left).compare(right);
            }
        }

        return 1;
    }

    winrt::IInspectable DataTriggerBehavior::ConvertRightOperand(winrt::IInspectable const& leftOperand, winrt::IInspectable const& rightOperand)
    {
        return winrt::XamlBindingHelper::ConvertValue(InferTypeName(leftOperand), rightOperand);
    }

    winrt::TypeName DataTriggerBehavior::InferTypeName(winrt::IInspectable const& value)
    {
        const auto propertyValue = value.try_as<winrt::IPropertyValue>();

        if (propertyValue)
        {
            switch (propertyValue.Type())
            {
            case winrt::PropertyType::Boolean:
                return winrt::xaml_typename<bool>();

            case winrt::PropertyType::UInt8:
                return winrt::xaml_typename<uint8_t>();

            case winrt::PropertyType::Int16:
                return winrt::xaml_typename<int16_t>();

            case winrt::PropertyType::UInt16:
                return winrt::xaml_typename<uint16_t>();

            case winrt::PropertyType::Int32:
                return winrt::xaml_typename<int32_t>();

            case winrt::PropertyType::UInt32:
                return winrt::xaml_typename<uint32_t>();

            case winrt::PropertyType::Int64:
                return winrt::xaml_typename<int64_t>();

            case winrt::PropertyType::UInt64:
                return winrt::xaml_typename<uint64_t>();

            case winrt::PropertyType::Single:
                return winrt::xaml_typename<float>();

            case winrt::PropertyType::Double:
                return winrt::xaml_typename<double>();

            case winrt::PropertyType::Char16:
                return winrt::xaml_typename<char16_t>();

            case winrt::PropertyType::String:
                return winrt::xaml_typename<winrt::hstring>();

            case winrt::PropertyType::DateTime:
                return winrt::xaml_typename<winrt::DateTime>();

            case winrt::PropertyType::TimeSpan:
                return winrt::xaml_typename<winrt::TimeSpan>();

            case winrt::PropertyType::Point:
                return winrt::xaml_typename<winrt::Point>();

            case winrt::PropertyType::Rect:
                return winrt::xaml_typename<winrt::Rect>();

            case winrt::PropertyType::Size:
                return winrt::xaml_typename<winrt::Size>();
            }
        }

        return { winrt::get_class_name(value), winrt::TypeKind::Metadata };
    }

    winrt::hstring DataTriggerBehavior::TypeDisplayName(winrt::IInspectable const& value)
    {
        if (value == nullptr)
        {
            return L"null";
        }

        return TypeDisplayName(InferTypeName(value));
    }

    winrt::hstring DataTriggerBehavior::TypeDisplayName(winrt::TypeName const& typeName)
    {
        return typeName.Name.empty() ? L"Object" : typeName.Name;
    }

    winrt::hstring DataTriggerBehavior::ValueToString(winrt::IInspectable const& value)
    {
        if (value == nullptr)
        {
            return {};
        }
        if (const auto stringRef = value.try_as<winrt::IReference<winrt::hstring>>())
        {
            return stringRef.Value();
        }
        if (const auto stringable = value.try_as<winrt::IStringable>())
        {
            return stringable.ToString();
        }

        return {};
    }

    void DataTriggerBehavior::OnValueChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        auto dataTriggerBehavior = winrt::get_self<DataTriggerBehavior>(dependencyObject.as<class_type>());
        if (!dataTriggerBehavior->AssociatedObject())
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
