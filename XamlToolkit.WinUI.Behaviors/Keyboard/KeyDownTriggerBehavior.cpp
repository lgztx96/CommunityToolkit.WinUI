#include "pch.h"
#include "winrt_module_imports.h"
#include "KeyDownTriggerBehavior.h"
#if __has_include("KeyDownTriggerBehavior.g.cpp")
#include "KeyDownTriggerBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> KeyDownTriggerBehavior::KeyProperty =
        winrt::DependencyProperty::Register(
            L"Key",
            winrt::xaml_typename<winrt::VirtualKey>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    void KeyDownTriggerBehavior::OnAttached()
    {
        base_type::OnAttached();

        if (auto frameworkElement = AssociatedObject().try_as<winrt::FrameworkElement>())
        {
            _keyDownRevoker = frameworkElement.KeyDown(winrt::auto_revoke, { this, &KeyDownTriggerBehavior::OnAssociatedObjectKeyDown });
        }
    }

    void KeyDownTriggerBehavior::OnDetaching()
    {
        _keyDownRevoker.revoke();

        base_type::OnDetaching();
    }

    void KeyDownTriggerBehavior::OnAssociatedObjectKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& args)
    {
        if (args.Key() == Key())
        {
            args.Handled(true);
            winrt::XamlToolkit::WinUI::Interactivity::Interaction::ExecuteActions(sender, Actions(), args);
        }
    }
}
