#include "pch.h"
#include "winrt_module_imports.h"
#include "PointExtensions.h"
#if __has_include("PointExtensions.g.cpp")
#include "PointExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    winrt::Rect PointExtensions::ToRect(winrt::Point point, double width, double height) noexcept
    {
        return winrt::Rect(point.X, point.Y, width, height);
    }

    winrt::Rect PointExtensions::ToRect(winrt::Point point, winrt::Point end) noexcept
    {
        return winrt::RectHelper::FromPoints(point, end);
    }

    winrt::Rect PointExtensions::ToRect(winrt::Point point, winrt::Size size) noexcept
    {
        return winrt::Rect(point, size);
    }

    winrt::float3 PointExtensions::ToVector3(winrt::Point point) noexcept
    {
        return winrt::float3(point.X, point.Y, 0.0f);
    }
}
