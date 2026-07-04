#pragma once

#include "EqualPanel.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct EqualPanel : EqualPanelT<EqualPanel>
    {
        static const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty;

        winrt::Microsoft::UI::Xaml::Controls::Orientation Orientation() const;

        void Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value) const;

        static const wil::single_threaded_property<winrt::DependencyProperty> SpacingProperty;

        double Spacing() const;

        void Spacing(double value);

        EqualPanel();

        winrt::Size MeasureOverride(winrt::Size availableSize);

        winrt::Size ArrangeOverride(winrt::Size finalSize);

        void OnAlignmentChanged(winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp);

        static void OnEqualPanelPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

    private:
        struct UVCoord
        {
        private:
            bool _horizontal;

        public:
            double X;
            double Y;

            UVCoord(double x, double y, winrt::Microsoft::UI::Xaml::Controls::Orientation orientation)
                : _horizontal(orientation == winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal),
                X(x),
                Y(y)
            {
            }

            UVCoord(const winrt::Size& size, winrt::Microsoft::UI::Xaml::Controls::Orientation orientation)
                : UVCoord(size.Width, size.Height, orientation)
            {
            }

            double U() const
            {
                return _horizontal ? X : Y;
            }

            void U(double value)
            {
                if (_horizontal)
                {
                    X = value;
                }
                else
                {
                    Y = value;
                }
            }

            double V() const
            {
                return _horizontal ? Y : X;
            }

            void V(double value)
            {
                if (_horizontal)
                {
                    Y = value;
                }
                else
                {
                    X = value;
                }
            }

            winrt::Windows::Foundation::Size Size() const
            {
                return winrt::Windows::Foundation::Size(static_cast<float>(X), static_cast<float>(Y));
            }
        };

        double _maxItemWidth = 0;
        double _maxItemHeight = 0;
        int _visibleItemsCount = 0;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct EqualPanel : EqualPanelT<EqualPanel, implementation::EqualPanel>
    {
    };
}
