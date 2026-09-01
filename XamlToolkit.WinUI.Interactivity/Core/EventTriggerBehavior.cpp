#include "pch.h"
#include "winrt_module_imports.h"
#include "EventTriggerBehavior.h"
#include "EventManager.h"
#include "../ActionCollection.h"
#include "../Interaction.h"
#if __has_include("EventTriggerBehavior.g.cpp")
#include "EventTriggerBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> EventTriggerBehavior::EventNameProperty =
        winrt::DependencyProperty::Register(
            L"EventName",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(L"Loaded"), &EventTriggerBehavior::OnEventNameChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> EventTriggerBehavior::SourceObjectProperty =
        winrt::DependencyProperty::Register(
            L"SourceObject",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &EventTriggerBehavior::OnSourceObjectChanged));

    winrt::hstring EventTriggerBehavior::EventName() const
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(EventNameProperty()));
    }

    void EventTriggerBehavior::EventName(winrt::hstring const& value)
    {
        SetValue(EventNameProperty(), winrt::box_value(value));
    }

    winrt::IInspectable EventTriggerBehavior::SourceObject() const
    {
        return GetValue(SourceObjectProperty());
    }

    void EventTriggerBehavior::SourceObject(winrt::IInspectable const& value)
    {
        SetValue(SourceObjectProperty(), value);
    }

    void EventTriggerBehavior::OnAttached()
    {
        base_type::OnAttached();
        _currentEventName = EventName();
        SetResolvedSource(ComputeResolvedSource());
    }

    void EventTriggerBehavior::OnDetaching()
    {
        base_type::OnDetaching();
        SetResolvedSource(nullptr);
    }

    void EventTriggerBehavior::SetResolvedSource(winrt::IInspectable const& newSource)
    {
        if (AssociatedObject() == nullptr || _resolvedSource == newSource)
        {
            return;
        }

        if (_resolvedSource)
        {
            UnregisterEvent(_currentEventName);
        }

        _resolvedSource = newSource;

        if (_resolvedSource)
        {
            RegisterEvent(_currentEventName);
        }
    }

    winrt::IInspectable EventTriggerBehavior::ComputeResolvedSource()
    {
        // If the SourceObject property is set at all, we want to use it. It is possible that it is data
        // bound and bindings haven't been evaluated yet. Plus, this makes the API more predictable.
        if (ReadLocalValue(EventTriggerBehavior::SourceObjectProperty()) != winrt::DependencyProperty::UnsetValue())
        {
            return SourceObject();
        }

        return AssociatedObject();
    }

    void EventTriggerBehavior::RegisterEvent(winrt::hstring const& eventName)
    {
        if (eventName.empty())
        {
            return;
        }

        if (eventName != L"Loaded")
        {
            _registeredToken = EventManager::Register(eventName, _resolvedSource, [this](winrt::IInspectable const& eventArgs) { OnEvent(_resolvedSource, eventArgs); });
        }
        else if (!_isLoadedEventRegistered)
        {
            const auto element = _resolvedSource.try_as<winrt::FrameworkElement>();
            if (element && !IsElementLoaded(element))
            {
                _isLoadedEventRegistered = true;
                _loadedToken = element.Loaded({ this, &EventTriggerBehavior::OnEvent });
            }
        }
    }

    void EventTriggerBehavior::UnregisterEvent(winrt::hstring const& eventName)
    {
        if (eventName.empty())
        {
            return;
        }

        if (eventName != L"Loaded")
        {
            if (_registeredToken)
            {
                EventManager::Unregister(eventName, _resolvedSource, _registeredToken);
                _registeredToken = { 0 };
            }
        }
		else if (_isLoadedEventRegistered)
        {
            _isLoadedEventRegistered = false;
            if (const auto element = _resolvedSource.try_as<winrt::FrameworkElement>())
            {
                element.Loaded(_loadedToken);
            }
        }
    }

    void EventTriggerBehavior::OnEvent([[maybe_unused]] winrt::IInspectable const& sender, winrt::IInspectable const& eventArgs)
    {
        Interaction::ExecuteActions(_resolvedSource, Actions(), eventArgs);
    }

    void EventTriggerBehavior::OnSourceObjectChanged(winrt::DependencyObject const& dependencyObject, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
    {
        const auto behavior = winrt::get_self<EventTriggerBehavior>(dependencyObject.as<class_type>());
        behavior->SetResolvedSource(behavior->ComputeResolvedSource());
    }

    void EventTriggerBehavior::OnEventNameChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        const auto behavior = winrt::get_self<EventTriggerBehavior>(dependencyObject.as<class_type>());
        if (behavior->AssociatedObject() == nullptr || behavior->_resolvedSource == nullptr)
        {
            return;
        }

        const auto oldEventName = winrt::unbox_value_or<winrt::hstring>(args.OldValue(), L"");
        const auto newEventName = winrt::unbox_value_or<winrt::hstring>(args.NewValue(), L"");

        behavior->_currentEventName = newEventName;
        behavior->UnregisterEvent(oldEventName);
        behavior->RegisterEvent(newEventName);
    }

    bool EventTriggerBehavior::IsElementLoaded(winrt::FrameworkElement const& element)
    {
        if (!element)
        {
            return false;
        }

        winrt::UIElement rootVisual{ nullptr };
        if (const auto xamlRoot = element.XamlRoot())
        {
            rootVisual = xamlRoot.Content();
        }

        winrt::DependencyObject parent = element.Parent();
        if (!parent)
        {
            // If the element is the child of a ControlTemplate it will have a null parent even when it is loaded.
            // To catch that scenario, also check its parent in the visual tree.
            parent = winrt::VisualTreeHelper::GetParent(element);
        }

        return (parent || (rootVisual && element == rootVisual));
    }
}
