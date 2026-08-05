#pragma once

#include "VisualExtensions.g.h"

#ifdef __INTELLISENSE__
#include <limits>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Hosting;
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct VisualExtensions
	{
		static winrt::Visual GetVisual(winrt::UIElement const& element);

		static winrt::hstring GetAnchorPoint(winrt::DependencyObject const& obj);

		static void SetAnchorPoint(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetCenterPoint(winrt::DependencyObject const& obj);

		static void SetCenterPoint(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetOffset(winrt::DependencyObject const& obj);

		static void SetOffset(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetTranslation(winrt::DependencyObject const& obj);

		static void SetTranslation(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static double GetOpacity(winrt::DependencyObject const& obj);

		static void SetOpacity(winrt::DependencyObject const& obj, double value);

		static double GetRotationAngle(winrt::DependencyObject const& obj);

		static void SetRotationAngle(winrt::DependencyObject const& obj, double value);

		static double GetRotationAngleInDegrees(winrt::DependencyObject const& obj);

		static void SetRotationAngleInDegrees(winrt::DependencyObject const& obj, double value);

		static winrt::hstring GetRotationAxis(winrt::DependencyObject const& obj);

		static void SetRotationAxis(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetScale(winrt::DependencyObject const& obj);

		static void SetScale(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetSize(winrt::DependencyObject const& obj);

		static void SetSize(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static winrt::hstring GetNormalizedCenterPoint(winrt::DependencyObject const& obj);

		static void SetNormalizedCenterPoint(winrt::DependencyObject const& obj, winrt::hstring const& value);

		static void OnAnchorPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnOffsetChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnTranslationChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnOpacityChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnRotationAngleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnRotationAngleInDegreesChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnRotationAxisChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnScaleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnSizeChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnNormalizedCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		// AnchorPoint
		static inline const wil::single_threaded_property<winrt::DependencyProperty> AnchorPointProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"AnchorPoint",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ nullptr, &VisualExtensions::OnAnchorPointChanged }
			);

		// CenterPoint
		static inline const wil::single_threaded_property<winrt::DependencyProperty> CenterPointProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"CenterPoint",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ nullptr, &VisualExtensions::OnCenterPointChanged }
			);

		// Offset
		static inline const wil::single_threaded_property<winrt::DependencyProperty> OffsetProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"Offset",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ nullptr, &VisualExtensions::OnOffsetChanged }
			);

		// Translation
		static inline const wil::single_threaded_property<winrt::DependencyProperty> TranslationProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"Translation",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ nullptr, &VisualExtensions::OnTranslationChanged }
			);

		// Opacity
		static inline const wil::single_threaded_property<winrt::DependencyProperty> OpacityProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"Opacity",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()), &VisualExtensions::OnOpacityChanged }
			);

		// RotationAngle
		static inline const wil::single_threaded_property<winrt::DependencyProperty> RotationAngleProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"RotationAngle",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()), &VisualExtensions::OnRotationAngleChanged }
			);

		// RotationAngleInDegrees
		static inline const wil::single_threaded_property<winrt::DependencyProperty> RotationAngleInDegreesProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"RotationAngleInDegrees",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ winrt::box_value(std::numeric_limits<double>::quiet_NaN()), &VisualExtensions::OnRotationAngleInDegreesChanged }
			);

		// RotationAxis
		static inline const wil::single_threaded_property<winrt::DependencyProperty> RotationAxisProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"RotationAxis",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ nullptr, &VisualExtensions::OnRotationAxisChanged }
			);

		// Scale
		static inline const wil::single_threaded_property<winrt::DependencyProperty> ScaleProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"Scale",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ nullptr, &VisualExtensions::OnScaleChanged }
			);

		// Size
		static inline const wil::single_threaded_property<winrt::DependencyProperty> SizeProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"Size",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ nullptr, &VisualExtensions::OnSizeChanged }
			);

		// NormalizedCenterPoint
		static inline const wil::single_threaded_property<winrt::DependencyProperty> NormalizedCenterPointProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"NormalizedCenterPoint",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::VisualExtensions>(),
				winrt::PropertyMetadata{ winrt::box_value(false), &VisualExtensions::OnNormalizedCenterPointChanged }
			);
	private:
		static winrt::hstring GetAnchorPointForElement(winrt::UIElement const& element);

		static void SetAnchorPointForElement(winrt::hstring const& value, winrt::UIElement const& element);

		static winrt::hstring GetCenterPointForElement(winrt::UIElement const& element);

		static void SetCenterPointForElement(winrt::hstring const& value, winrt::UIElement const& element);

		static winrt::hstring GetOffsetForElement(winrt::UIElement const& element);

		static void SetOffsetForElement(winrt::hstring const& value, winrt::UIElement const& element);

		static winrt::hstring GetTranslationForElement(winrt::UIElement const& element);

		static void SetTranslationForElement(winrt::hstring const& value, winrt::UIElement const& element);

		static double GetOpacityForElement(winrt::UIElement const& element);

		static void SetOpacityForElement(double value, winrt::UIElement const& element);

		static double GetRotationAngleForElement(winrt::UIElement const& element);

		static void SetRotationAngleForElement(double value, winrt::UIElement const& element);

		static double GetRotationAngleInDegreesForElement(winrt::UIElement const& element);

		static void SetRotationAngleInDegreesForElement(double value, winrt::UIElement const& element);

		static winrt::hstring GetRotationAxisForElement(winrt::UIElement const& element);

		static void SetRotationAxisForElement(winrt::hstring const& value, winrt::UIElement const& element);

		static winrt::hstring GetScaleForElement(winrt::UIElement const& element);

		static void SetScaleForElement(winrt::hstring const& value, winrt::UIElement const& element);

		static winrt::hstring GetSizeForElement(winrt::UIElement const& element);

		static void SetSizeForElement(winrt::hstring const& value, winrt::UIElement const& element);
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct VisualExtensions : VisualExtensionsT<VisualExtensions, implementation::VisualExtensions>
	{
	};
}
