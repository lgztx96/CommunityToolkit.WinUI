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
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct AdornerLayer : AdornerLayerT<AdornerLayer>
	{
		AdornerLayer();

		static void RemoveAdorner(winrt::XamlToolkit::Labs::WinUI::AdornerLayer const& layer, winrt::UIElement const& adornerXaml);

		static winrt::IAsyncOperation<winrt::XamlToolkit::Labs::WinUI::AdornerLayer> GetAdornerLayerAsync(winrt::FrameworkElement const& adornedElement);

		static const wil::single_threaded_property<winrt::DependencyProperty> XamlProperty;

		static winrt::UIElement GetXaml(winrt::FrameworkElement const& obj)
		{
			return obj.GetValue(XamlProperty()).try_as<winrt::UIElement>();
		}

		static void SetXaml(winrt::FrameworkElement const& obj, winrt::UIElement const& value)
		{
			obj.SetValue(XamlProperty(), value);
		}

	private:
		void AdornerLayer_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		static winrt::IAsyncAction OnXamlPropertyChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args);

		static winrt::IAsyncAction XamlPropertyFrameworkElement_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		static void AttachAdorner(winrt::XamlToolkit::Labs::WinUI::AdornerLayer const& layer, winrt::FrameworkElement const& adornedElement, winrt::UIElement const& adornerXaml);
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct AdornerLayer : AdornerLayerT<AdornerLayer, implementation::AdornerLayer>
	{
	};
}
