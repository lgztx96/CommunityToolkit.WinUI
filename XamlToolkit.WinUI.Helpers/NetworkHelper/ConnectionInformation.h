#pragma once

#include "ConnectionInformation.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.Connectivity.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Networking::Connectivity;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct ConnectionInformation : ConnectionInformationT<ConnectionInformation>
    {
        ConnectionInformation() = default;

        void UpdateConnectionInformation(winrt::Windows::Networking::Connectivity::ConnectionProfile const& profile);
        void Reset();

        bool IsInternetOnMeteredConnection();
        bool IsInternetAvailable();
        winrt::XamlToolkit::WinUI::Helpers::ConnectionType ConnectionType() const noexcept;
        winrt::Windows::Networking::Connectivity::NetworkConnectivityLevel ConnectivityLevel() const noexcept;
        winrt::Windows::Networking::Connectivity::ConnectionCost ConnectionCost() const noexcept;
        winrt::Windows::Foundation::IReference<uint8_t> SignalStrength() const noexcept;
        winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring> NetworkNames();

    private:
        std::vector<winrt::hstring> _networkNames;
        winrt::XamlToolkit::WinUI::Helpers::ConnectionType _connectionType{ winrt::XamlToolkit::WinUI::Helpers::ConnectionType::Unknown };
        winrt::Windows::Networking::Connectivity::NetworkConnectivityLevel _connectivityLevel{ winrt::Windows::Networking::Connectivity::NetworkConnectivityLevel::None };
        bool _isInternetAvailable{ false };
        winrt::Windows::Networking::Connectivity::ConnectionCost _connectionCost{ nullptr };
        winrt::Windows::Foundation::IReference<uint8_t> _signalStrength{ nullptr };
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct ConnectionInformation : ConnectionInformationT<ConnectionInformation, implementation::ConnectionInformation>
    {
    };
}
