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
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct ContrastHelper : ContrastHelperT<ContrastHelper>
    {
        ContrastHelper() = default;
        
#pragma region Properties
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> OpponentProperty;

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> MinRatioProperty;

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ContrastRatioProperty;

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> OriginalColorProperty;

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> OriginalContrastRatioProperty;

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> CallbackObjectProperty;

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> CallbackProperty;

        static Windows::UI::Color GetOpponent(Microsoft::UI::Xaml::DependencyObject const& obj);

        static void SetOpponent(Microsoft::UI::Xaml::DependencyObject const& obj, Windows::UI::Color const& value);

        static double GetMinRatio(Microsoft::UI::Xaml::DependencyObject const& obj);

        static void SetMinRatio(Microsoft::UI::Xaml::DependencyObject const& obj, double value);

        static double GetContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj);

        static void SetContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj, double value);

        static Windows::UI::Color GetOriginalColor(Microsoft::UI::Xaml::DependencyObject const& obj);

        static void SetOriginalColor(Microsoft::UI::Xaml::DependencyObject const& obj, Windows::UI::Color const& value);

        static double GetOriginalContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj);

        static void SetOriginalContrastRatio(Microsoft::UI::Xaml::DependencyObject const& obj, double value);

        static DependencyObject GetCallbackObject(DependencyObject const& obj);

        static void SetCallbackObject(DependencyObject const& obj, DependencyObject const& dp);

        static int64_t GetCallback(DependencyObject const& obj);

        static void SetCallback(DependencyObject const& obj, int64_t value);

#pragma endregion

#pragma region Callbacks
        static void OnOpponentChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnMinRatioChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnOriginalChangedFromSource(DependencyObject const& obj, DependencyObject const& sender, DependencyProperty const& prop);

        static void SubscribeToUpdates(DependencyObject const& d);

        static void SubscribeToBrushUpdates(DependencyObject const& d, SolidColorBrush const& brush);
#pragma endregion


    private:
        // When the helper is updating the color, this flag is set to avoid feedback loops
        // It has no threading issues since all updates are on the UI thread
        static inline bool _selfUpdate = false;

        static void ApplyContrastCheck(DependencyObject const& d);

        static SolidColorBrush FindBrush(DependencyObject const& d, DependencyProperty* dp);

        static void UpdateContrastedProperties(DependencyObject const& d, winrt::Windows::UI::Color color);

    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct ContrastHelper : ContrastHelperT<ContrastHelper, implementation::ContrastHelper>
    {
    };
}
