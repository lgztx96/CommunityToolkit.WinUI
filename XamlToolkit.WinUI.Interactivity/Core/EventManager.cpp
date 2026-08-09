#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <string>
#include <unordered_map>
#endif
#include "EventManager.h"
#include "ResourceHelper.h"
#if __has_include("EventManager.g.cpp")
#include "EventManager.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
#define UNREGISTER_EVENT(event_name, type) \
[](winrt::Windows::Foundation::IInspectable const& sender, winrt::event_token token) { sender.as<type>().event_name(token); }

#define ADD_DESCRIPTION(event_name, type, args_type) \
EventManager::AddDescription(L#event_name, [](winrt::Windows::Foundation::IInspectable const& sender, Action const& action) \
{ return sender.as<type>().event_name([action](auto&&, args_type const& e) { action(e); }); }, \
UNREGISTER_EVENT(event_name, type))

#define ADD_DESCRIPTION_NOARGS(event_name, type) \
EventManager::AddDescription(L#event_name, [](winrt::Windows::Foundation::IInspectable const& sender, Action const& action) \
{ return sender.as<type>().event_name([action](auto&&, auto&&) { action(nullptr); }); }, \
UNREGISTER_EVENT(event_name, type))

    struct wstring_hash 
    {
        using is_transparent = void;
        size_t operator()(const wchar_t* str) const { return std::hash<std::wstring_view>{}(str); }
        size_t operator()(std::wstring_view str) const { return std::hash<std::wstring_view>{}(str); }
        size_t operator()(const std::wstring& str) const { return std::hash<std::wstring_view>{}(str); }
        size_t operator()(const winrt::hstring& str) const { return std::hash<std::wstring_view>{}(str); }
    };

    using EventNameToRegisterHandlerMap = std::unordered_map<std::wstring, RegisterHandler, wstring_hash, std::equal_to<>>;
    using EventNameToUnregisterHandlerMap = std::unordered_map<std::wstring, UnregisterHandler, wstring_hash, std::equal_to<>>;

    namespace Global
    {
        static EventNameToRegisterHandlerMap RegisterHandlers;
        static EventNameToUnregisterHandlerMap UnregisterHandlers;
    }

    namespace
    {
        [[noreturn]] void ThrowCannotFindEvent(std::wstring_view eventName, winrt::Windows::Foundation::IInspectable const& sender)
        {
            auto typeName = winrt::get_class_name(sender);
            auto message = ResourceHelper::Format(
                winrt::XamlToolkit::WinUI::Interactivity::ResourceHelper::CannotFindEventNameExceptionMessage(),
                eventName, 
                typeName);
            throw winrt::hresult_error(E_FAIL, message);
        }
    }

    void EventManager::AddDescription(std::wstring_view eventName, RegisterHandler registerHandler, UnregisterHandler unregisterHandler)
    {
        Global::RegisterHandlers.emplace(eventName, registerHandler);
        Global::UnregisterHandlers.emplace(eventName, unregisterHandler);
    }

    winrt::event_token EventManager::Register(
        winrt::hstring const& eventName,
        winrt::Windows::Foundation::IInspectable const& sender,
        Action const& action)
    {
        if (Global::RegisterHandlers.empty())
        {
            EventManager::AddDefaultEvents();
        }

        if (auto iterator = Global::RegisterHandlers.find(eventName); iterator != Global::RegisterHandlers.end())
        {
            return iterator->second(sender, action);
        }

        ThrowCannotFindEvent(eventName, sender);
    }

    void EventManager::Unregister(
        winrt::hstring const& eventName,
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::event_token token)
    {
        if (Global::UnregisterHandlers.empty())
        {
            EventManager::AddDefaultEvents();
        }

        if (auto iterator = Global::UnregisterHandlers.find(eventName); iterator != Global::UnregisterHandlers.end())
        {
            iterator->second(sender, token);
            return;
        }

        ThrowCannotFindEvent(eventName, sender);
    }

    void EventManager::AddDefaultEvents()
    {
        if (!Global::RegisterHandlers.empty())
        {
            return;
        }

        ADD_DESCRIPTION(PointerPressed, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(PointerReleased, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(PointerMoved, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(PointerEntered, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(PointerExited, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(PointerCanceled, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(PointerCaptureLost, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(PointerWheelChanged, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs);
        ADD_DESCRIPTION(Tapped, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs);
        ADD_DESCRIPTION(DoubleTapped, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::DoubleTappedRoutedEventArgs);
        ADD_DESCRIPTION(RightTapped, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::RightTappedRoutedEventArgs);
        ADD_DESCRIPTION(Holding, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::HoldingRoutedEventArgs);
        ADD_DESCRIPTION(KeyDown, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs);
        ADD_DESCRIPTION(KeyUp, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs);
        ADD_DESCRIPTION(CharacterReceived, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::CharacterReceivedRoutedEventArgs);
        ADD_DESCRIPTION(DragEnter, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::DragEventArgs);
        ADD_DESCRIPTION(DragLeave, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::DragEventArgs);
        ADD_DESCRIPTION(DragOver, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::DragEventArgs);
        ADD_DESCRIPTION(Drop, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::DragEventArgs);
        ADD_DESCRIPTION(GotFocus, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(LostFocus, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(GettingFocus, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::GettingFocusEventArgs);
        ADD_DESCRIPTION(LosingFocus, winrt::Microsoft::UI::Xaml::UIElement, winrt::Microsoft::UI::Xaml::Input::LosingFocusEventArgs);

        ADD_DESCRIPTION(Unloaded, winrt::Microsoft::UI::Xaml::FrameworkElement, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(SizeChanged, winrt::Microsoft::UI::Xaml::FrameworkElement, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs);
        ADD_DESCRIPTION(DataContextChanged, winrt::Microsoft::UI::Xaml::FrameworkElement, winrt::Microsoft::UI::Xaml::DataContextChangedEventArgs);
        ADD_DESCRIPTION_NOARGS(LayoutUpdated, winrt::Microsoft::UI::Xaml::FrameworkElement);
        ADD_DESCRIPTION_NOARGS(ActualThemeChanged, winrt::Microsoft::UI::Xaml::FrameworkElement);

        ADD_DESCRIPTION(Click, winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(Checked, winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(Unchecked, winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(Indeterminate, winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(TextChanged, winrt::Microsoft::UI::Xaml::Controls::TextBox, winrt::Microsoft::UI::Xaml::Controls::TextChangedEventArgs);
        ADD_DESCRIPTION(SelectionChanged, winrt::Microsoft::UI::Xaml::Controls::Primitives::Selector, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs);
        ADD_DESCRIPTION(ValueChanged, winrt::Microsoft::UI::Xaml::Controls::Primitives::RangeBase, winrt::Microsoft::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs);
        ADD_DESCRIPTION(Toggled, winrt::Microsoft::UI::Xaml::Controls::ToggleSwitch, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(PasswordChanged, winrt::Microsoft::UI::Xaml::Controls::PasswordBox, winrt::Microsoft::UI::Xaml::RoutedEventArgs);
        ADD_DESCRIPTION(ViewChanged, winrt::Microsoft::UI::Xaml::Controls::ScrollViewer, winrt::Microsoft::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs);
    }
}
