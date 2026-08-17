#include "pch.h"
#include "winrt_module_imports.h"
#include "SizeExtensions.h"
#if __has_include("SizeExtensions.g.cpp")
#include "SizeExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    winrt::Rect SizeExtensions::ToRect(winrt::Size size) noexcept
    {
        return winrt::Rect(0, 0, size.Width, size.Height);
    }

    winrt::Rect SizeExtensions::ToRect(winrt::Size size, double x, double y) noexcept
    {
        return winrt::Rect(static_cast<float>(x), static_cast<float>(y), size.Width, size.Height);
    }

    winrt::Rect SizeExtensions::ToRect(winrt::Size size, Point point) noexcept
    {
        return winrt::Rect(point, size);
    }
}
