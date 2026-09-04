#pragma once

#include "NetworkHelper.g.h"

#ifdef __INTELLISENSE__
#include <mutex>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Networking::Connectivity;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct NetworkHelper : NetworkHelperT<NetworkHelper>
    {
        NetworkHelper();

        ~NetworkHelper();

        static winrt::XamlToolkit::WinUI::Helpers::NetworkHelper Instance();

        winrt::XamlToolkit::WinUI::Helpers::ConnectionInformation ConnectionInformation() const noexcept;

        wil::untyped_event<winrt::Windows::Foundation::IInspectable> NetworkChanged;

    private:
        // Checks the current connection information and raises NetworkChanged if needed.
        void UpdateConnectionInformation();

        // Invokes UpdateConnectionInformation when the current network status changes.
        void OnNetworkStatusChanged(winrt::IInspectable const& sender);

        std::mutex _mutex;
        winrt::XamlToolkit::WinUI::Helpers::ConnectionInformation _connectionInformation{ nullptr };
        winrt::event_token _networkStatusChangedToken{};
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct NetworkHelper : NetworkHelperT<NetworkHelper, implementation::NetworkHelper>
    {
    };
}
