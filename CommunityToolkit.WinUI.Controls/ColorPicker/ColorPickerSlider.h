#pragma once

#include "Primitives.ColorPickerSlider.g.h"
#include <winrt/CommunityToolkit.WinUI.Helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::Primitives::implementation
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::UI;
	using namespace winrt::Microsoft::UI::Xaml::Media;

    struct ColorPickerSlider : ColorPickerSliderT<ColorPickerSlider>
    {
        wil::single_threaded_rw_property<Color> CheckerBackgroundColor = winrt::Windows::UI::ColorHelper::FromArgb(0x19, 0x80, 0x80, 0x80);

    private:
        Size oldSize;
        Size measuredSize;
        Size cachedSize;

    public:
        ColorPickerSlider();

        void UpdateColors();

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
        Size MeasureOverride(Size availableSize);

        static void OnDependencyPropertyChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& args);

        winrt::Windows::UI::Color Color()
        {
            return winrt::unbox_value<winrt::Windows::UI::Color>(GetValue(ColorProperty));
        }

        void Color(winrt::Windows::UI::Color const& value)
        {
            SetValue(ColorProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> ColorProperty =
            DependencyProperty::Register(
                L"Color",
                winrt::xaml_typename<winrt::Windows::UI::Color>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(Colors::White()), &ColorPickerSlider::OnDependencyPropertyChanged));

        winrt::CommunityToolkit::WinUI::Controls::ColorChannel ColorChannel()
        {
            return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::ColorChannel>(GetValue(ColorChannelProperty));
        }

        void ColorChannel(winrt::CommunityToolkit::WinUI::Controls::ColorChannel const& value)
        {
            SetValue(ColorChannelProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> ColorChannelProperty =
            DependencyProperty::Register(
                L"ColorChannel",
                winrt::xaml_typename<winrt::CommunityToolkit::WinUI::Controls::ColorChannel>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(ColorChannel::Channel1), &ColorPickerSlider::OnDependencyPropertyChanged));

        winrt::CommunityToolkit::WinUI::Controls::ColorRepresentation ColorRepresentation()
        {
            return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::ColorRepresentation>(GetValue(ColorRepresentationProperty));
        }

        void ColorRepresentation(winrt::CommunityToolkit::WinUI::Controls::ColorRepresentation const& value)
        {
            SetValue(ColorRepresentationProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> ColorRepresentationProperty =
            DependencyProperty::Register(
                L"ColorRepresentation",
                winrt::xaml_typename<winrt::CommunityToolkit::WinUI::Controls::ColorRepresentation>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(ColorRepresentation::Rgba), &ColorPickerSlider::OnDependencyPropertyChanged));

        Brush DefaultForeground()
        {
            return winrt::unbox_value<Brush>(GetValue(DefaultForegroundProperty));
        }

        void DefaultForeground(Brush const& value)
        {
            SetValue(DefaultForegroundProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> DefaultForegroundProperty =
            DependencyProperty::Register(
                L"DefaultForeground",
                winrt::xaml_typename<Brush>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(nullptr, &ColorPickerSlider::OnDependencyPropertyChanged));

        winrt::CommunityToolkit::WinUI::HsvColor HsvColor()
        {
            return winrt::unbox_value<winrt::CommunityToolkit::WinUI::HsvColor>(GetValue(HsvColorProperty));
        }

        void HsvColor(winrt::CommunityToolkit::WinUI::HsvColor const& value)
        {
            SetValue(HsvColorProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> HsvColorProperty =
            DependencyProperty::Register(
                L"HsvColor",
                winrt::xaml_typename<winrt::CommunityToolkit::WinUI::HsvColor>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(winrt::CommunityToolkit::WinUI::Helpers::ColorHelper::ToHsv(Colors::White())), &ColorPickerSlider::OnDependencyPropertyChanged));

        bool IsAlphaMaxForced()
        {
            return winrt::unbox_value<bool>(GetValue(IsAlphaMaxForcedProperty));
        }

        void IsAlphaMaxForced(bool value)
        {
            SetValue(IsAlphaMaxForcedProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> IsAlphaMaxForcedProperty =
            DependencyProperty::Register(
                L"IsAlphaMaxForced",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(true), &ColorPickerSlider::OnDependencyPropertyChanged));

        bool IsAutoUpdatingEnabled()
        {
            return winrt::unbox_value<bool>(GetValue(IsAutoUpdatingEnabledProperty));
        }

        void IsAutoUpdatingEnabled(bool value)
        {
            SetValue(IsAutoUpdatingEnabledProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> IsAutoUpdatingEnabledProperty =
            DependencyProperty::Register(
                L"IsAutoUpdatingEnabled",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(true), &ColorPickerSlider::OnDependencyPropertyChanged));

        bool IsSaturationValueMaxForced()
        {
            return winrt::unbox_value<bool>(GetValue(IsSaturationValueMaxForcedProperty));
        }

        void IsSaturationValueMaxForced(bool value)
        {
            SetValue(IsSaturationValueMaxForcedProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> IsSaturationValueMaxForcedProperty =
            DependencyProperty::Register(
                L"IsSaturationValueMaxForced",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(winrt::box_value(true), &ColorPickerSlider::OnDependencyPropertyChanged));
        
    private:
        winrt::Windows::Foundation::IAsyncAction UpdateBackground(winrt::CommunityToolkit::WinUI::HsvColor color);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::Primitives::factory_implementation
{
    struct ColorPickerSlider : ColorPickerSliderT<ColorPickerSlider, implementation::ColorPickerSlider>
    {
    };
}
