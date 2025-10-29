#pragma once

#include "RadialGauge.g.h"
#include <numbers>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/CommunityToolkit.WinUI.Helpers.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt::Microsoft::UI::Xaml::Hosting;
	using namespace winrt::Microsoft::UI::Composition;
	using namespace winrt::CommunityToolkit::WinUI::Helpers;

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
        static inline winrt::CommunityToolkit::WinUI::Helpers::ThemeListener ThemeListener;
		SolidColorBrush _needleBrush{ nullptr };
		SolidColorBrush _needleBorderBrush{ nullptr };
		Brush _trailBrush{ nullptr };
		Brush _scaleBrush{ nullptr };
		SolidColorBrush _scaleTickBrush{ nullptr };
		SolidColorBrush _tickBrush{ nullptr };
		Brush _foreground{ nullptr };

		double _normalizedMinAngle;
		double _normalizedMaxAngle;

		Compositor _compositor{ nullptr };
		ContainerVisual _root{ nullptr };
		CompositionSpriteShape _needle{ nullptr };

        UIElement::PointerReleased_revoker _pointerReleasedRevoker;
        Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;
        FrameworkElement::Unloaded_revoker _unloadedRevoker;

        winrt::event_token _tappedToken;
        winrt::event_token _manipulationDeltaToken;

        winrt::event_token _themeChangedToken;

	public:
		RadialGauge();

		void OnApplyTemplate();

		double NormalizedMinAngle() const { return  _normalizedMinAngle; }

		double NormalizedMaxAngle() const { return _normalizedMaxAngle; }

		AutomationPeer OnCreateAutomationPeer();

		void OnMinimumChanged(double oldMinimum, double newMinimum);

		void OnMaximumChanged(double oldMaximum, double newMaximum);

		void OnValueChanged(double oldValue, double newValue);

        static void OnInteractivityChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnUnitChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnScaleChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnFaceChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

