#pragma once

#include "ContrastHelper.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::UI;
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ContrastHelper
    {
#pragma region Properties
        static const wil::single_threaded_property<winrt::DependencyProperty> OpponentProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> MinRatioProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ContrastRatioProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> OriginalColorProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> OriginalContrastRatioProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> CallbackObjectProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> CallbackProperty;

        static winrt::Color GetOpponent(winrt::DependencyObject const& obj);

        static void SetOpponent(winrt::DependencyObject const& obj, winrt::Color const& value);

        static double GetMinRatio(winrt::DependencyObject const& obj);

        static void SetMinRatio(winrt::DependencyObject const& obj, double value);

        static double GetContrastRatio(winrt::DependencyObject const& obj);

        static void SetContrastRatio(winrt::DependencyObject const& obj, double value);

        static winrt::Color GetOriginalColor(winrt::DependencyObject const& obj);

        static void SetOriginalColor(winrt::DependencyObject const& obj, winrt::Color const& value);

        static double GetOriginalContrastRatio(winrt::DependencyObject const& obj);

        static void SetOriginalContrastRatio(winrt::DependencyObject const& obj, double value);

        static winrt::DependencyObject GetCallbackObject(winrt::DependencyObject const& obj);

        static void SetCallbackObject(winrt::DependencyObject const& obj, winrt::DependencyObject const& dp);

        static int64_t GetCallback(winrt::DependencyObject const& obj);

        static void SetCallback(winrt::DependencyObject const& obj, int64_t value);

#pragma endregion

#pragma region Callbacks
        static void OnOpponentChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnMinRatioChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnOriginalChangedFromSource(winrt::DependencyObject const& obj, winrt::DependencyObject const& sender, winrt::DependencyProperty const& prop);

        static void SubscribeToUpdates(winrt::DependencyObject const& d);

        static void SubscribeToBrushUpdates(winrt::DependencyObject const& d, winrt::SolidColorBrush const& brush);
#pragma endregion

    private:
        // When the helper is updating the color, this flag is set to avoid feedback loops
        // It has no threading issues since all updates are on the UI thread
        static inline bool _selfUpdate = false;

        static void ApplyContrastCheck(winrt::DependencyObject const& d);

        static winrt::SolidColorBrush FindBrush(winrt::DependencyObject const& d, winrt::DependencyProperty* dp);

        static void UpdateContrastedProperties(winrt::DependencyObject const& d, winrt::Color color);

    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ContrastHelper : ContrastHelperT<ContrastHelper, implementation::ContrastHelper>
    {
    };
}
