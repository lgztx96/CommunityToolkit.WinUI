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
    const wil::single_threaded_property<winrt::DependencyProperty> Interaction::BehaviorsProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Behaviors",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::Interaction>(),
            winrt::PropertyMetadata(nullptr, &Interaction::OnBehaviorsChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> Interaction::BehaviorTrackerProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"BehaviorTracker",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::Interaction>(),
            winrt::PropertyMetadata(nullptr));
        
    winrt::com_ptr<BehaviorTracker> Interaction::GetBehaviorTracker(winrt::FrameworkElement const& element)
    {
        const auto value = element.GetValue(BehaviorTrackerProperty());
        if (!value)
        {
            return nullptr;
        }

        return winrt::get_self<BehaviorTracker>(value)->get_strong();
    }

    winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection Interaction::GetBehaviors(winrt::DependencyObject const& obj)
    {
        if (!obj)
        {
            throw winrt::hresult_invalid_argument(L"obj");
        }

        auto behaviors = obj.GetValue(BehaviorsProperty()).try_as<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>();
        if (!behaviors)
        {
            behaviors = winrt::make<winrt::XamlToolkit::WinUI::Interactivity::implementation::BehaviorCollection>();
            obj.SetValue(BehaviorsProperty(), behaviors);

            if (const auto frameworkElement = obj.try_as<winrt::FrameworkElement>())
            {
                auto tracker = GetBehaviorTracker(frameworkElement);
                if (!tracker)
                {
                    tracker = winrt::make_self<BehaviorTracker>();
                    frameworkElement.SetValue(BehaviorTrackerProperty(), *tracker);
                }

                if (!tracker->loadedToken)
                {
                    tracker->loadedToken = frameworkElement.Loaded(&Interaction::FrameworkElement_Loaded);
                }

                if (frameworkElement.IsLoaded())
                {
                    FrameworkElement_Loaded(frameworkElement, nullptr);
                }
            }
        }

        return behaviors;
    }

    void Interaction::SetBehaviors(
        winrt::DependencyObject const& obj,
        winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection const& value)
    {
        if (!obj)
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
        if (!actions || winrt::Windows::ApplicationModel::DesignMode::DesignModeEnabled())
        {
            return winrt::single_threaded_vector<winrt::IInspectable>();
        }

        std::vector<winrt::IInspectable> results;
        for (const auto& dependencyObject : actions)
        {
            const auto action = dependencyObject.as<winrt::XamlToolkit::WinUI::Interactivity::IAction>();
            results.emplace_back(action.Execute(sender, parameter));
        }

        return winrt::single_threaded_vector(std::move(results));
    }

    void Interaction::OnBehaviorsChanged(
        winrt::DependencyObject const& sender,
        winrt::DependencyPropertyChangedEventArgs const& args)
    {
        const auto oldCollection = args.OldValue().try_as<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>();
        const auto newCollection = args.NewValue().try_as<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>();

        if (oldCollection == newCollection)
        {
            return;
        }

        if (oldCollection && oldCollection.AssociatedObject())
        {
            oldCollection.Detach();
        }

        if (newCollection && sender)
        {
            newCollection.Attach(sender);
        }
    }

    void Interaction::FrameworkElement_Loaded(
        winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (const auto frameworkElement = sender.try_as<winrt::FrameworkElement>())
        {
            GetBehaviors(frameworkElement).Attach(frameworkElement);

            if (const auto tracker = GetBehaviorTracker(frameworkElement))
            {
                if (!tracker->unloadedToken)
                {
                    tracker->unloadedToken = frameworkElement.Unloaded(&Interaction::FrameworkElement_Unloaded);
                }
            }
        }
    }

    void Interaction::FrameworkElement_Unloaded(
        winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (const auto frameworkElement = sender.try_as<winrt::FrameworkElement>())
        {
            if (const auto tracker = GetBehaviorTracker(frameworkElement))
            {
                if (tracker->unloadedToken)
                {
                    frameworkElement.Unloaded(tracker->unloadedToken);
                    tracker->unloadedToken = { 0 };
                }
            }
            
            if (const auto behaviors = frameworkElement.GetValue(BehaviorsProperty())
                .try_as<winrt::XamlToolkit::WinUI::Interactivity::BehaviorCollection>())
            {
                behaviors.Detach();
            }
        }
    }
}
