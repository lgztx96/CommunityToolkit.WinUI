#pragma once

#include "ConnectedAnimationProperties.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <unordered_map>
#endif

namespace winrt
{
    using namespace ::winrt::Windows::Foundation;
    using namespace ::winrt::Microsoft::UI::Xaml;
    using namespace ::winrt::Microsoft::UI::Xaml::Controls;
    using namespace ::winrt::Microsoft::UI::Xaml::Navigation;
    using namespace ::winrt::Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Connected Animation Helper used with the Connected class.
    /// Attaches to Frame navigation events to handle connected animations.
    /// </summary>
    struct ConnectedAnimationHelper : winrt::implements<ConnectedAnimationHelper, winrt::IInspectable>
    {
        ConnectedAnimationHelper(winrt::Frame const& frame);

        void SetParameterForNextFrameNavigation(winrt::IInspectable const& parameter);

    private:
        std::unordered_map<winrt::hstring, ConnectedAnimationProperties> _previousPageConnectedAnimationProps;
        winrt::IInspectable _nextParameter{ nullptr };
        winrt::Frame::Navigating_revoker _navigatingRevoker;
        winrt::Frame::Navigated_revoker _navigatedRevoker;

        void Frame_Navigating(winrt::IInspectable const& sender, winrt::NavigatingCancelEventArgs const& e);
        void Frame_Navigated(winrt::IInspectable const& sender, winrt::NavigationEventArgs const& e);
        void UseDirectConnectedAnimationConfiguration(winrt::ConnectedAnimation const& animation);
    };
}
