#pragma once

#include "Shimmer.g.h"
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml::Hosting;
    using namespace Microsoft::UI::Xaml::Shapes;

    struct Shimmer : ShimmerT<Shimmer>
    {
        private:
            static constexpr float InitialStartPointX = -7.92f;
            static constexpr std::wstring_view PART_Shape = L"Shape";

        ExpressionAnimation _sizeAnimation{ nullptr };
        Vector2KeyFrameAnimation _gradientStartPointAnimation{ nullptr };
        Vector2KeyFrameAnimation _gradientEndPointAnimation{ nullptr };
        CompositionColorGradientStop _gradientStop1{ nullptr };
        CompositionColorGradientStop _gradientStop2{ nullptr };
        CompositionColorGradientStop _gradientStop3{ nullptr };
        CompositionColorGradientStop _gradientStop4{ nullptr };
        CompositionRoundedRectangleGeometry _rectangleGeometry{ nullptr };
        ShapeVisual _shapeVisual{ nullptr };
        CompositionLinearGradientBrush _shimmerMaskGradient{nullptr};
        Border _shape{ nullptr };

        bool _initialized;
        bool _animationStarted;

        winrt::event_token _actualThemeChangedToken;
    public:

        Shimmer();

        void OnApplyTemplate();

        static void PropertyChanged(DependencyObject const& s, DependencyPropertyChangedEventArgs e);

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> DurationProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Duration",
                winrt::xaml_typename<winrt::Windows::Foundation::TimeSpan>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{
                    winrt::box_value(winrt::Windows::Foundation::TimeSpan{ std::chrono::milliseconds(1600) }),
                    &Shimmer::PropertyChanged
                }
            );

        winrt::Windows::Foundation::TimeSpan Duration() const
        {
            return winrt::unbox_value<winrt::Windows::Foundation::TimeSpan>(GetValue(DurationProperty));
        }

        void Duration(winrt::Windows::Foundation::TimeSpan const& value)
        {
            SetValue(DurationProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsActiveProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"IsActive",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{
                    winrt::box_value(true),
                    &Shimmer::PropertyChanged
                }
            );

        bool IsActive() const
        {
            return winrt::unbox_value<bool>(GetValue(IsActiveProperty));
        }

        void IsActive(bool value)
        {
            SetValue(IsActiveProperty, winrt::box_value(value));
        }

    private:
        void OnLoaded(IInspectable const& sender, RoutedEventArgs const& e);

        void OnUnloaded(IInspectable const& sender, RoutedEventArgs const& e);

        void OnActualThemeChanged(FrameworkElement const& sender, IInspectable const& args);

        bool TryInitializationResource();

        void SetGradientAndStops();

        void SetGradientStopColorsByTheme();

        void TryStartAnimation();

        void StopAnimation();
    };
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
    struct Shimmer : ShimmerT<Shimmer, implementation::Shimmer>
    {
    };
}
