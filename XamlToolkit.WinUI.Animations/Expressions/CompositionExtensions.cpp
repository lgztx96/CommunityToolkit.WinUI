#include "pch.h"
#include "winrt_module_imports.h"

#include "CompositionExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    AmbientLightReferenceNode CompositionExtensions::GetReference(AmbientLight const& compObj) { return AmbientLightReferenceNode(std::nullopt, compObj); }
    ColorBrushReferenceNode CompositionExtensions::GetReference(CompositionColorBrush const& compObj) { return ColorBrushReferenceNode(std::nullopt, compObj); }
    DistantLightReferenceNode CompositionExtensions::GetReference(DistantLight const& compObj) { return DistantLightReferenceNode(std::nullopt, compObj); }
    DropShadowReferenceNode CompositionExtensions::GetReference(DropShadow const& compObj) { return DropShadowReferenceNode(std::nullopt, compObj); }
    InsetClipReferenceNode CompositionExtensions::GetReference(InsetClip const& compObj) { return InsetClipReferenceNode(std::nullopt, compObj); }
    InteractionTrackerReferenceNode CompositionExtensions::GetReference(InteractionTracker const& compObj) { return InteractionTrackerReferenceNode(std::nullopt, compObj); }
    NineGridBrushReferenceNode CompositionExtensions::GetReference(CompositionNineGridBrush const& compObj) { return NineGridBrushReferenceNode(std::nullopt, compObj); }
    PointLightReferenceNode CompositionExtensions::GetReference(PointLight const& compObj) { return PointLightReferenceNode(std::nullopt, compObj); }
    PropertySetReferenceNode CompositionExtensions::GetReference(CompositionPropertySet const& compObj) { return PropertySetReferenceNode(std::nullopt, compObj); }
    SpotLightReferenceNode CompositionExtensions::GetReference(SpotLight const& compObj) { return SpotLightReferenceNode(std::nullopt, compObj); }
    SurfaceBrushReferenceNode CompositionExtensions::GetReference(CompositionSurfaceBrush const& compObj) { return SurfaceBrushReferenceNode(std::nullopt, compObj); }
    VisualReferenceNode CompositionExtensions::GetReference(Visual const& compObj) { return VisualReferenceNode(std::nullopt, compObj); }

    void CompositionExtensions::StartAnimation(CompositionObject const& compObject, winrt::hstring const& propertyName, ExpressionNode& expressionNode)
    {
        compObject.StartAnimation(propertyName, CreateExpressionAnimationFromNode(compObject.Compositor(), expressionNode));
    }

    void CompositionExtensions::InsertExpressionKeyFrame(KeyFrameAnimation const& keyframeAnimation, float normalizedProgressKey, ExpressionNode& expressionNode, CompositionEasingFunction const& easing)
    {
        expressionNode.ClearReferenceInfo();
        keyframeAnimation.InsertExpressionKeyFrame(normalizedProgressKey, expressionNode.ToExpressionString(), easing);
        expressionNode.SetAllParameters(keyframeAnimation);
    }

    void CompositionExtensions::SetCondition(InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Condition(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetRestingValue(InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.RestingValue(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetCondition(InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Condition(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetMotion(InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Motion(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetCondition(CompositionConditionalValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Condition(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetValue(CompositionConditionalValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Value(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    ExpressionAnimation CompositionExtensions::CreateExpressionAnimationFromNode(Compositor const& compositor, ExpressionNode& expressionNode)
    {
        if (expressionNode.ExpressionAnimation == nullptr)
        {
            expressionNode.ClearReferenceInfo();
            expressionNode.ExpressionAnimation = compositor.CreateExpressionAnimation(expressionNode.ToExpressionString());
        }

        expressionNode.SetAllParameters(expressionNode.ExpressionAnimation);
        return expressionNode.ExpressionAnimation;
    }
}
