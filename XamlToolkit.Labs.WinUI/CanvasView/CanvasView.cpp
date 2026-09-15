#include "pch.h"
#include "winrt_module_imports.h"
#include "CanvasView.h"
#if __has_include("CanvasView.g.cpp")
#include "CanvasView.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#else
import winrt.Microsoft.UI.Xaml.Data;
import winrt.Microsoft.UI.Xaml.Markup;
import winrt.Microsoft.UI.Xaml.Media;
#endif

namespace winrt
{
    using namespace ::winrt::Microsoft::UI::Xaml::Data;
    using namespace ::winrt::Microsoft::UI::Xaml::Markup;
    using namespace ::winrt::Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    CanvasView::CanvasView()
    {
        // TODO: Need to use XamlReader because of https://github.com/microsoft/microsoft-ui-xaml/issues/2898
        ItemsPanel(winrt::XamlReader::Load(
            LR"(
            <ItemsPanelTemplate xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
                <Canvas/>
            </ItemsPanelTemplate>)").as<winrt::ItemsPanelTemplate>());
    }

    winrt::DependencyObject CanvasView::GetContainerForItemOverride()
    {
        return winrt::ContentPresenter();
    }

    bool CanvasView::IsItemItsOwnContainerOverride(winrt::IInspectable const& item)
    {
        return item.try_as<winrt::ContentPresenter>() != nullptr;
    }

    void CanvasView::PrepareContainerForItemOverride(
        winrt::DependencyObject const& element,
        winrt::IInspectable const& item)
    {
        base_type::PrepareContainerForItemOverride(element, item);

        // ContentPresenter is the default container for Canvas.
        if (const auto cp = element.try_as<winrt::ContentPresenter>())
        {
            const auto renderingEventToken = std::make_shared<winrt::event_token>();
            *renderingEventToken = winrt::CompositionTarget::Rendering([renderingEventToken, cp](auto&, auto&) 
            {
                // Detach event or Rendering will keep calling us back.
                winrt::CompositionTarget::Rendering(*renderingEventToken);

                SetupChildBinding(cp);
            });

            // Loaded is not firing when dynamically loading an element to the collection. Relay on CompositionTargetHelper above.
            // Seems like a bug in Loaded event?
            const auto loadedEventToken = std::make_shared<winrt::event_token>();
            *loadedEventToken = cp.Loaded([loadedEventToken](auto&& sender, auto&&)
            {
                const auto cp = sender.template try_as<winrt::ContentPresenter>();
                cp.Loaded(*loadedEventToken); // revoke the stored token

                SetupChildBinding(cp);
            });

            _manipulationDeltaTokens[cp] = cp.ManipulationDelta({ this, &CanvasView::ContentPresenter_ManipulationDelta });
        }

        // TODO: Do we want to support something else in a custom template??
        // else if (auto fe = item.try_as<winrt::FrameworkElement>())
    }

    void CanvasView::ClearContainerForItemOverride(
        winrt::DependencyObject const& element,
        winrt::IInspectable const& item)
    {
        base_type::ClearContainerForItemOverride(element, item);

        if (const auto cp = element.try_as<winrt::ContentPresenter>())
        {
            if (const auto it = _manipulationDeltaTokens.find(cp); it != _manipulationDeltaTokens.end())
            {
                cp.ManipulationDelta(it->second);
                _manipulationDeltaTokens.erase(it);
            }
        }
    }

    void CanvasView::ContentPresenter_ManipulationDelta(
        winrt::IInspectable const& sender,
        winrt::ManipulationDeltaRoutedEventArgs const& e)
    {
        // Move the rectangle.
        if (const auto cp = sender.try_as<winrt::ContentPresenter>())
        {
            // TODO: Seeing some drift, not sure if due to DPI or just general drift
            // or probably we need to do the start/from delta approach we did with SizerBase to resolve.
            const auto delta = e.Delta();
            SetBindingExpressionValue(cp,
                winrt::Canvas::LeftProperty(),
                winrt::box_value(winrt::Canvas::GetLeft(cp) + delta.Translation.X));

            SetBindingExpressionValue(cp,
                winrt::Canvas::TopProperty(),
                winrt::box_value(winrt::Canvas::GetTop(cp) + delta.Translation.Y));
        }
    }

    void CanvasView::SetupChildBinding(winrt::ContentPresenter const& cp)
    {
        // Get direct visual descendant for ContentPresenter to look for Canvas properties within Template.
        if (const auto child = winrt::VisualTreeHelper::GetChild(cp, 0))
        {
            // TODO: Should we avoid doing this twice?

            // Hook up any properties we care about from the templated children to its parent ContentPresenter.
            for (const auto& [property, path] : lifted_properties)
            {
                winrt::Binding binding;
                binding.Source(child);
                // binding.Mode(BindingMode::TwoWay); // TODO: Should this be exposed as a general property?
                binding.Path(winrt::PropertyPath(path));

                cp.SetBinding(property, binding);
            }
        }
    }

    void CanvasView::SetBindingExpressionValue(
        winrt::FrameworkElement const& fe,
        winrt::DependencyProperty const& property,
        winrt::IInspectable const& value)
    {
        const auto subBinding = fe.GetBindingExpression(property);

        if (!subBinding)
        {
            fe.SetValue(property, value);
        }
        else if (const auto subfe = subBinding.DataItem().try_as<winrt::FrameworkElement>())
        {
            SetBindingExpressionValue(subfe, property, value);
        }
        else if (const auto dataItem = subBinding.DataItem();
            dataItem && subBinding.ParentBinding().Path())
        {
            const auto path = subBinding.ParentBinding().Path().Path();

            if (const auto propertyProvider = dataItem.try_as<winrt::ICustomPropertyProvider>())
            {
                if (const auto customProperty = propertyProvider.GetCustomProperty(path))
                {
                    customProperty.SetValue(dataItem, value);
                }
            }
        }
    }
}
