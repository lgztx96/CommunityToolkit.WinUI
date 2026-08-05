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
            winrt::PropertyMetadata(winrt::box_value(winrt::VirtualKey::None)));

    const wil::single_threaded_property<winrt::DependencyProperty> KeyDownTriggerBehavior::ModifiersProperty =
        winrt::DependencyProperty::Register(
            L"Modifiers",
            winrt::xaml_typename<winrt::VirtualKeyModifiers>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::VirtualKeyModifiers::None)));

    const wil::single_threaded_property<winrt::DependencyProperty> KeyDownTriggerBehavior::ModifiersEnabledProperty =
        winrt::DependencyProperty::Register(
            L"ModifiersEnabled",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(false)));

    const wil::single_threaded_property<winrt::DependencyProperty> KeyDownTriggerBehavior::HandledEventsTooProperty =
        winrt::DependencyProperty::Register(
            L"HandledEventsToo",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(true)));

    void KeyDownTriggerBehavior::OnAttached()
    {
        base_type::OnAttached();

        if (auto frameworkElement = AssociatedObject().try_as<winrt::FrameworkElement>())
        {
            _previewKeyDownHandler = winrt::box_value(winrt::KeyEventHandler({ this, &KeyDownTriggerBehavior::OnAssociatedObjectPreviewKeyDown }));
            frameworkElement.AddHandler(winrt::UIElement::PreviewKeyDownEvent(), _previewKeyDownHandler, HandledEventsToo());
        }
    }

    void KeyDownTriggerBehavior::OnDetaching()
    {
        if (_previewKeyDownHandler)
        {
            if (auto frameworkElement = AssociatedObject().try_as<winrt::FrameworkElement>())
            {
                frameworkElement.RemoveHandler(winrt::UIElement::PreviewKeyDownEvent(), _previewKeyDownHandler);
            }

            _previewKeyDownHandler = nullptr;
        }

        base_type::OnDetaching();
    }

    void KeyDownTriggerBehavior::OnAssociatedObjectPreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& args)
    {
        if (args.Key() != Key())
        {
            return;
        }

        if (ModifiersEnabled() && !CheckModifiers())
        {
            return;
        }

        args.Handled(true);
        winrt::XamlToolkit::WinUI::Interactivity::Interaction::ExecuteActions(sender, Actions(), args);
    }

    bool KeyDownTriggerBehavior::CheckModifiers()
    {
        auto ctrlState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Control);
        auto shiftState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Shift);
        auto altState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Menu);

        bool ctrl = (static_cast<uint32_t>(ctrlState) & static_cast<uint32_t>(winrt::CoreVirtualKeyStates::Down)) != 0;
        bool shift = (static_cast<uint32_t>(shiftState) & static_cast<uint32_t>(winrt::CoreVirtualKeyStates::Down)) != 0;
        bool alt = (static_cast<uint32_t>(altState) & static_cast<uint32_t>(winrt::CoreVirtualKeyStates::Down)) != 0;

        return Match(winrt::VirtualKeyModifiers::Control, ctrl)
            && Match(winrt::VirtualKeyModifiers::Shift, shift)
            && Match(winrt::VirtualKeyModifiers::Menu, alt);
    }

    bool KeyDownTriggerBehavior::Match(winrt::VirtualKeyModifiers mod, bool isDown) const
    {
        bool required = (Modifiers() & mod) != winrt::VirtualKeyModifiers::None;
        return required == isDown;
    }
}
