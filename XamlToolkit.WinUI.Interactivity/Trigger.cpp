#include "pch.h"
#include "winrt_module_imports.h"
#include "Trigger.h"
#include "ActionCollection.h"
#if __has_include("Trigger.g.cpp")
#include "Trigger.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Trigger::ActionsProperty =
        winrt::DependencyProperty::Register(
            L"Actions",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Interactivity::ActionCollection>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    winrt::XamlToolkit::WinUI::Interactivity::ActionCollection Trigger::Actions()
    {
        auto actionCollection = GetValue(ActionsProperty()).try_as<winrt::XamlToolkit::WinUI::Interactivity::ActionCollection>();
        if (actionCollection == nullptr)
        {
            actionCollection = winrt::make<winrt::XamlToolkit::WinUI::Interactivity::implementation::ActionCollection>();
            SetValue(ActionsProperty(), actionCollection);
        }

        return actionCollection;
    }
}
