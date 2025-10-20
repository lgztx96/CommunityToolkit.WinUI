#pragma once

#include "AttachedShadowBase.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "AttachedShadowElementContext.h"
#include "Extensions/Text/StringExtensions.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Hosting;

	struct AttachedShadowBase : AttachedShadowBaseT<AttachedShadowBase>
	{
		AttachedShadowBase() = default;

		std::vector<CommunityToolkit::WinUI::AttachedShadowElementContext> ShadowElementContextTable;

		static void OnDependencyPropertyChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<DependencyProperty> BlurRadiusProperty =
			DependencyProperty::Register(L"BlurRadius",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(12.0), OnDependencyPropertyChanged));

		double BlurRadius() const { return winrt::unbox_value<double>(GetValue(BlurRadiusProperty)); }
		void BlurRadius(double value) const { SetValue(BlurRadiusProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> ColorProperty =
			DependencyProperty::Register(L"Color",
				winrt::xaml_typename<Windows::UI::Color>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(Microsoft::UI::Colors::Black()), OnDependencyPropertyChanged));

		Windows::UI::Color Color() const { return winrt::unbox_value<Windows::UI::Color>(GetValue(ColorProperty)); }
		void Color(Windows::UI::Color value) const { SetValue(ColorProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> OffsetProperty =
			DependencyProperty::Register(L"Offset",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value({}), OnDependencyPropertyChanged));

		winrt::hstring Offset() const { return winrt::unbox_value<winrt::hstring>(GetValue(OffsetProperty)); }
		void Offset(winrt::hstring value) const { SetValue(OffsetProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> OpacityProperty =
			DependencyProperty::Register(L"Opacity",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(1.0), OnDependencyPropertyChanged));

		double Opacity() const { return winrt::unbox_value<double>(GetValue(OpacityProperty)); }
		void Opacity(double value) const { SetValue(OpacityProperty, winrt::box_value(value)); }

		void ConnectElement(FrameworkElement const& element);

		void DisconnectElement(FrameworkElement const& element);

		CommunityToolkit::WinUI::AttachedShadowElementContext GetElementContext(FrameworkElement const& element);

		bool SupportsOnSizeChangedEvent() const noexcept;

		void OnSizeChanged(
			CommunityToolkit::WinUI::AttachedShadowElementContext const& context,
			Windows::Foundation::Size newSize,
			Windows::Foundation::Size previousSize);

		void OnElementContextInitialized(CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		void OnElementContextUninitialized(CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		Windows::Foundation::Collections::IVector<CommunityToolkit::WinUI::AttachedShadowElementContext> EnumerateElementContexts();

		void SetElementChildVisual(CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		void CallPropertyChangedForEachElement(DependencyProperty const& property, IInspectable const& oldValue, IInspectable const& newValue);

		CompositionBrush GetShadowMask(CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		CompositionClip GetShadowClip(CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		void UpdateShadowMask(CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		void UpdateShadowClip(CommunityToolkit::WinUI::AttachedShadowElementContext const& context);

		void OnPropertyChanged(CommunityToolkit::WinUI::AttachedShadowElementContext const& context, DependencyProperty const& property, IInspectable const& oldValue, IInspectable const& newValue);
	};
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
	struct AttachedShadowBase : AttachedShadowBaseT<AttachedShadowBase, implementation::AttachedShadowBase>
	{
	};
}
