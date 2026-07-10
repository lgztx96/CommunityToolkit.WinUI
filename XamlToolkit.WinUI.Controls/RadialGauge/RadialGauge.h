#pragma once

#include "RadialGauge.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#include <numbers>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::System;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Xaml::Hosting;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Automation;
	using namespace Microsoft::UI::Xaml::Automation::Peers;
    using namespace XamlToolkit::WinUI::Helpers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct RadialGauge : RadialGaugeT<RadialGauge>
	{
		// States
	private:
		static constexpr std::wstring_view NormalState = L"Normal";
		static constexpr std::wstring_view DisabledState = L"Disabled";
		static constexpr std::wstring_view CommonStates = L"CommonStates";

		// Template Parts.
		static constexpr std::wstring_view ContainerPartName = L"PART_Container";
		static constexpr std::wstring_view ScalePartName = L"PART_Scale";
		static constexpr std::wstring_view TrailPartName = L"PART_Trail";
		static constexpr std::wstring_view ValueTextPartName = L"PART_ValueText";
		static constexpr std::wstring_view UnitTextPartName = L"PART_UnitText";

		// For convenience.
		static constexpr double Degrees2Radians = std::numbers::pi / 180;

		// High-contrast accessibility
        static inline winrt::XamlToolkit::WinUI::Helpers::ThemeListener ThemeListener;
        winrt::SolidColorBrush _needleBrush{ nullptr };
        winrt::SolidColorBrush _needleBorderBrush{ nullptr };
        winrt::Brush _trailBrush{ nullptr };
        winrt::Brush _scaleBrush{ nullptr };
        winrt::SolidColorBrush _scaleTickBrush{ nullptr };
        winrt::SolidColorBrush _tickBrush{ nullptr };
        winrt::Brush _foreground{ nullptr };

		double _normalizedMinAngle;
		double _normalizedMaxAngle;

        winrt::Compositor _compositor{ nullptr };
        winrt::ContainerVisual _root{ nullptr };
        winrt::CompositionSpriteShape _needle{ nullptr };

        winrt::UIElement::PointerReleased_revoker _pointerReleasedRevoker;
        winrt::Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;
        winrt::FrameworkElement::Unloaded_revoker _unloadedRevoker;

        winrt::event_token _tappedToken;
        winrt::event_token _manipulationDeltaToken;

        winrt::event_token _themeChangedToken;

	public:
		RadialGauge();

		void OnApplyTemplate();

		double NormalizedMinAngle() const { return  _normalizedMinAngle; }

		double NormalizedMaxAngle() const { return _normalizedMaxAngle; }

        winrt::AutomationPeer OnCreateAutomationPeer();

		void OnMinimumChanged(double oldMinimum, double newMinimum);

		void OnMaximumChanged(double oldMaximum, double newMaximum);

		void OnValueChanged(double oldValue, double newValue);

        static void OnInteractivityChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnUnitChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnScaleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnFaceChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

#pragma region Properties
        static inline const wil::single_threaded_property<winrt::DependencyProperty> IsInteractiveProperty =
            winrt::DependencyProperty::Register(
                L"IsInteractive",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(true), &RadialGauge::OnInteractivityChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleWidthProperty =
            winrt::DependencyProperty::Register(
                L"ScaleWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(12.0), &RadialGauge::OnScaleChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> StepSizeProperty =
            winrt::DependencyProperty::Register(
                L"StepSize",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(0.0) });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> NeedleBrushProperty =
            winrt::DependencyProperty::Register(
                L"NeedleBrush",
                winrt::xaml_typename<winrt::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> NeedleBorderBrushProperty =
            winrt::DependencyProperty::Register(
                L"NeedleBorderBrush",
                winrt::xaml_typename<winrt::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> UnitProperty =
            winrt::DependencyProperty::Register(
                L"Unit",
                winrt::xaml_typename<winrt::hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(L""), &RadialGauge::OnUnitChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TrailBrushProperty =
            winrt::DependencyProperty::Register(
                L"TrailBrush",
                winrt::xaml_typename<winrt::Brush>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleBrushProperty =
            winrt::DependencyProperty::Register(
                L"ScaleBrush",
                winrt::xaml_typename<winrt::Brush>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleTickBrushProperty =
            winrt::DependencyProperty::Register(
                L"ScaleTickBrush",
                winrt::xaml_typename<winrt::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TickBrushProperty =
            winrt::DependencyProperty::Register(
                L"TickBrush",
                winrt::xaml_typename<winrt::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ValueStringFormatProperty =
            winrt::DependencyProperty::Register(
                L"ValueStringFormat",
                winrt::xaml_typename<winrt::hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(L"{:.0f}"), [](auto& s, auto&) 
                { 
                    auto radialGauge = s.template try_as<class_type>();
                    auto self = winrt::get_self<RadialGauge>(radialGauge)->get_strong();
                    self->OnValueChanged(s);
                }});

        static inline const wil::single_threaded_property<winrt::DependencyProperty> NeedleLengthProperty =
            winrt::DependencyProperty::Register(
                L"NeedleLength",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(58.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> NeedleBorderThicknessProperty =
            winrt::DependencyProperty::Register(
                L"NeedleBorderThickness",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(1.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> NeedleWidthProperty =
            winrt::DependencyProperty::Register(
                L"NeedleWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(5.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ScalePaddingProperty =
            winrt::DependencyProperty::Register(
                L"ScalePadding",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(0.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleTickWidthProperty =
            winrt::DependencyProperty::Register(
                L"ScaleTickWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(0.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleTickLengthProperty =
            winrt::DependencyProperty::Register(
                L"ScaleTickLength",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(12.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleTickCornerRadiusProperty =
            winrt::DependencyProperty::Register(
                L"ScaleTickCornerRadius",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(2.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TickSpacingProperty =
            winrt::DependencyProperty::Register(
                L"TickSpacing",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(15), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TickWidthProperty =
            winrt::DependencyProperty::Register(
                L"TickWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(2.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TickLengthProperty =
            winrt::DependencyProperty::Register(
                L"TickLength",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(6.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TickPaddingProperty =
            winrt::DependencyProperty::Register(
                L"TickPadding",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(24.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TickCornerRadiusProperty =
            winrt::DependencyProperty::Register(
                L"TickCornerRadius",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(2.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> MinAngleProperty =
            winrt::DependencyProperty::Register(
                L"MinAngle",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(-150), &RadialGauge::OnScaleChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> MaxAngleProperty =
            winrt::DependencyProperty::Register(
                L"MaxAngle",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ winrt::box_value(150), &RadialGauge::OnScaleChanged });

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ValueAngleProperty =
            winrt::DependencyProperty::Register(
                L"ValueAngle",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata{ nullptr });

        bool IsInteractive() const
        {
            return winrt::unbox_value<bool>(GetValue(IsInteractiveProperty));
        }

        void IsInteractive(bool value)
        {
            SetValue(IsInteractiveProperty, winrt::box_value(value));
        }

        double ScaleWidth() const
        {
            return winrt::unbox_value<double>(GetValue(ScaleWidthProperty));
        }

        void ScaleWidth(double value)
        {
            SetValue(ScaleWidthProperty, winrt::box_value(value));
        }

        double StepSize() const
        {
            return winrt::unbox_value<double>(GetValue(StepSizeProperty));
        }

        void StepSize(double value)
        {
            SetValue(StepSizeProperty, winrt::box_value(value));
        }

        winrt::SolidColorBrush NeedleBrush() const
        {
            return GetValue(NeedleBrushProperty).try_as<winrt::SolidColorBrush>();
        }

        void NeedleBrush(winrt::SolidColorBrush const& value)
        {
            SetValue(NeedleBrushProperty, value);
        }

        winrt::SolidColorBrush NeedleBorderBrush() const
        {
            return GetValue(NeedleBorderBrushProperty).try_as<winrt::SolidColorBrush>();
        }

        void NeedleBorderBrush(winrt::SolidColorBrush const& value)
        {
            SetValue(NeedleBorderBrushProperty, value);
        }

        winrt::hstring Unit() const
        {
            return winrt::unbox_value<winrt::hstring>(GetValue(UnitProperty));
        }

        void Unit(winrt::hstring const& value)
        {
            SetValue(UnitProperty, winrt::box_value(value));
        }

        winrt::Brush TrailBrush() const
        {
            return GetValue(TrailBrushProperty).try_as<winrt::Brush>();
        }

        void TrailBrush(winrt::Brush const& value)
        {
            SetValue(TrailBrushProperty, value);
        }

        winrt::Brush ScaleBrush() const
        {
            return GetValue(ScaleBrushProperty).try_as<winrt::Brush>();
        }

        void ScaleBrush(winrt::Brush const& value)
        {
            SetValue(ScaleBrushProperty, value);
        }

        winrt::SolidColorBrush ScaleTickBrush() const
        {
            return GetValue(ScaleTickBrushProperty).try_as<winrt::SolidColorBrush>();
        }

        void ScaleTickBrush(winrt::SolidColorBrush const& value)
        {
            SetValue(ScaleTickBrushProperty, value);
        }

        winrt::SolidColorBrush TickBrush() const
        {
            return GetValue(TickBrushProperty).try_as<winrt::SolidColorBrush>();
        }

        void TickBrush(winrt::SolidColorBrush const& value)
        {
            SetValue(TickBrushProperty, value);
        }

        winrt::hstring ValueStringFormat() const
        {
            return winrt::unbox_value<winrt::hstring>(GetValue(ValueStringFormatProperty));
        }

        void ValueStringFormat(winrt::hstring const& value)
        {
            SetValue(ValueStringFormatProperty, winrt::box_value(value));
        }

        double NeedleLength() const
        {
            return winrt::unbox_value<double>(GetValue(NeedleLengthProperty));
        }

        void NeedleLength(double value)
        {
            SetValue(NeedleLengthProperty, winrt::box_value(value));
        }

        double NeedleBorderThickness() const
        {
            return winrt::unbox_value<double>(GetValue(NeedleBorderThicknessProperty));
        }

        void NeedleBorderThickness(double value)
        {
            SetValue(NeedleBorderThicknessProperty, winrt::box_value(value));
        }

        double NeedleWidth() const
        {
            return winrt::unbox_value<double>(GetValue(NeedleWidthProperty));
        }

        void NeedleWidth(double value)
        {
            SetValue(NeedleWidthProperty, winrt::box_value(value));
        }

        double ScalePadding() const
        {
            return winrt::unbox_value<double>(GetValue(ScalePaddingProperty));
        }

        void ScalePadding(double value)
        {
            SetValue(ScalePaddingProperty, winrt::box_value(value));
        }

        double ScaleTickWidth() const
        {
            return winrt::unbox_value<double>(GetValue(ScaleTickWidthProperty));
        }

        void ScaleTickWidth(double value)
        {
            SetValue(ScaleTickWidthProperty, winrt::box_value(value));
        }

        double ScaleTickLength() const
        {
            return winrt::unbox_value<double>(GetValue(ScaleTickLengthProperty));
        }

        void ScaleTickLength(double value)
        {
            SetValue(ScaleTickLengthProperty, winrt::box_value(value));
        }

        double ScaleTickCornerRadius() const
        {
            return winrt::unbox_value<double>(GetValue(ScaleTickCornerRadiusProperty));
        }

        void ScaleTickCornerRadius(double value)
        {
            SetValue(ScaleTickCornerRadiusProperty, winrt::box_value(value));
        }

        int32_t TickSpacing() const
        {
            return winrt::unbox_value<int32_t>(GetValue(TickSpacingProperty));
        }
        void TickSpacing(int32_t value)
        {
            SetValue(TickSpacingProperty, winrt::box_value(value));
        }

        double TickWidth() const
        {
            return winrt::unbox_value<double>(GetValue(TickWidthProperty));
        }

        void TickWidth(double value)
        {
            SetValue(TickWidthProperty, winrt::box_value(value));
        }

        double TickLength() const
        {
            return winrt::unbox_value<double>(GetValue(TickLengthProperty));
        }

        void TickLength(double value)
        {
            SetValue(TickLengthProperty, winrt::box_value(value));
        }

        double TickPadding() const
        {
            return winrt::unbox_value<double>(GetValue(TickPaddingProperty));
        }

        void TickPadding(double value)
        {
            SetValue(TickPaddingProperty, winrt::box_value(value));
        }

        double TickCornerRadius() const
        {
            return winrt::unbox_value<double>(GetValue(TickCornerRadiusProperty));
        }

        void TickCornerRadius(double value)
        {
            SetValue(TickCornerRadiusProperty, winrt::box_value(value));
        }

        int32_t MinAngle() const
        {
            return winrt::unbox_value<int32_t>(GetValue(MinAngleProperty));
        }

        void MinAngle(int32_t value)
        {
            SetValue(MinAngleProperty, winrt::box_value(value));
        }

        int32_t MaxAngle() const
        {
            return winrt::unbox_value<int32_t>(GetValue(MaxAngleProperty));
        }

        void MaxAngle(int32_t value)
        {
            SetValue(MaxAngleProperty, winrt::box_value(value));
        }

        double ValueAngle() const
        {
            return winrt::unbox_value<double>(GetValue(ValueAngleProperty));
        }

        void ValueAngle(double value)
        {
            SetValue(ValueAngleProperty, winrt::box_value(value));
        }

#pragma endregion

	private:
		void RadialGauge_Unloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void ThemeListener_ThemeChanged(struct ThemeListener const& sender);

		void RadialGauge_IsEnabledChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnValueChanged(winrt::DependencyObject const& d);

		static void OnInteractivityChanged(winrt::DependencyObject const& d);

		static void OnScaleChanged(winrt::DependencyObject const& d);

		static void OnFaceChanged(winrt::DependencyObject const& d);

		void OnColorsChanged();

		void OnEnabledChanged();

		static void OnUnitChanged(winrt::DependencyObject const& d);

		void ClearBrush(winrt::Brush const& brush, winrt::DependencyProperty const& prop);

		void RestoreBrush(winrt::Brush const& source, winrt::DependencyProperty const& prop);

		void UpdateNormalizedAngles();

		void SetGaugeValueFromPoint(winrt::Point const& p);

        winrt::Point ScalePoint(double angle, double middleOfScale);

		double ValueToAngle(double value);

		double Mod(double number, double divider);

		double RoundToMultiple(double number, double multiple);

#pragma region Input
        void RadialGauge_ManipulationDelta(winrt::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e);

        void RadialGauge_Tapped(winrt::IInspectable const& sender, winrt::TappedRoutedEventArgs const& e);

        void RadialGauge_PointerReleased(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void SetKeyboardAccelerators();

        void AddKeyboardAccelerator(
            winrt::VirtualKeyModifiers keyModifiers,
            winrt::VirtualKey key,
            winrt::TypedEventHandler<winrt::KeyboardAccelerator, winrt::KeyboardAcceleratorInvokedEventArgs> const& handler);
#pragma endregion
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct RadialGauge : RadialGaugeT<RadialGauge, implementation::RadialGauge>
	{
	};
}
