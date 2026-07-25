#include "pch.h"
#include "winrt_module_imports.h"
#include "StartTransitionAction.h"
#include "TransitionHelper.h"
#if __has_include("StartTransitionAction.g.cpp")
#include "StartTransitionAction.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> StartTransitionAction::TransitionProperty =
        winrt::DependencyProperty::Register(
            L"Transition",
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> StartTransitionAction::SourceProperty =
        winrt::DependencyProperty::Register(
            L"Source",
            winrt::xaml_typename<winrt::FrameworkElement>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> StartTransitionAction::TargetProperty =
        winrt::DependencyProperty::Register(
            L"Target",
            winrt::xaml_typename<winrt::FrameworkElement>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    winrt::IInspectable StartTransitionAction::Execute(
        [[maybe_unused]] winrt::IInspectable const& sender, 
        [[maybe_unused]] winrt::IInspectable const& parameter) const
    {
        auto transition = Transition();
        auto source = Source();
        auto target = Target();
        if (!transition || !source || !target)
        {
            throw winrt::hresult_invalid_argument(L"Transition, Source and Target are required.");
        }

        transition.Source(source);
        transition.Target(target);
        transition.StartAsync();

        return nullptr;
    }
}