#pragma region Properties
        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsInteractiveProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"IsInteractive",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(true), &RadialGauge::OnInteractivityChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleWidthProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ScaleWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(12.0), &RadialGauge::OnScaleChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> StepSizeProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"StepSize",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.0) });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> NeedleBrushProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"NeedleBrush",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> NeedleBorderBrushProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"NeedleBorderBrush",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> UnitProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Unit",
                winrt::xaml_typename<winrt::hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L""), &RadialGauge::OnUnitChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TrailBrushProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"TrailBrush",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::Brush>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleBrushProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ScaleBrush",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::Brush>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleTickBrushProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ScaleTickBrush",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TickBrushProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"TickBrush",
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ValueStringFormatProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ValueStringFormat",
                winrt::xaml_typename<winrt::hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L"{:.0f}"), [](auto& s, auto&) 
                { 
                    auto radialGauge = s.template try_as<class_type>();
                    auto self = winrt::get_self<RadialGauge>(radialGauge)->get_strong();
                    self->OnValueChanged(s);
                }});

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> NeedleLengthProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"NeedleLength",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(58.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> NeedleBorderThicknessProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"NeedleBorderThickness",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(1.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> NeedleWidthProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"NeedleWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(5.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScalePaddingProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ScalePadding",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleTickWidthProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ScaleTickWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleTickLengthProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ScaleTickLength",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(12.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleTickCornerRadiusProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ScaleTickCornerRadius",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(2.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TickSpacingProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"TickSpacing",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(15), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TickWidthProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"TickWidth",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(2.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TickLengthProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"TickLength",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(6.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TickPaddingProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"TickPadding",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(24.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TickCornerRadiusProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"TickCornerRadius",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(2.0), &RadialGauge::OnFaceChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MinAngleProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"MinAngle",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(-150), &RadialGauge::OnScaleChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> MaxAngleProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"MaxAngle",
                winrt::xaml_typename<int32_t>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(150), &RadialGauge::OnScaleChanged });

        static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ValueAngleProperty =
            winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"ValueAngle",
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

        bool IsInteractive()
        {
            return winrt::unbox_value<bool>(GetValue(IsInteractiveProperty));
        }

        void IsInteractive(bool value)
        {
            SetValue(IsInteractiveProperty, winrt::box_value(value));
        }

        double ScaleWidth()
        {
            return winrt::unbox_value<double>(GetValue(ScaleWidthProperty));
        }

        void ScaleWidth(double value)
        {
            SetValue(ScaleWidthProperty, winrt::box_value(value));
        }

        double StepSize()
        {
            return winrt::unbox_value<double>(GetValue(StepSizeProperty));
        }

        void StepSize(double value)
        {
            SetValue(StepSizeProperty, winrt::box_value(value));
        }

        winrt::Microsoft::UI::Xaml::Media::SolidColorBrush NeedleBrush()
        {
            return GetValue(NeedleBrushProperty).try_as<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>();
        }

        void NeedleBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush const& value)
        {
            SetValue(NeedleBrushProperty, value);
        }

        winrt::Microsoft::UI::Xaml::Media::SolidColorBrush NeedleBorderBrush()
        {
            return GetValue(NeedleBorderBrushProperty).try_as<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>();
        }

        void NeedleBorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush const& value)
        {
            SetValue(NeedleBorderBrushProperty, value);
        }

        winrt::hstring Unit()
        {
            return winrt::unbox_value<winrt::hstring>(GetValue(UnitProperty));
        }

        void Unit(winrt::hstring const& value)
        {
            SetValue(UnitProperty, winrt::box_value(value));
        }

        winrt::Microsoft::UI::Xaml::Media::Brush TrailBrush()
        {
            return GetValue(TrailBrushProperty).try_as<winrt::Microsoft::UI::Xaml::Media::Brush>();
        }

        void TrailBrush(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
        {
            SetValue(TrailBrushProperty, value);
        }

        winrt::Microsoft::UI::Xaml::Media::Brush ScaleBrush()
        {
            return GetValue(ScaleBrushProperty).try_as<winrt::Microsoft::UI::Xaml::Media::Brush>();
        }

        void ScaleBrush(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
        {
            SetValue(ScaleBrushProperty, value);
        }

        winrt::Microsoft::UI::Xaml::Media::SolidColorBrush ScaleTickBrush()
        {
            return GetValue(ScaleTickBrushProperty).try_as<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>();
        }

        void ScaleTickBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush const& value)
        {
            SetValue(ScaleTickBrushProperty, value);
        }

        winrt::Microsoft::UI::Xaml::Media::SolidColorBrush TickBrush()
        {
            return GetValue(TickBrushProperty).try_as<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>();
        }

        void TickBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush const& value)
        {
            SetValue(TickBrushProperty, value);
        }

        winrt::hstring ValueStringFormat()
        {
            return winrt::unbox_value<winrt::hstring>(GetValue(ValueStringFormatProperty));
        }

        void ValueStringFormat(winrt::hstring const& value)
        {
            SetValue(ValueStringFormatProperty, winrt::box_value(value));
        }

        double NeedleLength()
        {
            return winrt::unbox_value<double>(GetValue(NeedleLengthProperty));
        }

        void NeedleLength(double value)
        {
            SetValue(NeedleLengthProperty, winrt::box_value(value));
        }

        double NeedleBorderThickness()
        {
            return winrt::unbox_value<double>(GetValue(NeedleBorderThicknessProperty));
        }

        void NeedleBorderThickness(double value)
        {
            SetValue(NeedleBorderThicknessProperty, winrt::box_value(value));
        }

        double NeedleWidth()
        {
            return winrt::unbox_value<double>(GetValue(NeedleWidthProperty));
        }

        void NeedleWidth(double value)
        {
            SetValue(NeedleWidthProperty, winrt::box_value(value));
        }

        double ScalePadding()
        {
            return winrt::unbox_value<double>(GetValue(ScalePaddingProperty));
        }

        void ScalePadding(double value)
        {
            SetValue(ScalePaddingProperty, winrt::box_value(value));
        }

        double ScaleTickWidth()
        {
            return winrt::unbox_value<double>(GetValue(ScaleTickWidthProperty));
        }

        void ScaleTickWidth(double value)
        {
            SetValue(ScaleTickWidthProperty, winrt::box_value(value));
        }

        double ScaleTickLength()
        {
            return winrt::unbox_value<double>(GetValue(ScaleTickLengthProperty));
        }

        void ScaleTickLength(double value)
        {
            SetValue(ScaleTickLengthProperty, winrt::box_value(value));
        }

        double ScaleTickCornerRadius()
        {
            return winrt::unbox_value<double>(GetValue(ScaleTickCornerRadiusProperty));
        }

        void ScaleTickCornerRadius(double value)
        {
            SetValue(ScaleTickCornerRadiusProperty, winrt::box_value(value));
        }

        int32_t TickSpacing()
        {
            return winrt::unbox_value<int32_t>(GetValue(TickSpacingProperty));
        }
        void TickSpacing(int32_t value)
        {
            SetValue(TickSpacingProperty, winrt::box_value(value));
        }

        double TickWidth()
        {
            return winrt::unbox_value<double>(GetValue(TickWidthProperty));
        }

        void TickWidth(double value)
        {
            SetValue(TickWidthProperty, winrt::box_value(value));
        }

        double TickLength()
        {
            return winrt::unbox_value<double>(GetValue(TickLengthProperty));
        }

        void TickLength(double value)
        {
            SetValue(TickLengthProperty, winrt::box_value(value));
        }

        double TickPadding()
        {
            return winrt::unbox_value<double>(GetValue(TickPaddingProperty));
        }

        void TickPadding(double value)
        {
            SetValue(TickPaddingProperty, winrt::box_value(value));
        }

        double TickCornerRadius()
        {
            return winrt::unbox_value<double>(GetValue(TickCornerRadiusProperty));
        }

        void TickCornerRadius(double value)
        {
            SetValue(TickCornerRadiusProperty, winrt::box_value(value));
        }

        int32_t MinAngle()
        {
            return winrt::unbox_value<int32_t>(GetValue(MinAngleProperty));
        }

        void MinAngle(int32_t value)
        {
            SetValue(MinAngleProperty, winrt::box_value(value));
        }

        int32_t MaxAngle()
        {
            return winrt::unbox_value<int32_t>(GetValue(MaxAngleProperty));
        }

        void MaxAngle(int32_t value)
        {
            SetValue(MaxAngleProperty, winrt::box_value(value));
        }

        double ValueAngle()
        {
            return winrt::unbox_value<double>(GetValue(ValueAngleProperty));
        }

        void ValueAngle(double value)
        {
            SetValue(ValueAngleProperty, winrt::box_value(value));
        }

#pragma endregion

	private:
		void RadialGauge_Unloaded(IInspectable const& sender, RoutedEventArgs const& e);

		void ThemeListener_ThemeChanged(struct ThemeListener const& sender, IInspectable const& e);

		void RadialGauge_IsEnabledChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& e);

		static void OnValueChanged(DependencyObject const& d);

		static void OnInteractivityChanged(DependencyObject const& d);

		static void OnScaleChanged(DependencyObject const& d);

		static void OnFaceChanged(DependencyObject const& d);

		void OnColorsChanged();

		void OnEnabledChanged();

		static void OnUnitChanged(DependencyObject const& d);

		void ClearBrush(Brush const& brush, DependencyProperty const& prop);

		void RestoreBrush(Brush const& source, DependencyProperty const& prop);

		void UpdateNormalizedAngles();

		void SetGaugeValueFromPoint(Point const& p);

		Point ScalePoint(double angle, double middleOfScale);

		double ValueToAngle(double value);

		double Mod(double number, double divider);

		double RoundToMultiple(double number, double multiple);

#pragma region Input
        void RadialGauge_ManipulationDelta(IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e);

        void RadialGauge_Tapped(IInspectable const& sender, TappedRoutedEventArgs const& e);

        void RadialGauge_PointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e);

        void SetKeyboardAccelerators();

        void AddKeyboardAccelerator(
            VirtualKeyModifiers keyModifiers,
            VirtualKey key,
            TypedEventHandler<KeyboardAccelerator, KeyboardAcceleratorInvokedEventArgs> const& handler);
#pragma endregion
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct RadialGauge : RadialGaugeT<RadialGauge, implementation::RadialGauge>
	{
	};
}
