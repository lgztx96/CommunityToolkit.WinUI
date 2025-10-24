#pragma once

#include "VisualExtensions.g.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
	using namespace winrt::Microsoft::UI::Composition;

	struct VisualExtensions : VisualExtensionsT<VisualExtensions>
	{
		static Visual GetVisual(UIElement const& element);

		static winrt::hstring GetAnchorPoint(DependencyObject const& obj);

		static void SetAnchorPoint(DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetCenterPoint(DependencyObject const& obj);

		static void SetCenterPoint(DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetOffset(DependencyObject const& obj);

		static void SetOffset(DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetTranslation(DependencyObject const& obj);

		static void SetTranslation(DependencyObject const& obj, winrt::hstring const& value);

		static double GetOpacity(DependencyObject const& obj);

		static void SetOpacity(DependencyObject const& obj, double value);

		static double GetRotationAngle(DependencyObject const& obj);

		static void SetRotationAngle(DependencyObject const& obj, double value);

		static double GetRotationAngleInDegrees(DependencyObject const& obj);

		static void SetRotationAngleInDegrees(DependencyObject const& obj, double value);

		static winrt::hstring GetRotationAxis(DependencyObject const& obj);

		static void SetRotationAxis(DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetScale(DependencyObject const& obj);

		static void SetScale(DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetSize(DependencyObject const& obj);

		static void SetSize(DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetNormalizedCenterPoint(DependencyObject const& obj);

		static void SetNormalizedCenterPoint(DependencyObject const& obj, winrt::hstring const& value);

		static void OnAnchorPointChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnCenterPointChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnOffsetChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnTranslationChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnOpacityChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnRotationAngleChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnRotationAngleInDegreesChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnRotationAxisChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnScaleChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnSizeChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		static void OnNormalizedCenterPointChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		// AnchorPoint
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AnchorPointProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"AnchorPoint",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &VisualExtensions::OnAnchorPointChanged }
			);

		// CenterPoint
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CenterPointProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"CenterPoint",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &VisualExtensions::OnCenterPointChanged }
			);

		// Offset
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OffsetProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"Offset",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &VisualExtensions::OnOffsetChanged }
			);

		// Translation
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> TranslationProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"Translation",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &VisualExtensions::OnTranslationChanged }
			);

		// Opacity
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> OpacityProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"Opacity",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()), &VisualExtensions::OnOpacityChanged }
			);

		// RotationAngle
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RotationAngleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"RotationAngle",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()), &VisualExtensions::OnRotationAngleChanged }
			);

		// RotationAngleInDegrees
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RotationAngleInDegreesProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"RotationAngleInDegrees",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()), &VisualExtensions::OnRotationAngleInDegreesChanged }
			);

		// RotationAxis
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RotationAxisProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"RotationAxis",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &VisualExtensions::OnRotationAxisChanged }
			);

		// Scale
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScaleProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"Scale",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &VisualExtensions::OnScaleChanged }
			);

		// Size
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> SizeProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"Size",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &VisualExtensions::OnSizeChanged }
			);

		// NormalizedCenterPoint
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> NormalizedCenterPointProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"NormalizedCenterPoint",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(false), &VisualExtensions::OnNormalizedCenterPointChanged }
			);
	private:
		static winrt::hstring GetAnchorPointForElement(UIElement const& element);

		static void SetAnchorPointForElement(winrt::hstring const& value, UIElement const& element);

		static winrt::hstring GetCenterPointForElement(UIElement const& element);

		static void SetCenterPointForElement(winrt::hstring const& value, UIElement const& element);

		static winrt::hstring GetOffsetForElement(UIElement const& element);

		static void SetOffsetForElement(winrt::hstring const& value, UIElement const& element);

		static winrt::hstring GetTranslationForElement(UIElement const& element);

		static void SetTranslationForElement(winrt::hstring const& value, UIElement const& element);

		static double GetOpacityForElement(UIElement const& element);

		static void SetOpacityForElement(double value, UIElement const& element);

		static double GetRotationAngleForElement(UIElement const& element);

		static void SetRotationAngleForElement(double value, UIElement const& element);

		static double GetRotationAngleInDegreesForElement(UIElement const& element);

		static void SetRotationAngleInDegreesForElement(double value, UIElement const& element);

		static winrt::hstring GetRotationAxisForElement(UIElement const& element);

		static void SetRotationAxisForElement(winrt::hstring const& value, UIElement const& element);

		static winrt::hstring GetScaleForElement(UIElement const& element);

		static void SetScaleForElement(winrt::hstring const& value, UIElement const& element);

		static winrt::hstring GetSizeForElement(UIElement const& element);

		static void SetSizeForElement(winrt::hstring const& value, UIElement const& element);
	};
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
	struct VisualExtensions : VisualExtensionsT<VisualExtensions, implementation::VisualExtensions>
	{
	};
}
