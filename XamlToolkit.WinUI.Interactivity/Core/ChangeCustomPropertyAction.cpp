#include "pch.h"
#include "winrt_module_imports.h"
#include "ChangeCustomPropertyAction.h"
#include "ResourceHelper.h"
#if __has_include("ChangeCustomPropertyAction.g.cpp")
#include "ChangeCustomPropertyAction.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ChangeCustomPropertyAction::PropertyNameProperty = winrt::DependencyProperty::Register(
        L"PropertyName",
        winrt::xaml_typename<winrt::PropertyPath>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> ChangeCustomPropertyAction::TargetObjectProperty = winrt::DependencyProperty::Register(
        L"TargetObject",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> ChangeCustomPropertyAction::ValueProperty = winrt::DependencyProperty::Register(
        L"Value",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr));

    winrt::PropertyPath ChangeCustomPropertyAction::PropertyName() const
    {
        return GetValue(PropertyNameProperty()).try_as<winrt::PropertyPath>();
    }

    void ChangeCustomPropertyAction::PropertyName(winrt::PropertyPath const& value)
    {
        SetValue(PropertyNameProperty(), value);
    }

    winrt::IInspectable ChangeCustomPropertyAction::Value() const
    {
        return GetValue(ValueProperty());
    }

    void ChangeCustomPropertyAction::Value(winrt::IInspectable const& value)
    {
        SetValue(ValueProperty(), value);
    }

    winrt::IInspectable ChangeCustomPropertyAction::TargetObject() const
    {
        return GetValue(TargetObjectProperty());
    }

    void ChangeCustomPropertyAction::TargetObject(winrt::IInspectable const& value)
    {
        SetValue(TargetObjectProperty(), value);
    }

    winrt::IInspectable ChangeCustomPropertyAction::Execute(winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& parameter)
    {
        winrt::IInspectable targetObject{ nullptr };
        if (ReadLocalValue(ChangeCustomPropertyAction::TargetObjectProperty()) != winrt::DependencyProperty::UnsetValue())
        {
            targetObject = TargetObject();
        }
        else
        {
            targetObject = sender;
        }

        auto propertyName = PropertyName();
        if (targetObject == nullptr || propertyName == nullptr)
        {
            return winrt::box_value(false);
        }

        UpdatePropertyValue(targetObject, propertyName.Path());
        return winrt::box_value(true);
    }

    void ChangeCustomPropertyAction::UpdatePropertyValue(winrt::IInspectable const& targetObject, winrt::hstring const& propertyPath)
    {
        auto customPropertyProvider = targetObject.try_as<winrt::ICustomPropertyProvider>();
        winrt::ICustomProperty property{ nullptr };
        if (customPropertyProvider)
        {
            property = customPropertyProvider.GetCustomProperty(propertyPath);
        }

        auto targetTypeName = customPropertyProvider ? customPropertyProvider.Type().Name : winrt::hstring{ L"Object" };
        ValidateProperty(targetTypeName, property, propertyPath);

        try
        {
            property.SetValue(targetObject, GetConvertedValue(property));
        }
        catch (winrt::hresult_error const&)
        {
            auto value = Value();
            auto incomingTypeName = value ? winrt::get_class_name(value) : winrt::hstring{ L"null" };
            auto propertyTypeName = property.Type().Name;
            auto message = ResourceHelper::Format(ResourceHelper::ChangePropertyActionCannotSetValueExceptionMessage(), incomingTypeName, propertyPath, propertyTypeName);
            throw winrt::hresult_invalid_argument(message);
        }
    }

    void ChangeCustomPropertyAction::ValidateProperty(winrt::hstring const& targetTypeName, winrt::ICustomProperty const& property, winrt::hstring const& propertyPath)
    {
        if (property == nullptr)
        {
            auto message = ResourceHelper::Format(ResourceHelper::ChangePropertyActionCannotFindPropertyNameExceptionMessage(), propertyPath, targetTypeName);
            throw winrt::hresult_invalid_argument(message);
        }
        else if (!property.CanWrite())
        {
            auto message = ResourceHelper::Format(ResourceHelper::ChangePropertyActionPropertyIsReadOnlyExceptionMessage(), propertyPath, targetTypeName);
            throw winrt::hresult_invalid_argument(message);
        }
    }

    winrt::IInspectable ChangeCustomPropertyAction::GetConvertedValue(winrt::ICustomProperty const& property) const
    {
        auto currentValue = Value();
        auto propertyType = property.Type();

        if (!currentValue)
        {
            return GetDefaultValue(propertyType);
        }

        return winrt::XamlBindingHelper::ConvertValue(propertyType, currentValue);
    }

    winrt::IInspectable ChangeCustomPropertyAction::GetDefaultValue(winrt::TypeName const& typeName)
    {
        const auto& name = typeName.Name;

        static const std::flat_map<std::wstring_view, winrt::PropertyType> type_map
        {
            { winrt::name_of<bool>(), winrt::PropertyType::Boolean },

            { winrt::name_of<int32_t>(), winrt::PropertyType::Int32 },
            { winrt::name_of<uint32_t>(), winrt::PropertyType::UInt32 },
            { winrt::name_of<int64_t>(), winrt::PropertyType::Int64 },
            { winrt::name_of<uint64_t>(), winrt::PropertyType::UInt64 },

            { winrt::name_of<float>(), winrt::PropertyType::Single },
            { winrt::name_of<double>(), winrt::PropertyType::Double },

            { winrt::name_of<uint8_t>(), winrt::PropertyType::UInt8 },
            { winrt::name_of<char16_t>(), winrt::PropertyType::Char16 },

            { winrt::name_of<winrt::hstring>(), winrt::PropertyType::String },

            { winrt::name_of<winrt::guid>(), winrt::PropertyType::Guid },

            { winrt::name_of<winrt::DateTime>(), winrt::PropertyType::DateTime },
            { L"DateTime", winrt::PropertyType::DateTime },

            { winrt::name_of<winrt::TimeSpan>(), winrt::PropertyType::TimeSpan },
            { L"TimeSpan", winrt::PropertyType::TimeSpan },

            { winrt::name_of<winrt::Point>(), winrt::PropertyType::Point },
            { L"Point", winrt::PropertyType::Point },

            { winrt::name_of<winrt::Rect>(), winrt::PropertyType::Rect },
            { L"Rect", winrt::PropertyType::Rect },

            { winrt::name_of<winrt::Size>(), winrt::PropertyType::Size },
            { L"Size", winrt::PropertyType::Size },
        };

        const auto it = type_map.find(typeName.Name);
        if (it == type_map.end())
        {
            return nullptr;
        }

        switch (it->second)
        {
        case winrt::PropertyType::Boolean:
            return winrt::box_value(false);

        case winrt::PropertyType::Int32:
            return winrt::box_value(int32_t{});

        case winrt::PropertyType::UInt32:
            return winrt::box_value(uint32_t{});

        case winrt::PropertyType::Int64:
            return winrt::box_value(int64_t{});

        case winrt::PropertyType::UInt64:
            return winrt::box_value(uint64_t{});

        case winrt::PropertyType::Single:
            return winrt::box_value(float{});

        case winrt::PropertyType::Double:
            return winrt::box_value(double{});

        case winrt::PropertyType::UInt8:
            return winrt::box_value(uint8_t{});

        case winrt::PropertyType::Char16:
            return winrt::box_value(char16_t{});

        case winrt::PropertyType::String:
            return winrt::box_value(winrt::hstring{});

        case winrt::PropertyType::Guid:
            return winrt::box_value(winrt::guid{});

        case winrt::PropertyType::DateTime:
            return winrt::box_value(winrt::DateTime{});

        case winrt::PropertyType::TimeSpan:
            return winrt::box_value(winrt::TimeSpan{});

        case winrt::PropertyType::Point:
            return winrt::box_value(winrt::Point{});

        case winrt::PropertyType::Rect:
            return winrt::box_value(winrt::Rect{});

        case winrt::PropertyType::Size:
            return winrt::box_value(winrt::Size{});

        default:
            return nullptr;
        }
    }
}