#include "pch.h"
#include "winrt_module_imports.h"
#include "ReverseTransitionAction.h"
#include "TransitionHelper.h"
#if __has_include("ReverseTransitionAction.g.cpp")
#include "ReverseTransitionAction.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ReverseTransitionAction::TransitionProperty =
        winrt::DependencyProperty::Register(
            L"Transition",
            winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::TransitionHelper>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    winrt::IInspectable ReverseTransitionAction::Execute(
        [[maybe_unused]] winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::IInspectable const& parameter) const
    {
        const auto transition = Transition();
        if (!transition)
        {
            throw winrt::hresult_invalid_argument(L"Transition is required.");
        }

        transition.ReverseAsync();

        return nullptr;
    }
}
