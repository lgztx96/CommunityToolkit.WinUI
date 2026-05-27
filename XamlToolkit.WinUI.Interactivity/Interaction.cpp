#include "pch.h"
#include "winrt_module_imports.h"
#include "Interaction.h"
#if __has_include("Interaction.g.cpp")
#include "Interaction.g.cpp"
#endif

#include "ActionCollection.h"
#include "BehaviorCollection.h"

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Interaction::BehaviorsProperty = winrt::DependencyProperty::RegisterAttached(
        L"Behaviors",
        winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>(),
        winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::Interaction>(),
        winrt::PropertyMetadata(nullptr, &Interaction::OnBehaviorsChanged));

    winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection Interaction::GetBehaviors(winrt::DependencyObject const& obj)
    {
        if (obj == nullptr)
        {
            throw winrt::hresult_invalid_argument(L"obj");
        }

        auto behaviors = obj.GetValue(BehaviorsProperty()).try_as<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>();
        if (behaviors == nullptr)
        {
            behaviors = winrt::make<winrt::XamlToolkit::WinUI::Interactivity::implementation::BehaviorCollection>();
            obj.SetValue(BehaviorsProperty(), behaviors);

            //if (auto frameworkElement = obj.try_as<winrt::FrameworkElement>())
            //{
            //    frameworkElement.Loaded({ &Interaction::FrameworkElement_Loaded });
            //    frameworkElement.Unloaded({ &Interaction::FrameworkElement_Unloaded });
            //}
        }

        return behaviors;
    }

    void Interaction::SetBehaviors(
        winrt::DependencyObject const& obj,
        winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection const& value)
    {
        if (obj == nullptr)
        {
            throw winrt::hresult_invalid_argument(L"obj");
        }

        obj.SetValue(BehaviorsProperty(), value);
    }

    winrt::IIterable<winrt::IInspectable> Interaction::ExecuteActions(
        winrt::IInspectable const& sender,
        winrt::XamlToolkit::WinUI::Interactivity::ActionCollection const& actions,
        winrt::IInspectable const& parameter)
    {
        if (actions == nullptr || winrt::Windows::ApplicationModel::DesignMode::DesignModeEnabled())
        {
            return winrt::single_threaded_vector<winrt::IInspectable>();
        }

        std::vector<winrt::IInspectable> results;
        for (winrt::DependencyObject const& dependencyObject : actions)
        {
            auto action = dependencyObject.as<winrt::XamlToolkit::WinUI::Interactivity::IAction>();
            results.emplace_back(action.Execute(sender, parameter));
        }

        return winrt::single_threaded_vector(std::move(results));
    }

    void Interaction::OnBehaviorsChanged(
        winrt::DependencyObject const& sender,
        winrt::DependencyPropertyChangedEventArgs const& args)
    {
        auto oldCollection = args.OldValue().try_as<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>();
        auto newCollection = args.NewValue().try_as<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>();

        if (oldCollection == newCollection)
        {
            return;
        }

        if (oldCollection != nullptr && oldCollection.AssociatedObject() != nullptr)
        {
            oldCollection.Detach();
        }

        if (newCollection != nullptr && sender != nullptr)
        {
            newCollection.Attach(sender);
        }
    }

    void Interaction::FrameworkElement_Loaded(
        winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (auto dependencyObject = sender.try_as<winrt::DependencyObject>())
        {
            GetBehaviors(dependencyObject).Attach(dependencyObject);
        }
    }

    void Interaction::FrameworkElement_Unloaded(
        winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (auto dependencyObject = sender.try_as<winrt::DependencyObject>())
        {
            GetBehaviors(dependencyObject).Detach();
        }
    }
}
