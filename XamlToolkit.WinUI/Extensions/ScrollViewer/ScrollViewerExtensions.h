#pragma once

#include "ScrollViewerExtensions.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#else
import winrt.Windows.Foundation;
import winrt.Windows.UI.Xaml.Interop;
import winrt.Microsoft.UI.Xaml;
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::Xaml::Interop;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
    struct ScrollViewerExtensions
    {
        /// <summary>
        /// Attached <see cref="DependencyProperty"/> for binding a <see cref="Thickness"/> for the horizontal <see cref="ScrollBar"/> of a <see cref="ScrollViewer"/>
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> HorizontalScrollBarMarginProperty;

        /// <summary>
        /// Attached <see cref="DependencyProperty"/> for binding a <see cref="Thickness"/> for the vertical <see cref="ScrollBar"/> of a <see cref="ScrollViewer"/>
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> VerticalScrollBarMarginProperty;

        /// <summary>
        /// Gets the <see cref="Thickness"/> associated with the specified vertical <see cref="ScrollBar"/> of a <see cref="ScrollViewer"/>
        /// </summary>
        /// <param name="obj">The <see cref="FrameworkElement"/> to get the associated <see cref="Thickness"/> from</param>
        /// <returns>The <see cref="Thickness"/> associated with the <see cref="FrameworkElement"/></returns>
        static winrt::Thickness GetVerticalScrollBarMargin(winrt::FrameworkElement const& obj)
        {
            return winrt::unbox_value<winrt::Thickness>(obj.GetValue(VerticalScrollBarMarginProperty()));
        }

        /// <summary>
        /// Sets the <see cref="Thickness"/> associated with the specified vertical <see cref="ScrollBar"/> of a <see cref="ScrollViewer"/>
        /// </summary>
        /// <param name="obj">The <see cref="FrameworkElement"/> to associate the <see cref="Thickness"/> with</param>
        /// <param name="value">The <see cref="Thickness"/> for binding to the <see cref="FrameworkElement"/></param>
        static void SetVerticalScrollBarMargin(winrt::FrameworkElement const& obj, winrt::Thickness const& value)
        {
            obj.SetValue(VerticalScrollBarMarginProperty(), winrt::box_value(value));
        }

        /// <summary>
        /// Gets the <see cref="Thickness"/> associated with the specified horizontal <see cref="ScrollBar"/> of a <see cref="ScrollViewer"/>
        /// </summary>
        /// <param name="obj">The <see cref="FrameworkElement"/> to get the associated <see cref="Thickness"/> from</param>
        /// <returns>The <see cref="Thickness"/> associated with the <see cref="FrameworkElement"/></returns>
        static winrt::Thickness GetHorizontalScrollBarMargin(winrt::FrameworkElement const& obj)
        {
            return winrt::unbox_value<winrt::Thickness>(obj.GetValue(HorizontalScrollBarMarginProperty()));
        }

        /// <summary>
        /// Sets the <see cref="Thickness"/> associated with the specified horizontal <see cref="ScrollBar"/> of a <see cref="ScrollViewer"/>
        /// </summary>
        /// <param name="obj">The <see cref="FrameworkElement"/> to associate the <see cref="Thickness"/> with</param>
        /// <param name="value">The <see cref="Thickness"/> for binding to the <see cref="FrameworkElement"/></param>
        static void SetHorizontalScrollBarMargin(winrt::FrameworkElement const& obj, winrt::Thickness const& value)
        {
            obj.SetValue(HorizontalScrollBarMarginProperty(), winrt::box_value(value));
        }

    private:
        static void OnHorizontalScrollBarMarginPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        static bool ChangeHorizontalScrollBarMarginProperty(winrt::FrameworkElement const& sender);

        static void OnVerticalScrollBarMarginPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        static bool ChangeVerticalScrollBarMarginProperty(winrt::FrameworkElement const& sender);
    };
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
    struct ScrollViewerExtensions : ScrollViewerExtensionsT<ScrollViewerExtensions, implementation::ScrollViewerExtensions>
    {
    };
}
