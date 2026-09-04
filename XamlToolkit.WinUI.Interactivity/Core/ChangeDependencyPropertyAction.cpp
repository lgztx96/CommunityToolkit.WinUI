#include "pch.h"
#include "winrt_module_imports.h"
#include "ChangeDependencyPropertyAction.h"
#if __has_include("ChangeDependencyPropertyAction.g.cpp")
#include "ChangeDependencyPropertyAction.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ChangeDependencyPropertyAction::TargetObjectProperty =
        winrt::DependencyProperty::Register(
            L"TargetObject",
            winrt::xaml_typename<winrt::DependencyObject>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> ChangeDependencyPropertyAction::ValueProperty =
        winrt::DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    winrt::IInspectable ChangeDependencyPropertyAction::Value() const
    {
        return GetValue(ValueProperty());
    }

    void ChangeDependencyPropertyAction::Value(winrt::IInspectable const& value)
    {
        SetValue(ValueProperty(), value);
    }

    winrt::DependencyObject ChangeDependencyPropertyAction::TargetObject() const
    {
        return GetValue(TargetObjectProperty()).try_as<winrt::DependencyObject>();
    }

    void ChangeDependencyPropertyAction::TargetObject(winrt::DependencyObject const& value)
    {
        SetValue(TargetObjectProperty(), value);
    }

    winrt::IInspectable ChangeDependencyPropertyAction::Execute(winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& parameter)
    {
        winrt::DependencyObject targetObject{ nullptr };
        if (ReadLocalValue(ChangeDependencyPropertyAction::TargetObjectProperty()) != winrt::DependencyProperty::UnsetValue())
        {
            targetObject = TargetObject();
        }
        else
        {
            targetObject = sender.try_as<winrt::DependencyObject>();
        }

        const auto poperty = Property();
        if (!targetObject || !poperty)
        {
            return winrt::box_value(false);
        }

        targetObject.SetValue(poperty, Value());
        return winrt::box_value(true);
    }
}
