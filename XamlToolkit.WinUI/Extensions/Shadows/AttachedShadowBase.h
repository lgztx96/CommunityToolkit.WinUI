#pragma once

#include "AttachedShadowBase.g.h"
#include "AttachedShadowElementContext.h"
#include "Extensions/Text/StringExtensions.h"

#ifdef __INTELLISENSE__
#include <unordered_map>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#endif

namespace winrt
{
	using namespace Windows::UI;
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	template<typename T>
	struct WeakElementHash
	{
		size_t operator()(winrt::weak_ref<T> const& wref) const noexcept
		{
			if (auto ref = wref.get())
			{
				return std::hash<void*>{}(winrt::get_abi(ref));
			}

			return 0;
		}
	};

	struct AttachedShadowBase : AttachedShadowBaseT<AttachedShadowBase>
	{
		AttachedShadowBase() = default;

		std::unordered_map<winrt::weak_ref<winrt::FrameworkElement>, XamlToolkit::WinUI::AttachedShadowElementContext, WeakElementHash<winrt::FrameworkElement>> ShadowElementContextTable;

		static void OnDependencyPropertyChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> BlurRadiusProperty =
			winrt::DependencyProperty::Register(
				L"BlurRadius",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(winrt::box_value(12.0), &AttachedShadowBase::OnDependencyPropertyChanged));

		double BlurRadius() const { return winrt::unbox_value<double>(GetValue(BlurRadiusProperty)); }
		void BlurRadius(double value) const { SetValue(BlurRadiusProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ColorProperty =
			winrt::DependencyProperty::Register(
				L"Color",
				winrt::xaml_typename<winrt::Color>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(winrt::box_value(winrt::Microsoft::UI::Colors::Black()), &AttachedShadowBase::OnDependencyPropertyChanged));

		winrt::Color Color() const { return winrt::unbox_value<winrt::Color>(GetValue(ColorProperty)); }
		void Color(winrt::Color value) const { SetValue(ColorProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::DependencyProperty> OffsetProperty =
			winrt::DependencyProperty::Register(
				L"Offset",
				winrt::xaml_typename<winrt::hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(winrt::box_value(L""), &AttachedShadowBase::OnDependencyPropertyChanged));

		winrt::hstring Offset() const { return winrt::unbox_value<winrt::hstring>(GetValue(OffsetProperty)); }
		void Offset(winrt::hstring value) const { SetValue(OffsetProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::DependencyProperty> OpacityProperty =
			winrt::DependencyProperty::Register(L"Opacity",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(winrt::box_value(1.0), &AttachedShadowBase::OnDependencyPropertyChanged));

		double Opacity() const { return winrt::unbox_value<double>(GetValue(OpacityProperty)); }
		void Opacity(double value) const { SetValue(OpacityProperty, winrt::box_value(value)); }

		void ConnectElement(winrt::FrameworkElement const& element);

		void DisconnectElement(winrt::FrameworkElement const& element);

		winrt::XamlToolkit::WinUI::AttachedShadowElementContext GetElementContext(winrt::FrameworkElement const& element);

		virtual bool SupportsOnSizeChangedEvent() const noexcept;

		virtual void OnSizeChanged(
			winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context,
			winrt::Size newSize,
			winrt::Size previousSize);

		virtual void OnElementContextInitialized(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		virtual void OnElementContextUninitialized(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		winrt::IVector<winrt::XamlToolkit::WinUI::AttachedShadowElementContext> EnumerateElementContexts();

		virtual void SetElementChildVisual(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		void CallPropertyChangedForEachElement(winrt::DependencyProperty const& property, winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);

		virtual winrt::CompositionBrush GetShadowMask(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		virtual winrt::CompositionClip GetShadowClip(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		void UpdateShadowMask(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		void UpdateShadowClip(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		virtual void OnPropertyChanged(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context, winrt::DependencyProperty const& property, winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct AttachedShadowBase : AttachedShadowBaseT<AttachedShadowBase, implementation::AttachedShadowBase>
	{
	};
}
