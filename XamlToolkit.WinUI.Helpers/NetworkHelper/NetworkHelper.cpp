#include "pch.h"
#include "winrt_module_imports.h"
#include "NetworkHelper.h"
#if __has_include("NetworkHelper.g.cpp")
#include "NetworkHelper.g.cpp"
#endif
#include "ConnectionInformation.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Networking::Connectivity;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    // Initializes a new instance of the NetworkHelper class.
    NetworkHelper::NetworkHelper()
    {
        _connectionInformation = winrt::make<winrt::XamlToolkit::WinUI::Helpers::implementation::ConnectionInformation>();

        UpdateConnectionInformation();

        _networkStatusChangedToken = NetworkInformation::NetworkStatusChanged({ this, &NetworkHelper::OnNetworkStatusChanged });
    }

    NetworkHelper::~NetworkHelper()
    {
        NetworkInformation::NetworkStatusChanged(_networkStatusChangedToken);
    }

    // Gets public singleton property.
    winrt::XamlToolkit::WinUI::Helpers::NetworkHelper NetworkHelper::Instance()
    {
        static auto instance = winrt::make<NetworkHelper>();
        return instance;
    }

    // Gets instance of ConnectionInformation.
    winrt::XamlToolkit::WinUI::Helpers::ConnectionInformation NetworkHelper::ConnectionInformation()
    {
        return _connectionInformation;
    }

    // Checks the current connection information and raises NetworkChanged if needed.
    void NetworkHelper::UpdateConnectionInformation()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        try
        {
            _connectionInformation.UpdateConnectionInformation(NetworkInformation::GetInternetConnectionProfile());

            NetworkChanged.invoke(*this, nullptr);
        }
        catch (...)
        {
            _connectionInformation.Reset();
        }
    }

    // Invokes UpdateConnectionInformation when the current network status changes.
    void NetworkHelper::OnNetworkStatusChanged([[maybe_unused]] winrt::IInspectable const& sender)
    {
        UpdateConnectionInformation();
    }
}
