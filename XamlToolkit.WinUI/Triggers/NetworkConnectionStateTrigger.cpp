#include "pch.h"
#include "winrt_module_imports.h"
#include "NetworkConnectionStateTrigger.h"
#if __has_include("NetworkConnectionStateTrigger.g.cpp")
#include "NetworkConnectionStateTrigger.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    NetworkConnectionStateTrigger::NetworkConnectionStateTrigger()
    {
        auto weakThis = get_weak();

        _networkStatusChangedToken = winrt::NetworkInformation::NetworkStatusChanged({ get_weak(), [this](winrt::IInspectable const& sender)
        {
            OnNetworkStatusChanged(sender);
        } });

        UpdateState();
    }

    NetworkConnectionStateTrigger::~NetworkConnectionStateTrigger()
    {
        winrt::NetworkInformation::NetworkStatusChanged(_networkStatusChangedToken);
    }

    const wil::single_threaded_property<winrt::DependencyProperty> NetworkConnectionStateTrigger::ConnectionStateProperty =
        winrt::DependencyProperty::Register(
            L"ConnectionState",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::ConnectionState>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::ConnectionState::Connected), &NetworkConnectionStateTrigger::OnConnectionStatePropertyChanged));

    void NetworkConnectionStateTrigger::OnConnectionStatePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        winrt::get_self<NetworkConnectionStateTrigger>(d.as<class_type>())->UpdateState();
    }

    void NetworkConnectionStateTrigger::OnNetworkStatusChanged([[maybe_unused]] winrt::IInspectable const& sender)
    {
        auto weakThis = get_weak();
        DispatcherQueue().TryEnqueue(winrt::DispatcherQueuePriority::Normal, [weakThis]()
        {
            if (auto strongThis = weakThis.get())
            {
                strongThis->UpdateState();
            }
        });
    }

    void NetworkConnectionStateTrigger::UpdateState()
    {
        bool isConnected = false;
        
        if (auto profile = winrt::NetworkInformation::GetInternetConnectionProfile())
        {
            isConnected = profile.GetNetworkConnectivityLevel() == winrt::NetworkConnectivityLevel::InternetAccess;
        }

        auto connectionState = ConnectionState();
        SetActive((isConnected && connectionState == winrt::XamlToolkit::WinUI::ConnectionState::Connected) ||
                  (!isConnected && connectionState == winrt::XamlToolkit::WinUI::ConnectionState::Disconnected));
    }
}
