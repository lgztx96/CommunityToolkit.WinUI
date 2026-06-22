#include "pch.h"
#include "winrt_module_imports.h"
#include "SurfaceDialOptions.h"
#if __has_include("SurfaceDialOptions.g.cpp")
#include "SurfaceDialOptions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    winrt::XamlToolkit::WinUI::SurfaceDialOptions SurfaceDialOptions::Default()
    {
        static auto instance = winrt::make<implementation::SurfaceDialOptions>();
        return instance;
    }
}
