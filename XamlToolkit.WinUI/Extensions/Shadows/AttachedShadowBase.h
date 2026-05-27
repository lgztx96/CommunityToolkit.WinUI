#pragma once

#include "AttachedShadowBase.g.h"
#include "AttachedShadowElementContext.h"
#include "Extensions/Text/StringExtensions.h"

#ifdef __INTELLISENSE__
#include <unordered_map>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.h>
#endif

namespace winrt
{
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

		std::unordered_map<winrt::weak_ref<FrameworkElement>, XamlToolkit::WinUI::AttachedShadowElementContext, WeakElementHash<FrameworkElement>> ShadowElementContextTable;

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
				PropertyMetadata(winrt::box_value(L""), OnDependencyPropertyChanged));

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

		XamlToolkit::WinUI::AttachedShadowElementContext GetElementContext(FrameworkElement const& element);

		virtual bool SupportsOnSizeChangedEvent() const noexcept;

		virtual void OnSizeChanged(
			XamlToolkit::WinUI::AttachedShadowElementContext const& context,
			Windows::Foundation::Size newSize,
			Windows::Foundation::Size previousSize);

		virtual void OnElementContextInitialized(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		virtual void OnElementContextUninitialized(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		Windows::Foundation::Collections::IVector<XamlToolkit::WinUI::AttachedShadowElementContext> EnumerateElementContexts();

		virtual void SetElementChildVisual(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		void CallPropertyChangedForEachElement(DependencyProperty const& property, IInspectable const& oldValue, IInspectable const& newValue);

		virtual CompositionBrush GetShadowMask(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		virtual CompositionClip GetShadowClip(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		void UpdateShadowMask(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		void UpdateShadowClip(XamlToolkit::WinUI::AttachedShadowElementContext const& context);

		virtual void OnPropertyChanged(XamlToolkit::WinUI::AttachedShadowElementContext const& context, DependencyProperty const& property, IInspectable const& oldValue, IInspectable const& newValue);
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct AttachedShadowBase : AttachedShadowBaseT<AttachedShadowBase, implementation::AttachedShadowBase>
	{
	};
}
