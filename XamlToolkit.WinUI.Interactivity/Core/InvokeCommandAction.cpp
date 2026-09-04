#include "pch.h"
#include "winrt_module_imports.h"
#include "InvokeCommandAction.h"
#if __has_include("InvokeCommandAction.g.cpp")
#include "InvokeCommandAction.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> InvokeCommandAction::CommandProperty =
        winrt::DependencyProperty::Register(
            L"Command",
            winrt::xaml_typename<winrt::ICommand>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> InvokeCommandAction::CommandParameterProperty =
        winrt::DependencyProperty::Register(
            L"CommandParameter",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> InvokeCommandAction::InputConverterProperty =
        winrt::DependencyProperty::Register(
            L"InputConverter",
            winrt::xaml_typename<winrt::IValueConverter>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> InvokeCommandAction::InputConverterParameterProperty =
        winrt::DependencyProperty::Register(
            L"InputConverterParameter",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> InvokeCommandAction::InputConverterLanguageProperty =
        winrt::DependencyProperty::Register(
            L"InputConverterLanguage",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(L"")));

    winrt::ICommand InvokeCommandAction::Command() const
    {
        return GetValue(CommandProperty()).try_as<winrt::ICommand>();
    }

    void InvokeCommandAction::Command(winrt::ICommand const& value)
    {
        SetValue(CommandProperty(), value);
    }

    winrt::IInspectable InvokeCommandAction::CommandParameter() const
    {
        return GetValue(CommandParameterProperty());
    }

    void InvokeCommandAction::CommandParameter(winrt::IInspectable const& value)
    {
        SetValue(CommandParameterProperty(), value);
    }

    winrt::IValueConverter InvokeCommandAction::InputConverter() const
    {
        return GetValue(InputConverterProperty()).try_as<winrt::IValueConverter>();
    }

    void InvokeCommandAction::InputConverter(winrt::IValueConverter const& value)
    {
        SetValue(InputConverterProperty(), value);
    }

    winrt::IInspectable InvokeCommandAction::InputConverterParameter() const
    {
        return GetValue(InputConverterParameterProperty());
    }

    void InvokeCommandAction::InputConverterParameter(winrt::IInspectable const& value)
    {
        SetValue(InputConverterParameterProperty(), value);
    }

    winrt::hstring InvokeCommandAction::InputConverterLanguage() const
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(InputConverterLanguageProperty()));
    }

    void InvokeCommandAction::InputConverterLanguage(winrt::hstring const& value)
    {
        SetValue(InputConverterLanguageProperty(), winrt::box_value(value));
    }

    winrt::IInspectable InvokeCommandAction::Execute([[maybe_unused]] winrt::IInspectable const& sender, winrt::IInspectable const& parameter)
    {
        const auto command = Command();
        if (!command)
        {
            return winrt::box_value(false);
        }

        winrt::IInspectable resolvedParameter{ nullptr };
        if (ReadLocalValue(InvokeCommandAction::CommandParameterProperty()) != DependencyProperty::UnsetValue())
        {
            resolvedParameter = CommandParameter();
        }
        else if (const auto converter = InputConverter())
        {
            resolvedParameter = converter.Convert(
                parameter,
                winrt::xaml_typename<winrt::IInspectable>(),
                InputConverterParameter(),
                InputConverterLanguage());
        }
        else
        {
            resolvedParameter = parameter;
        }

        if (!command.CanExecute(resolvedParameter))
        {
            return winrt::box_value(false);
        }

        command.Execute(resolvedParameter);
        return winrt::box_value(true);
    }
}
