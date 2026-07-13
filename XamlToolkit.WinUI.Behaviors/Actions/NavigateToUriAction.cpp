#include "pch.h"
#include "winrt_module_imports.h"
#include "NavigateToUriAction.h"
#if __has_include("NavigateToUriAction.g.cpp")
#include "NavigateToUriAction.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <winrt/Windows.System.h>
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> NavigateToUriAction::NavigateUriProperty =
        winrt::DependencyProperty::Register(
            L"NavigateUri",
            winrt::xaml_typename<winrt::Uri>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    winrt::IInspectable NavigateToUriAction::Execute(
       [[maybe_unused]] winrt::IInspectable const& sender,
       [[maybe_unused]] winrt::IInspectable const& parameter) const
    {
        if (auto uri = NavigateUri())
        {
            winrt::Windows::System::Launcher::LaunchUriAsync(uri);
            return winrt::box_value(true);
        }

        throw winrt::hresult_invalid_argument(L"NavigateUri");
    }
}
