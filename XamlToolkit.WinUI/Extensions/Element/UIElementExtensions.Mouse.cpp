#include "pch.h"
#include "winrt_module_imports.h"
#include "UIElementExtensions.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> UIElementExtensions::CursorProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Cursor", 
            winrt::xaml_typename<winrt::InputSystemCursorShape>(), 
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::UIElementExtensions>(), 
            winrt::PropertyMetadata(winrt::box_value(winrt::InputSystemCursorShape::Arrow), &UIElementExtensions::CursorChanged));

    winrt::InputSystemCursorShape UIElementExtensions::GetCursor(winrt::FrameworkElement const& element)
    {
        return winrt::unbox_value<winrt::InputSystemCursorShape>(element.GetValue(CursorProperty()));
    }

    void UIElementExtensions::SetCursor(winrt::FrameworkElement const& element, winrt::InputSystemCursorShape const& value)
    {
        element.SetValue(CursorProperty(), winrt::box_value(value));
    }

    void UIElementExtensions::CursorChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::UIElement>();
        if (!element)
        {
            throw winrt::hresult_invalid_argument(L"element");
        }

        auto shape = winrt::unbox_value<winrt::InputSystemCursorShape>(e.NewValue());

        auto cursor = winrt::InputSystemCursor::Create(shape);

        if (const auto impl = element.try_as<winrt::IUIElementProtected>())
        {
            impl.ProtectedCursor(cursor);
        }
    }
}