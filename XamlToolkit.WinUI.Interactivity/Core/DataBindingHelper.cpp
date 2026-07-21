#include "pch.h"
#include "winrt_module_imports.h"
#include "DataBindingHelper.h"
#include "Core\InvokeCommandAction.h"
#include "Core\ChangeCustomPropertyAction.h"
#include "Core\ChangeDependencyPropertyAction.h"
#include "Core\GoToStateAction.h"

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    void DataBindingHelper::RefreshDataBindingsOnActions(winrt::XamlToolkit::WinUI::Interactivity::ActionCollection const& actions)
    {
        for (auto const& dependencyObject : actions)
        {
            if (auto invokeCommandAction = dependencyObject.try_as<winrt::XamlToolkit::WinUI::Interactivity::InvokeCommandAction>())
            {
                RefreshBinding(invokeCommandAction, invokeCommandAction.CommandProperty());
                RefreshBinding(invokeCommandAction, invokeCommandAction.CommandParameterProperty());
                RefreshBinding(invokeCommandAction, invokeCommandAction.InputConverterProperty());
                RefreshBinding(invokeCommandAction, invokeCommandAction.InputConverterParameterProperty());
                RefreshBinding(invokeCommandAction, invokeCommandAction.InputConverterLanguageProperty());
            }
            else if (auto changeCustomPropertyAction = dependencyObject.try_as<winrt::XamlToolkit::WinUI::Interactivity::ChangeCustomPropertyAction>())
            {
                RefreshBinding(changeCustomPropertyAction, changeCustomPropertyAction.PropertyNameProperty());
                RefreshBinding(changeCustomPropertyAction, changeCustomPropertyAction.TargetObjectProperty());
                RefreshBinding(changeCustomPropertyAction, changeCustomPropertyAction.ValueProperty());
            }
            else if (auto changeDpPropertyAction = dependencyObject.try_as<winrt::XamlToolkit::WinUI::Interactivity::ChangeDependencyPropertyAction>())
            {
                RefreshBinding(changeDpPropertyAction, changeDpPropertyAction.TargetObjectProperty());
                RefreshBinding(changeDpPropertyAction, changeDpPropertyAction.ValueProperty());
            }
            else if (auto goToStateAction = dependencyObject.try_as<winrt::XamlToolkit::WinUI::Interactivity::GoToStateAction>())
            {
                RefreshBinding(goToStateAction, goToStateAction.UseTransitionsProperty());
                RefreshBinding(goToStateAction, goToStateAction.StateNameProperty());
                RefreshBinding(goToStateAction, goToStateAction.TargetObjectProperty());
            }
        }
    }

    void DataBindingHelper::RefreshBinding(winrt::DependencyObject const& target, winrt::DependencyProperty const& property)
    {
        auto binding = target.ReadLocalValue(property).try_as<winrt::BindingExpression>();
        if (binding && binding.ParentBinding())
        {
            winrt::BindingOperations::SetBinding(target, property, binding.ParentBinding());
        }
    }
}
