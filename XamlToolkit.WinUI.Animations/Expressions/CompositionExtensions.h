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
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Interactions.h>
#include <type_traits>
#include <concepts>
#endif

namespace winrt 
{
	using namespace Windows::UI::Composition;
	using namespace Windows::UI::Composition::Interactions;
}

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    /// <summary>
    /// Connects expression nodes to composition APIs.
    /// </summary>
    struct CompositionExtensions final
    {
        static AmbientLightReferenceNode GetReference(AmbientLight const& compObj);
        static ColorBrushReferenceNode GetReference(CompositionColorBrush const& compObj);
        static DistantLightReferenceNode GetReference(DistantLight const& compObj);
        static DropShadowReferenceNode GetReference(DropShadow const& compObj);
        static InsetClipReferenceNode GetReference(InsetClip const& compObj);
        static InteractionTrackerReferenceNode GetReference(InteractionTracker const& compObj);
        static NineGridBrushReferenceNode GetReference(CompositionNineGridBrush const& compObj);
        static PointLightReferenceNode GetReference(PointLight const& compObj);
        static PropertySetReferenceNode GetReference(CompositionPropertySet const& compObj);
        static SpotLightReferenceNode GetReference(SpotLight const& compObj);
        static SurfaceBrushReferenceNode GetReference(CompositionSurfaceBrush const& compObj);
        static VisualReferenceNode GetReference(Visual const& compObj);

        template<typename T>
        static T GetSpecializedReference(CompositionPropertySet const& ps)
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

        static void StartAnimation(CompositionObject const& compObject, winrt::hstring const& propertyName, ExpressionNode& expressionNode);
        static void InsertExpressionKeyFrame(KeyFrameAnimation const& keyframeAnimation, float normalizedProgressKey, ExpressionNode& expressionNode, CompositionEasingFunction const& easing = nullptr);
        static void SetCondition(InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode);
        static void SetRestingValue(InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode);
        static void SetCondition(InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode);
        static void SetMotion(InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode);
        static void SetCondition(CompositionConditionalValue const& modifier, ExpressionNode& expressionNode);
        static void SetValue(CompositionConditionalValue const& modifier, ExpressionNode& expressionNode);

    private:
        static ExpressionAnimation CreateExpressionAnimationFromNode(Compositor const& compositor, ExpressionNode& expressionNode);
    };
}
