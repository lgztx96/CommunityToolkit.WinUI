#include "pch.h"
#include "winrt_module_imports.h"
#include "HyperlinkExtensions.h"
#if __has_include("HyperlinkExtensions.g.cpp")
#include "HyperlinkExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> HyperlinkExtensions::CommandProperty =
        DependencyProperty::RegisterAttached(
            L"Command",
            winrt::xaml_typename<winrt::ICommand>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::HyperlinkExtensions>(),
            winrt::PropertyMetadata(nullptr, OnCommandPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> HyperlinkExtensions::CommandParameterProperty =
        DependencyProperty::RegisterAttached(
            L"CommandParameter",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::HyperlinkExtensions>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> HyperlinkExtensions::ClickEventTokenProperty =
        DependencyProperty::RegisterAttached(
            L"ClickEventToken",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::HyperlinkExtensions>(),
            winrt::PropertyMetadata(nullptr));

    winrt::ICommand HyperlinkExtensions::GetCommand(winrt::Hyperlink const& obj)
    {
        return obj.GetValue(CommandProperty()).try_as<winrt::ICommand>();
    }

    void HyperlinkExtensions::SetCommand(winrt::Hyperlink const& obj, winrt::ICommand const& value)
    {
        obj.SetValue(CommandProperty(), value);
    }

    winrt::IInspectable HyperlinkExtensions::GetCommandParameter(winrt::Hyperlink const& obj) 
    { 
        return obj.GetValue(CommandParameterProperty());
    }

    void HyperlinkExtensions::SetCommandParameter(winrt::Hyperlink const& obj, winrt::IInspectable const& value) 
    {
        obj.SetValue(CommandParameterProperty(), value);
    }

    void HyperlinkExtensions::OnCommandPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (const auto hyperlink = sender.try_as<winrt::Hyperlink>())
        {
            if (const auto value = hyperlink.GetValue(ClickEventTokenProperty()))
            {
                if (const auto token = value.try_as<winrt::event_token>())
                {
                    hyperlink.Click(*token);
                }
            }

            if (args.NewValue().try_as<winrt::ICommand>())
            {
                auto token = hyperlink.Click(&HyperlinkExtensions::OnHyperlinkClicked);
                hyperlink.SetValue(ClickEventTokenProperty(), winrt::box_value(token));
            }
            else
            {
                hyperlink.ClearValue(ClickEventTokenProperty());
            }
        }
    }

    void HyperlinkExtensions::OnHyperlinkClicked(winrt::Hyperlink const& sender, [[maybe_unused]] winrt::HyperlinkClickEventArgs const& args)
    {
        auto command = GetCommand(sender);
        auto parameter = GetCommandParameter(sender);

        if (command && command.CanExecute(parameter))
        {
            command.Execute(parameter);
        }
    }
}
