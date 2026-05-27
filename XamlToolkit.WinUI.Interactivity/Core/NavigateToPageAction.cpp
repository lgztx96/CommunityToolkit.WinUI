#include "pch.h"
#include "winrt_module_imports.h"
#include "NavigateToPageAction.h"
#include "../Utility/WinUIVisualTreeHelper.h"
#if __has_include("NavigateToPageAction.g.cpp")
#include "NavigateToPageAction.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    NavigateToPageAction::NavigateToPageAction()
        : _visualTreeHelper(std::make_unique<winrt::XamlToolkit::WinUI::Interactivity::WinUIVisualTreeHelper>())
    {
    }

    const wil::single_threaded_property<winrt::DependencyProperty> NavigateToPageAction::TargetPageProperty = winrt::DependencyProperty::Register(
        L"TargetPage",
        winrt::xaml_typename<winrt::hstring>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> NavigateToPageAction::ParameterProperty = winrt::DependencyProperty::Register(
        L"Parameter",
        winrt::xaml_typename<winrt::IInspectable>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr));

    winrt::hstring NavigateToPageAction::TargetPage() const
    {
        auto value = GetValue(TargetPageProperty());
        return winrt::unbox_value_or<winrt::hstring>(value, L"");
    }

    void NavigateToPageAction::TargetPage(winrt::hstring const& value)
    {
        SetValue(TargetPageProperty(), winrt::box_value(value));
    }

    winrt::IInspectable NavigateToPageAction::Parameter() const
    {
        return GetValue(ParameterProperty());
    }

    void NavigateToPageAction::Parameter(winrt::IInspectable const& value)
    {
        SetValue(ParameterProperty(), value);
    }

    winrt::IInspectable NavigateToPageAction::Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter)
    {
		auto targetPage = TargetPage();
        if (targetPage.empty())
        {
            return winrt::box_value(false);
        }

        auto metadataProvider = winrt::Application::Current().try_as<winrt::Markup::IXamlMetadataProvider>();
        if (metadataProvider == nullptr)
        {
            // This will happen if there are no XAML files in the project other than App.xaml.
            // The markup compiler doesn't bother implementing IXamlMetadataProvider on the app
            // in that case.
            return winrt::box_value(false);
        }

        auto xamlType = metadataProvider.GetXamlType(targetPage);
        if (xamlType == nullptr)
        {
            return winrt::box_value(false);
        }

        winrt::INavigate navigateElement{ nullptr };
        if (auto element = sender.try_as<winrt::UIElement>())
        {
            if (auto xamlRoot = element.XamlRoot())
            {
                navigateElement = xamlRoot.Content().try_as<winrt::INavigate>();
            }
        }

        auto senderObject = sender.try_as<winrt::DependencyObject>();

        // If the sender wasn't an INavigate, then keep looking up the tree from the
        // root we were given for another INavigate.
        while (senderObject != nullptr && navigateElement == nullptr)
        {
            navigateElement = senderObject.try_as<winrt::Controls::INavigate>();
            if (navigateElement == nullptr)
            {
                senderObject = _visualTreeHelper ? _visualTreeHelper->GetParent(senderObject) : nullptr;
            }
        }

        if (navigateElement == nullptr)
        {
            return winrt::box_value(false);
        }

        if (auto frame = navigateElement.try_as<winrt::Frame>())
        {
            auto navigationParameter = Parameter() != nullptr ? Parameter() : parameter;
            return winrt::box_value(frame.Navigate(xamlType.UnderlyingType(), navigationParameter));
        }
        else
        {
            return winrt::box_value(navigateElement.Navigate(xamlType.UnderlyingType()));
        }
    }
}