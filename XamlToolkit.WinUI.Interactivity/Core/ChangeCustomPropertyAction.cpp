#include "pch.h"
#include "winrt_module_imports.h"
#include "ChangeCustomPropertyAction.h"
#include "ResourceHelper.h"
#include "TypeConverterHelper.h"
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
        if (customPropertyProvider != nullptr)
        {
            property = customPropertyProvider.GetCustomProperty(propertyPath);
        }

        auto targetTypeName = customPropertyProvider != nullptr ? TypeNameToString(customPropertyProvider.Type()) : winrt::hstring{ L"Object" };
        ValidateProperty(targetTypeName, property, propertyPath);

        try
        {
            property.SetValue(targetObject, GetConvertedValue(property));
        }
        catch (winrt::hresult_error const&)
        {
            auto value = Value();
            auto incomingTypeName = value != nullptr ? winrt::get_class_name(value) : winrt::hstring{ L"null" };
            auto propertyTypeName = TypeNameToString(property.Type());
            auto message = ResourceHelper::Format(winrt::XamlToolkit::WinUI::Interactivity::ResourceHelper::ChangePropertyActionCannotSetValueExceptionMessage(), incomingTypeName, propertyPath, propertyTypeName);
            throw winrt::hresult_invalid_argument(message);
        }
    }

    void ChangeCustomPropertyAction::ValidateProperty(winrt::hstring const& targetTypeName, winrt::ICustomProperty const& property, winrt::hstring const& propertyPath)
    {
        if (property == nullptr)
        {
            auto message = ResourceHelper::Format(winrt::XamlToolkit::WinUI::Interactivity::ResourceHelper::ChangePropertyActionCannotFindPropertyNameExceptionMessage(), propertyPath, targetTypeName);
            throw winrt::hresult_invalid_argument(message);
        }
        else if (!property.CanWrite())
        {
            auto message = ResourceHelper::Format(winrt::XamlToolkit::WinUI::Interactivity::ResourceHelper::ChangePropertyActionPropertyIsReadOnlyExceptionMessage(), propertyPath, targetTypeName);
            throw winrt::hresult_invalid_argument(message);
        }
    }

    winrt::IInspectable ChangeCustomPropertyAction::GetConvertedValue(winrt::ICustomProperty const& property) const
    {
        auto currentValue = Value();
        auto propertyType = property.Type();

        if (currentValue == nullptr)
        {
            return GetDefaultValue(propertyType);
        }

        auto asString = InspectableToString(currentValue);
        if (!asString.empty())
        {
            try
            {
                return winrt::XamlBindingHelper::ConvertValue(propertyType, winrt::box_value(asString));
            }
            catch (winrt::hresult_error const&)
            {
                auto converted = winrt::XamlToolkit::WinUI::Interactivity::TypeConverterHelper::Convert(asString, TypeNameToString(propertyType));
                if (converted != nullptr)
                {
                    return converted;
                }
            }
        }

        return currentValue;
    }

    winrt::IInspectable ChangeCustomPropertyAction::GetDefaultValue(winrt::Windows::UI::Xaml::Interop::TypeName const& typeName)
    {
        auto const name = TypeNameToString(typeName);
        if (name == L"Boolean" || name == L"System.Boolean") return winrt::box_value(false);
        if (name == L"Int32" || name == L"System.Int32") return winrt::box_value(int32_t{});
        if (name == L"UInt32" || name == L"System.UInt32") return winrt::box_value(uint32_t{});
        if (name == L"Int64" || name == L"System.Int64") return winrt::box_value(int64_t{});
        if (name == L"UInt64" || name == L"System.UInt64") return winrt::box_value(uint64_t{});
        if (name == L"Single" || name == L"System.Single") return winrt::box_value(float{});
        if (name == L"Double" || name == L"System.Double") return winrt::box_value(double{});
        if (name == L"Byte" || name == L"System.Byte") return winrt::box_value(uint8_t{});
        if (name == L"Char16" || name == L"Char" || name == L"System.Char") return winrt::box_value(char16_t{});
        if (name == L"String" || name == L"System.String") return winrt::box_value(winrt::hstring{});
        if (name == L"TimeSpan" || name == L"Windows.Foundation.TimeSpan") return winrt::box_value(winrt::Windows::Foundation::TimeSpan{});
        if (name == L"DateTime" || name == L"Windows.Foundation.DateTime") return winrt::box_value(winrt::Windows::Foundation::DateTime{});
        if (name == L"Point" || name == L"Windows.Foundation.Point") return winrt::box_value(winrt::Windows::Foundation::Point{});
        if (name == L"Rect" || name == L"Windows.Foundation.Rect") return winrt::box_value(winrt::Windows::Foundation::Rect{});
        if (name == L"Size" || name == L"Windows.Foundation.Size") return winrt::box_value(winrt::Windows::Foundation::Size{});

        return nullptr;
    }

    winrt::hstring ChangeCustomPropertyAction::InspectableToString(winrt::IInspectable const& value)
    {
        if (value == nullptr)
        {
            return {};
        }

        if (auto stringValue = value.try_as<winrt::IReference<winrt::hstring>>())
        {
            return stringValue.Value();
        }

        if (auto stringable = value.try_as<winrt::IStringable>())
        {
            return stringable.ToString();
        }

        return {};
    }

    winrt::hstring ChangeCustomPropertyAction::TypeNameToString(winrt::Windows::UI::Xaml::Interop::TypeName const& typeName)
    {
        return typeName.Name;
    }
}