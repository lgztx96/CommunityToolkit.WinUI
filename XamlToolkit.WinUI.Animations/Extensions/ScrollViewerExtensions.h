#pragma once

#include "../Enums/Axis.h"
#include "../Enums/VisualProperty.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Hosting;
	using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Provides attached dependency properties and methods for the <see cref="ScrollViewer"/> control.
    /// </summary>
    class ScrollViewerExtensions
    {
    public:
        /// <summary>
        /// Creates and starts an animation on the target element that binds either the X or Y axis of the source <see cref="ScrollViewer"/>.
        /// </summary>
        /// <param name="scroller">The source <see cref="ScrollViewer"/> control to use.</param>
        /// <param name="target">The target <see cref="UIElement"/> that will be animated.</param>
        /// <param name="axis">The scrolling axis of the source <see cref="ScrollViewer"/>.</param>
        /// <param name="property">The target <see cref="Visual"/> property to animate.</param>
        /// <returns>An <see cref="ExpressionAnimation"/> instance that represents an already running animation.</returns>
        static ExpressionAnimation StartExpressionAnimation(
            ScrollViewer const& scroller,
            UIElement const& target,
            Axis axis,
            VisualProperty property = VisualProperty::Translation)
        {
            return StartExpressionAnimation(scroller, target, axis, axis, property);
        }

        /// <summary>
        /// Creates and starts an animation on the target element that binds either the X or Y axis of the source <see cref="ScrollViewer"/>
        /// </summary>
        /// <param name="scroller">The source <see cref="ScrollViewer"/> control to use</param>
        /// <param name="target">The target <see cref="UIElement"/> that will be animated</param>
        /// <param name="sourceAxis">The scrolling axis of the source <see cref="ScrollViewer"/></param>
        /// <param name="targetAxis">The optional scrolling axis of the target element, if <see langword="null"/> the source axis will be used</param>
        /// <param name="property">The target <see cref="Visual"/> property to animate.</param>
        /// <returns>An <see cref="ExpressionAnimation"/> instance that represents an already running animation.</returns>
        static ExpressionAnimation StartExpressionAnimation(
            ScrollViewer const& scroller,
            UIElement const& target,
            Axis sourceAxis,
            Axis targetAxis,
            VisualProperty property = VisualProperty::Translation)
        {
            CompositionPropertySet scrollSet = ElementCompositionPreview::GetScrollViewerManipulationPropertySet(scroller);

            winrt::hstring axisName = GetAxisName(sourceAxis);
            winrt::hstring expression = L"scroller.Translation." + axisName;

            ExpressionAnimation animation = scrollSet.Compositor().CreateExpressionAnimation(expression);
            animation.SetReferenceParameter(L"scroller", scrollSet);

            Visual visual = ElementCompositionPreview::GetElementVisual(target);

            winrt::hstring targetAxisName = GetAxisName(targetAxis);

            switch (property)
            {
            case VisualProperty::Translation:
                ElementCompositionPreview::SetIsTranslationEnabled(target, true);
                visual.StartAnimation(L"Translation." + targetAxisName, animation);
                break;
            case VisualProperty::Offset:
                visual.StartAnimation(L"Offset." + targetAxisName, animation);
                break;
            default:
                throw winrt::hresult_invalid_argument(L"Invalid target property");
            }

            return animation;
        }

    private:
        static winrt::hstring GetAxisName(Axis axis)
        {
            switch (axis)
            {
            case Axis::X: return L"X";
            case Axis::Y: return L"Y";
            case Axis::Z: return L"Z";
            default: return L"X";
            }
        }
    };
}
