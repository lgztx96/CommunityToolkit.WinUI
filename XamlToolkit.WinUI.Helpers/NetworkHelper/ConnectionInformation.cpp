#include "pch.h"
#include "winrt_module_imports.h"
#include "ConnectionInformation.h"
#if __has_include("ConnectionInformation.g.cpp")
#include "ConnectionInformation.g.cpp"
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Networking::Connectivity;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    // Updates the current object based on profile passed.
    void ConnectionInformation::UpdateConnectionInformation(winrt::Windows::Networking::Connectivity::ConnectionProfile const& profile)
    {
        if (profile == nullptr)
        {
            Reset();

            return;
        }

        _networkNames.clear();

        uint32_t ianaInterfaceType = 0;

        if (auto adapter = profile.NetworkAdapter())
        {
            ianaInterfaceType = adapter.IanaInterfaceType();
        }

        switch (ianaInterfaceType)
        {
            case 6:
                _connectionType = winrt::XamlToolkit::WinUI::Helpers::ConnectionType::Ethernet;
                break;

            case 71:
                _connectionType = winrt::XamlToolkit::WinUI::Helpers::ConnectionType::WiFi;
                break;

            case 243:
            case 244:
                _connectionType = winrt::XamlToolkit::WinUI::Helpers::ConnectionType::Data;
                break;

            default:
                _connectionType = winrt::XamlToolkit::WinUI::Helpers::ConnectionType::Unknown;
                break;
        }

        auto names = profile.GetNetworkNames();
        if (names && names.Size() > 0)
        {
            for (const auto& name : names)
            {
                _networkNames.push_back(name);
            }
        }

        _connectivityLevel = profile.GetNetworkConnectivityLevel();

        switch (_connectivityLevel)
        {
            case winrt::Windows::Networking::Connectivity::NetworkConnectivityLevel::None:
            case winrt::Windows::Networking::Connectivity::NetworkConnectivityLevel::LocalAccess:
                _isInternetAvailable = false;
                break;

            default:
                _isInternetAvailable = true;
                break;
        }

        _connectionCost = profile.GetConnectionCost();
        _signalStrength = profile.GetSignalBars();
    }

    // Resets the current object to default values.
    void ConnectionInformation::Reset()
    {
        _networkNames.clear();

        _connectionType = winrt::XamlToolkit::WinUI::Helpers::ConnectionType::Unknown;
        _connectivityLevel = winrt::Windows::Networking::Connectivity::NetworkConnectivityLevel::None;
        _isInternetAvailable = false;
        _connectionCost = nullptr;
        _signalStrength = nullptr;
    }

    bool ConnectionInformation::IsInternetOnMeteredConnection()
    {
        return _connectionCost && _connectionCost.NetworkCostType() != winrt::Windows::Networking::Connectivity::NetworkCostType::Unrestricted;
    }

    bool ConnectionInformation::IsInternetAvailable() const noexcept { return _isInternetAvailable; }

    winrt::XamlToolkit::WinUI::Helpers::ConnectionType ConnectionInformation::ConnectionType() const noexcept { return _connectionType; }

    winrt::Windows::Networking::Connectivity::NetworkConnectivityLevel ConnectionInformation::ConnectivityLevel() const noexcept { return _connectivityLevel; }

    winrt::Windows::Networking::Connectivity::ConnectionCost ConnectionInformation::ConnectionCost() const noexcept { return _connectionCost; }

    winrt::Windows::Foundation::IReference<uint8_t> ConnectionInformation::SignalStrength() const noexcept { return _signalStrength; }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring> ConnectionInformation::NetworkNames()
    {
        return *this;
    }
}
