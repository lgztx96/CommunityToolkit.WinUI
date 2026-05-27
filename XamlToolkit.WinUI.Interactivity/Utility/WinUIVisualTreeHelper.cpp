#include "pch.h"
#include "winrt_module_imports.h"
#include "WinUIVisualTreeHelper.h"

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    winrt::DependencyObject WinUIVisualTreeHelper::GetParent(winrt::DependencyObject const& reference) const
    {
        return winrt::Microsoft::UI::Xaml::Media::VisualTreeHelper::GetParent(reference);
    }
}
