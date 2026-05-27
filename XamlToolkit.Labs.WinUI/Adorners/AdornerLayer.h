#pragma once

#include "AdornerLayer.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt 
{ 
	using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct AdornerLayer : AdornerLayerT<AdornerLayer>
	{
		AdornerLayer();

		static void RemoveAdorner(winrt::XamlToolkit::Labs::WinUI::AdornerLayer const& layer, UIElement const& adornerXaml);

		static winrt::Windows::Foundation::IAsyncOperation<winrt::XamlToolkit::Labs::WinUI::AdornerLayer> GetAdornerLayerAsync(FrameworkElement const& adornedElement);

		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> XamlProperty;

		static winrt::Microsoft::UI::Xaml::UIElement GetXaml(winrt::Microsoft::UI::Xaml::FrameworkElement const& obj)
		{
			return obj.GetValue(XamlProperty()).try_as<winrt::Microsoft::UI::Xaml::UIElement>();
		}

		static void SetXaml(winrt::Microsoft::UI::Xaml::FrameworkElement const& obj, winrt::Microsoft::UI::Xaml::UIElement const& value)
		{
			obj.SetValue(XamlProperty(), value);
		}

	private:
		void AdornerLayer_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, SizeChangedEventArgs const& e);

		static winrt::Windows::Foundation::IAsyncAction OnXamlPropertyChanged(DependencyObject const& dependencyObject, DependencyPropertyChangedEventArgs const& args);

		static winrt::Windows::Foundation::IAsyncAction XamlPropertyFrameworkElement_Loaded(winrt::Windows::Foundation::IInspectable const& sender, RoutedEventArgs const& e);

		static void AttachAdorner(winrt::XamlToolkit::Labs::WinUI::AdornerLayer const& layer, FrameworkElement const& adornedElement, UIElement const& adornerXaml);
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct AdornerLayer : AdornerLayerT<AdornerLayer, implementation::AdornerLayer>
	{
	};
}
