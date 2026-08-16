#pragma once

#include "NetworkConnectionStateTrigger.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Dispatching;
    using namespace Windows::Networking::Connectivity;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct NetworkConnectionStateTrigger : NetworkConnectionStateTriggerT<NetworkConnectionStateTrigger>
    {
        NetworkConnectionStateTrigger();
        ~NetworkConnectionStateTrigger();

        winrt::XamlToolkit::WinUI::ConnectionState ConnectionState() const { return winrt::unbox_value<winrt::XamlToolkit::WinUI::ConnectionState>(GetValue(ConnectionStateProperty)); }
        void ConnectionState(winrt::XamlToolkit::WinUI::ConnectionState const& value) const { SetValue(ConnectionStateProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ConnectionStateProperty;

    private:
        static void OnConnectionStatePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        void UpdateState();

        void OnNetworkStatusChanged(winrt::IInspectable const& sender);

        winrt::event_token _networkStatusChangedToken;
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct NetworkConnectionStateTrigger : NetworkConnectionStateTriggerT<NetworkConnectionStateTrigger, implementation::NetworkConnectionStateTrigger>
    {
    };
}
