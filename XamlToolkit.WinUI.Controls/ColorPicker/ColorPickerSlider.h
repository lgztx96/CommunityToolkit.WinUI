#pragma once

#include "Primitives.ColorPickerSlider.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.h>
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
	using namespace XamlToolkit::WinUI::Controls;
}

import winrt.XamlToolkit.WinUI.Controls;

namespace winrt::XamlToolkit::WinUI::Controls::Primitives::implementation
{
    struct ColorPickerSlider : ColorPickerSliderT<ColorPickerSlider>
    {
        ColorPickerSlider();

        void UpdateColors();

        wil::single_threaded_rw_property<winrt::Color> CheckerBackgroundColor = winrt::Microsoft::UI::ColorHelper::FromArgb(0x19, 0x80, 0x80, 0x80);

        /// <summary>
        /// Measures the size in layout required for child elements and determines a size for the
        /// FrameworkElement-derived class.
        /// </summary>
        /// <remarks>
        ///
        /// Slider has some critical bugs:
        ///
        ///  * https://github.com/microsoft/microsoft-ui-xaml/issues/477
        ///  * https://social.msdn.microsoft.com/Forums/sqlserver/en-US/0d3a2e64-d192-4250-b583-508a02bd75e1/uwp-bug-crash-layoutcycleexception-because-of-slider-under-certain-circumstances?forum=wpdevelop
        ///
        /// </remarks>
        /// <param name="availableSize">The available size that this element can give to child elements.
        /// Infinity can be specified as a value to indicate that the element will size to whatever content
        /// is available.</param>
        /// <returns>The size that this element determines it needs during layout,
        /// based on its calculations of child element sizes.</returns>
        winrt::Size MeasureOverride(winrt::Size availableSize);

        static void OnDependencyPropertyChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        winrt::Color Color() const
        {
            return winrt::unbox_value<winrt::Color>(GetValue(ColorProperty()));
        }

        void Color(winrt::Color const& value)
        {
            SetValue(ColorProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> ColorProperty;

        winrt::XamlToolkit::WinUI::Controls::ColorChannel ColorChannel() const
        {
            return winrt::unbox_value<winrt::XamlToolkit::WinUI::Controls::ColorChannel>(GetValue(ColorChannelProperty()));
        }

        void ColorChannel(winrt::XamlToolkit::WinUI::Controls::ColorChannel const& value)
        {
            SetValue(ColorChannelProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> ColorChannelProperty;

        winrt::XamlToolkit::WinUI::Controls::ColorRepresentation ColorRepresentation() const
        {
            return winrt::unbox_value<winrt::XamlToolkit::WinUI::Controls::ColorRepresentation>(GetValue(ColorRepresentationProperty()));
        }

        void ColorRepresentation(winrt::XamlToolkit::WinUI::Controls::ColorRepresentation const& value)
        {
            SetValue(ColorRepresentationProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> ColorRepresentationProperty;

        winrt::Brush DefaultForeground() const
        {
            return GetValue(DefaultForegroundProperty()).try_as<winrt::Brush>();
        }

        void DefaultForeground(winrt::Brush const& value)
        {
            SetValue(DefaultForegroundProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> DefaultForegroundProperty;

        winrt::XamlToolkit::WinUI::HsvColor HsvColor() const
        {
            return winrt::unbox_value<winrt::XamlToolkit::WinUI::HsvColor>(GetValue(HsvColorProperty()));
        }

        void HsvColor(winrt::XamlToolkit::WinUI::HsvColor const& value)
        {
            SetValue(HsvColorProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> HsvColorProperty;

        bool IsAlphaMaxForced() const
        {
            return winrt::unbox_value<bool>(GetValue(IsAlphaMaxForcedProperty()));
        }

        void IsAlphaMaxForced(bool value)
        {
            SetValue(IsAlphaMaxForcedProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> IsAlphaMaxForcedProperty;

        bool IsAutoUpdatingEnabled() const
        {
            return winrt::unbox_value<bool>(GetValue(IsAutoUpdatingEnabledProperty()));
        }

        void IsAutoUpdatingEnabled(bool value)
        {
            SetValue(IsAutoUpdatingEnabledProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> IsAutoUpdatingEnabledProperty;

        bool IsSaturationValueMaxForced() const
        {
            return winrt::unbox_value<bool>(GetValue(IsSaturationValueMaxForcedProperty()));
        }

        void IsSaturationValueMaxForced(bool value)
        {
            SetValue(IsSaturationValueMaxForcedProperty(), winrt::box_value(value));
        }

        static const wil::single_threaded_property<winrt::DependencyProperty> IsSaturationValueMaxForcedProperty;
        
    private:
        winrt::fire_and_forget UpdateBackground(winrt::XamlToolkit::WinUI::HsvColor color);

        winrt::Size oldSize;
        winrt::Size measuredSize;
        winrt::Size cachedSize;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::Primitives::factory_implementation
{
    struct ColorPickerSlider : ColorPickerSliderT<ColorPickerSlider, implementation::ColorPickerSlider>
    {
    };
}
