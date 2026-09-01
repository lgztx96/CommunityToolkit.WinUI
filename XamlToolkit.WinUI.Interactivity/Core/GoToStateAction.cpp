#include "pch.h"
#include "winrt_module_imports.h"
#include "GoToStateAction.h"
#include "EventTriggerBehavior.h"
#include "ResourceHelper.h"
#include "../VisualStateUtilities.h"
#if __has_include("GoToStateAction.g.cpp")
#include "GoToStateAction.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> GoToStateAction::UseTransitionsProperty =
        winrt::DependencyProperty::Register(
            L"UseTransitions",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(true)));

    const wil::single_threaded_property<winrt::DependencyProperty> GoToStateAction::StateNameProperty =
        winrt::DependencyProperty::Register(
            L"StateName",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> GoToStateAction::TargetObjectProperty =
        winrt::DependencyProperty::Register(
            L"TargetObject",
            winrt::xaml_typename<winrt::FrameworkElement>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    bool GoToStateAction::UseTransitions() const
    {
        return winrt::unbox_value_or<bool>(GetValue(UseTransitionsProperty()), true);
    }

    void GoToStateAction::UseTransitions(bool value)
    {
        SetValue(UseTransitionsProperty(), winrt::box_value(value));
    }

    winrt::hstring GoToStateAction::StateName() const
    {
        return winrt::unbox_value_or<winrt::hstring>(GetValue(StateNameProperty()), L"");
    }

    void GoToStateAction::StateName(winrt::hstring const& value)
    {
        SetValue(StateNameProperty(), winrt::box_value(value));
    }

    winrt::FrameworkElement GoToStateAction::TargetObject() const
    {
        return GetValue(TargetObjectProperty()).try_as<winrt::FrameworkElement>();
    }

    void GoToStateAction::TargetObject(winrt::FrameworkElement const& value)
    {
        SetValue(TargetObjectProperty(), value);
    }

    winrt::IInspectable GoToStateAction::Execute(winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& parameter)
    {
        const auto stateName = StateName();
        if (stateName.empty())
        {
            return winrt::box_value(false);
        }

        if (ReadLocalValue(GoToStateAction::TargetObjectProperty()) != winrt::DependencyProperty::UnsetValue())
        {
            const auto control = TargetObject().try_as<winrt::Control>();
            if (!control)
            {
                return winrt::box_value(false);
            }

            return winrt::box_value(VisualStateUtilities::GoToState(control, stateName, UseTransitions()));
        }

        const auto element = sender.try_as<winrt::FrameworkElement>();
        if (!element || !EventTriggerBehavior::IsElementLoaded(element))
        {
            return winrt::box_value(false);
        }

        const auto resolvedControl = VisualStateUtilities::FindNearestStatefulControl(element);
        if (!resolvedControl)
        {
            const auto message = ResourceHelper::Format(ResourceHelper::GoToStateActionTargetHasNoStateGroups(), element.Name());
            throw winrt::hresult_error(E_FAIL, message);
        }

        return winrt::box_value(VisualStateUtilities::GoToState(resolvedControl, stateName, UseTransitions()));
    }
}
