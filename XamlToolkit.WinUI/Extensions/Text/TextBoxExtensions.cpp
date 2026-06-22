#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"
#if __has_include("TextBoxExtensions.g.cpp")
#include "TextBoxExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
#pragma region SurfaceDial - Dependency Property Registration

    const wil::single_threaded_property<winrt::DependencyProperty> TextBoxExtensions::SurfaceDialOptionsProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"SurfaceDialOptions",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::SurfaceDialOptions>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::TextBoxExtensions>(),
            winrt::PropertyMetadata(nullptr, &TextBoxExtensions::OnSurfaceDialOptionsPropertyChanged));

#pragma endregion

#pragma region SurfaceDial - Getters and Setters

    winrt::XamlToolkit::WinUI::SurfaceDialOptions TextBoxExtensions::GetSurfaceDialOptions(winrt::TextBox const& obj)
    {
        return obj.GetValue(SurfaceDialOptionsProperty()).try_as<winrt::XamlToolkit::WinUI::SurfaceDialOptions>();
    }

    void TextBoxExtensions::SetSurfaceDialOptions(winrt::TextBox const& obj, winrt::XamlToolkit::WinUI::SurfaceDialOptions const& value)
    {
        obj.SetValue(SurfaceDialOptionsProperty(), value);
    }

    bool TextBoxExtensions::IsSurfaceDialOptionsSupported()
    {
        return winrt::RadialController::IsSupported();
    }

    winrt::RadialController TextBoxExtensions::Controller()
    {
        if (!_controller)
        {
            _controller = winrt::RadialController::CreateForCurrentView();
        }
        return _controller;
    }

    void TextBoxExtensions::Controller(winrt::RadialController const& value)
    {
        _controller = value;
    }

#pragma endregion
}
