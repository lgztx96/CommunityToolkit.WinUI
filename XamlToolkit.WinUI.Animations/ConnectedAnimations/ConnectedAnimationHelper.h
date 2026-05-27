#pragma once

#include "ConnectedAnimationProperties.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <map>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Connected Animation Helper used with the Connected class.
    /// Attaches to Frame navigation events to handle connected animations.
    /// </summary>
    class ConnectedAnimationHelper
    {
    public:
        ConnectedAnimationHelper(Microsoft::UI::Xaml::Controls::Frame const& frame);

        void SetParameterForNextFrameNavigation(winrt::Windows::Foundation::IInspectable const& parameter);

    private:
        std::map<winrt::hstring, ConnectedAnimationProperties> previousPageConnectedAnimationProps;
        winrt::Windows::Foundation::IInspectable nextParameter{ nullptr };
        Microsoft::UI::Xaml::Controls::Frame frame{ nullptr };
        winrt::event_token navigatingToken{};
        winrt::event_token navigatedToken{};

        void Frame_Navigating(winrt::Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Navigation::NavigatingCancelEventArgs const& e);
        void Frame_Navigated(winrt::Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
        void UseDirectConnectedAnimationConfiguration(Microsoft::UI::Xaml::Media::Animation::ConnectedAnimation const& animation);
    };
}
