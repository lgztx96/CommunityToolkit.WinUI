#include "pch.h"
#include "winrt_module_imports.h"
#include "InvokeActionsActivity.h"
#if __has_include("InvokeActionsActivity.g.cpp")
#include "InvokeActionsActivity.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> InvokeActionsActivity::ActionsProperty =
        winrt::DependencyProperty::Register(
            L"Actions",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::ActionCollection>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    winrt::XamlToolkit::WinUI::Interactivity::ActionCollection InvokeActionsActivity::Actions()
    {
        auto actionCollection = GetValue(ActionsProperty()).try_as<winrt::XamlToolkit::WinUI::Interactivity::ActionCollection>();
        if (actionCollection == nullptr)
        {
            actionCollection = winrt::XamlToolkit::WinUI::Interactivity::ActionCollection();
            SetValue(ActionsProperty(), actionCollection);
        }

        return actionCollection;
    }

    winrt::IAsyncAction InvokeActionsActivity::InvokeAsync(winrt::UIElement element)
    {
        auto strongThis = get_strong();
		co_await base_type::InvokeAsync(element);
        winrt::XamlToolkit::WinUI::Interactivity::Interaction::ExecuteActions(element, Actions(), nullptr);
    }
}

