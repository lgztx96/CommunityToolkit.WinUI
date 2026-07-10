#pragma once

#include "ExpressionNodes/ExpressionNode.h"
#include "ReferenceNodes/AmbientLightReferenceNode.h"
#include "ReferenceNodes/ColorBrushReferenceNode.h"
#include "ReferenceNodes/DistantLightReferenceNode.h"
#include "ReferenceNodes/DropShadowReferenceNode.h"
#include "ReferenceNodes/InsetClipReferenceNode.h"
#include "ReferenceNodes/InteractionTrackerReferenceNode.h"
#include "ReferenceNodes/NineGridBrushReferenceNode.h"
#include "ReferenceNodes/PointLightReferenceNode.h"
#include "ReferenceNodes/PropertySetReferenceNode.h"
#include "ReferenceNodes/SpotLightReferenceNode.h"
#include "ReferenceNodes/SurfaceBrushReferenceNode.h"
#include "ReferenceNodes/VisualReferenceNode.h"
#include "ReferenceNodes/ManipulationPropertySetReferenceNode.h"
#include "ReferenceNodes/PointerPositionPropertySetReferenceNode.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Composition.Interactions.h>
#include <type_traits>
#include <concepts>
#else
import winrt.Microsoft.UI.Composition.Interactions;
#endif

namespace winrt 
{
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Composition::Interactions;
}

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    /// <summary>
    /// Connects expression nodes to composition APIs.
    /// </summary>
    struct CompositionExtensions final
    {
        static AmbientLightReferenceNode GetReference(winrt::AmbientLight const& compObj);
        static ColorBrushReferenceNode GetReference(winrt::CompositionColorBrush const& compObj);
        static DistantLightReferenceNode GetReference(winrt::DistantLight const& compObj);
        static DropShadowReferenceNode GetReference(winrt::DropShadow const& compObj);
        static InsetClipReferenceNode GetReference(winrt::InsetClip const& compObj);
        static InteractionTrackerReferenceNode GetReference(InteractionTracker const& compObj);
        static NineGridBrushReferenceNode GetReference(winrt::CompositionNineGridBrush const& compObj);
        static PointLightReferenceNode GetReference(winrt::PointLight const& compObj);
        static PropertySetReferenceNode GetReference(winrt::CompositionPropertySet const& compObj);
        static SpotLightReferenceNode GetReference(winrt::SpotLight const& compObj);
        static SurfaceBrushReferenceNode GetReference(winrt::CompositionSurfaceBrush const& compObj);
        static VisualReferenceNode GetReference(winrt::Visual const& compObj);

        template<typename T>
        static T GetSpecializedReference(winrt::CompositionPropertySet const& ps)
            requires std::is_base_of_v<PropertySetReferenceNode, T>
        {
            if constexpr (std::is_same_v<T, ManipulationPropertySetReferenceNode>)
            {
                return ManipulationPropertySetReferenceNode(std::nullopt, ps);
            }
            else if constexpr (std::is_same_v<T, PointerPositionPropertySetReferenceNode>)
            {
                return PointerPositionPropertySetReferenceNode(std::nullopt, ps);
            }
            else
            {
                throw winrt::hresult_invalid_argument(L"Invalid property set specialization");
            }
        }

        static void StartAnimation(winrt::CompositionObject const& compObject, winrt::hstring const& propertyName, ExpressionNode& expressionNode);
        static void InsertExpressionKeyFrame(winrt::KeyFrameAnimation const& keyframeAnimation, float normalizedProgressKey, ExpressionNode& expressionNode, winrt::CompositionEasingFunction const& easing = nullptr);
        static void SetCondition(winrt::InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode);
        static void SetRestingValue(winrt::InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode);
        static void SetCondition(winrt::InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode);
        static void SetMotion(winrt::InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode);
        static void SetCondition(winrt::CompositionConditionalValue const& modifier, ExpressionNode& expressionNode);
        static void SetValue(winrt::CompositionConditionalValue const& modifier, ExpressionNode& expressionNode);

    private:
        static winrt::ExpressionAnimation CreateExpressionAnimationFromNode(winrt::Compositor const& compositor, ExpressionNode& expressionNode);
    };
}
