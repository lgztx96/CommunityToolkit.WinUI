#pragma once

#include "ConnectedAnimationProperties.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <map>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Navigation;
    using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Connected Animation Helper used with the Connected class.
    /// Attaches to Frame navigation events to handle connected animations.
    /// </summary>
    class ConnectedAnimationHelper
    {
    public:
        ConnectedAnimationHelper(winrt::Frame const& frame);

        void SetParameterForNextFrameNavigation(winrt::IInspectable const& parameter);

    private:
        std::map<winrt::hstring, ConnectedAnimationProperties> previousPageConnectedAnimationProps;
        winrt::IInspectable nextParameter{ nullptr };
        winrt::Frame frame{ nullptr };
        winrt::event_token navigatingToken{};
        winrt::event_token navigatedToken{};

        void Frame_Navigating(winrt::IInspectable const& sender, winrt::NavigatingCancelEventArgs const& e);
        void Frame_Navigated(winrt::IInspectable const& sender, winrt::NavigationEventArgs const& e);
        void UseDirectConnectedAnimationConfiguration(winrt::ConnectedAnimation const& animation);
    };
}
