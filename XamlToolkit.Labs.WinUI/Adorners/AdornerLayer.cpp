#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <memory>
#endif
#include "AdornerLayer.h"
#if __has_include("AdornerLayer.g.cpp")
#include "AdornerLayer.g.cpp"
#endif
#include "AdornerDecorator.h"
#include "../XamlToolkit.WinUI/common.h"
#include "Adorner.h"
#include "Helpers/FrameworkElementExtensions.WaitUntilLoaded.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> AdornerLayer::XamlProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"Xaml",
			winrt::xaml_typename<winrt::UIElement>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr, &AdornerLayer::OnXamlPropertyChanged });

	AdornerLayer::AdornerLayer()
	{
		SizeChanged({ this, &AdornerLayer::AdornerLayer_SizeChanged });
	}

	void AdornerLayer::AdornerLayer_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
	{
		for (const auto& adornerXaml : Children())
		{
			if (auto adorner = adornerXaml.try_as<winrt::XamlToolkit::Labs::WinUI::Adorner>())
			{
				auto adornerImpl = winrt::get_self<Adorner>(adorner);
				// Notify each adorner that our general layout has updated.
				adornerImpl->OnLayoutUpdated(nullptr, nullptr);
			}
		}
	}

	winrt::IAsyncAction AdornerLayer::OnXamlPropertyChanged(winrt::DependencyObject const& dependencyObject, winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (auto fe = dependencyObject.try_as<winrt::FrameworkElement>())
		{
			if (!fe.IsLoaded() || fe.Parent() == nullptr)
			{
				auto loadedToken = std::make_shared<winrt::event_token>();
				*loadedToken = fe.Loaded([=](auto& s, auto& e)
				{
					s.template as<winrt::FrameworkElement>().Loaded(*loadedToken);
					XamlPropertyFrameworkElement_Loaded(s, e);
				});
			}
			else if (auto adorner = args.NewValue().try_as<winrt::UIElement>())
			{
				if (auto layer = co_await GetAdornerLayerAsync(fe))
				{
					AttachAdorner(layer, fe, adorner);
				}
			}
			else if (args.NewValue() == nullptr)
			{
				if (auto oldAdorner = args.OldValue().try_as<winrt::UIElement>())
				{
					if (auto layer = co_await GetAdornerLayerAsync(fe))
					{
						RemoveAdorner(layer, oldAdorner);
					}
				}
			}
		}
	}

	winrt::IAsyncAction AdornerLayer::XamlPropertyFrameworkElement_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e)
	{
		if (auto fe = sender.try_as<winrt::FrameworkElement>())
		{
			if (auto layer = co_await GetAdornerLayerAsync(fe))
			{
				auto adorner = GetXaml(fe);

				if (adorner == nullptr) co_return;

				AttachAdorner(layer, fe, adorner);
			}
		}
	}

	winrt::IAsyncOperation<winrt::XamlToolkit::Labs::WinUI::AdornerLayer> AdornerLayer::GetAdornerLayerAsync(winrt::FrameworkElement const& adornedElement)
	{
		// 1. Find Adorner Layer for element or top-most element
		winrt::FrameworkElement lastElement{ nullptr };

		auto adornerLayerOrTopMostElement = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::FrameworkElement>(adornedElement, [&](auto&& element) -> bool
		{
			lastElement = element; // TODO: should this be after our if, does it matter?

			if (element.template try_as<winrt::XamlToolkit::Labs::WinUI::AdornerDecorator>())
			{
				return true;
			}
			else if (element.template try_as<winrt::XamlToolkit::Labs::WinUI::AdornerLayer>())
			{
				return true;
			}
			else if (element.template try_as<winrt::ScrollViewer>())
			{
				return true;
			}
			// TODO: Need to figure out porting new DO toolkit helpers to Uno, only needed for custom adorner layer placement...
			/*else
			{
				// TODO: Use BreadthFirst Search w/ Depth Limited?
				auto child = element.FindFirstLevelDescendants<AdornerLayer>();

				if (child != null)
				{
					lastElement = child;
					return true;
				}
			}*/

			return false;
		});
		
		if (adornerLayerOrTopMostElement == nullptr)
			adornerLayerOrTopMostElement = lastElement;

		// Check cases where we may have found a child that we want to use instead of the element returned by search.
		if (lastElement.try_as<winrt::XamlToolkit::Labs::WinUI::AdornerLayer>() || lastElement.try_as<winrt::XamlToolkit::Labs::WinUI::AdornerDecorator>())
		{
			adornerLayerOrTopMostElement = lastElement;
		}

		if (auto decorator = adornerLayerOrTopMostElement.try_as<winrt::XamlToolkit::Labs::WinUI::AdornerDecorator>())
		{
			co_await winrt::XamlToolkit::WinUI::Future::FrameworkElementExtensions::WaitUntilLoadedAsync(decorator);
			auto decoratorImpl = winrt::get_self<AdornerDecorator>(decorator);
			co_return decoratorImpl->AdornerLayer();
		}
		else if (auto layer = adornerLayerOrTopMostElement.try_as<winrt::XamlToolkit::Labs::WinUI::AdornerLayer>())
		{
			co_await winrt::XamlToolkit::WinUI::Future::FrameworkElementExtensions::WaitUntilLoadedAsync(layer);

			// If we just have an adorner layer now, we're done!
			co_return layer;
		}
		else
		{
			// TODO: Windows.UI.Xaml.Internal.RootScrollViewer is a maybe different and what was causing issues before I looked for ScrollViewers along the way?
			// It's an internal unexposed type, so maybe it inherits from ScrollViewer? Not sure yet, but might need to detect and
			// do something different here?

			// ScrollViewers need AdornerLayers so they can provide adorners that scroll with the adorned elements (as it worked in WPF).
			// Note: ScrollViewers and the Window were the main AdornerLayer integration points in WPF.
			if (auto scroller = adornerLayerOrTopMostElement.try_as<winrt::ScrollViewer>())
			{
				auto content = scroller.Content().try_as<winrt::FrameworkElement>();
				// Extra code for RootScrollViewer TODO: Can we detect this better?
				if (scroller.Parent() == nullptr)
				{
					//// XamlMarkupHelper.UnloadObject doesn't work here (throws an invalid value exception) does content need a name?
					// TODO: Figure out this scenario?
					throw winrt::hresult_not_implemented(L"RootScrollViewer attachment isn't supported, add a AdornerDecorator or ScrollViewer manually to the top-level of your application.");
				}

				scroller.Content(nullptr);

				auto layerContainer = winrt::make_self<AdornerDecorator>();
				layerContainer->Child(content);

				scroller.Content(*layerContainer);

				co_await winrt::XamlToolkit::WinUI::Future::FrameworkElementExtensions::WaitUntilLoadedAsync(*layerContainer);

				co_return layerContainer->AdornerLayer();
			}
			// Grid seems like the easiest place for us to inject AdornerLayers automatically at the top-level (if needed) - not sure how common this will be?
			else if (auto grid = adornerLayerOrTopMostElement.try_as<winrt::Grid>())
			{
				// TODO: Not sure how we want to handle AdornerDecorator in this scenario...
				auto adornerLayer = winrt::make<AdornerLayer>();

				// TODO: Handle if grid row/columns change.
				winrt::Grid::SetRowSpan(adornerLayer, grid.RowDefinitions().Size());
				winrt::Grid::SetColumnSpan(adornerLayer, grid.ColumnDefinitions().Size());
				grid.Children().Append(adornerLayer);

				co_await winrt::XamlToolkit::WinUI::Future::FrameworkElementExtensions::WaitUntilLoadedAsync(adornerLayer);

				co_return adornerLayer;
			}
		}

		co_return nullptr;
	}

	void AdornerLayer::AttachAdorner(winrt::XamlToolkit::Labs::WinUI::AdornerLayer const& layer, winrt::FrameworkElement const& adornedElement, winrt::UIElement const& adornerXaml)
	{
		auto adorner = adornerXaml.try_as<winrt::XamlToolkit::Labs::WinUI::Adorner>();
		if (adorner)
		{
			// We already have an adorner type, use it directly.
		}
		else
		{
			adorner = winrt::make<Adorner>();
			adorner.Content(adornerXaml);
		}

		// Add adorner XAML content to the Adorner Layer
		auto adornerImpl = winrt::get_self<Adorner>(adorner);
		adornerImpl->AdornerLayer(layer);
		adornerImpl->AdornedElement(adornedElement);

		layer.Children().Append(adorner);
	}

	void AdornerLayer::RemoveAdorner(winrt::XamlToolkit::Labs::WinUI::AdornerLayer const& layer, winrt::UIElement const& adornerXaml)
	{
		if (auto adorner = XamlToolkit::WinUI::DependencyObjectEx::FindAscendantOrSelf<winrt::XamlToolkit::Labs::WinUI::Adorner>(adornerXaml))
		{
			auto adornerImpl = winrt::get_self<Adorner>(adorner);
			adornerImpl->AdornedElement(nullptr);
			adornerImpl->AdornerLayer(nullptr);

			auto children = layer.Children();
			if (uint32_t index; children.IndexOf(adorner, index)) 
				children.RemoveAt(index);

			winrt::VisualTreeHelper::DisconnectChildrenRecursive(adorner);
		}
	}
}
