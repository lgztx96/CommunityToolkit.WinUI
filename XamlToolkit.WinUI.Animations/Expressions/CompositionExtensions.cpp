#include "pch.h"
#include "winrt_module_imports.h"

#include "CompositionExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    AmbientLightReferenceNode CompositionExtensions::GetReference(winrt::AmbientLight const& compObj) { return AmbientLightReferenceNode(std::nullopt, compObj); }
    ColorBrushReferenceNode CompositionExtensions::GetReference(winrt::CompositionColorBrush const& compObj) { return ColorBrushReferenceNode(std::nullopt, compObj); }
    DistantLightReferenceNode CompositionExtensions::GetReference(winrt::DistantLight const& compObj) { return DistantLightReferenceNode(std::nullopt, compObj); }
    DropShadowReferenceNode CompositionExtensions::GetReference(winrt::DropShadow const& compObj) { return DropShadowReferenceNode(std::nullopt, compObj); }
    InsetClipReferenceNode CompositionExtensions::GetReference(winrt::InsetClip const& compObj) { return InsetClipReferenceNode(std::nullopt, compObj); }
    InteractionTrackerReferenceNode CompositionExtensions::GetReference(winrt::InteractionTracker const& compObj) { return InteractionTrackerReferenceNode(std::nullopt, compObj); }
    NineGridBrushReferenceNode CompositionExtensions::GetReference(winrt::CompositionNineGridBrush const& compObj) { return NineGridBrushReferenceNode(std::nullopt, compObj); }
    PointLightReferenceNode CompositionExtensions::GetReference(winrt::PointLight const& compObj) { return PointLightReferenceNode(std::nullopt, compObj); }
    PropertySetReferenceNode CompositionExtensions::GetReference(winrt::CompositionPropertySet const& compObj) { return PropertySetReferenceNode(std::nullopt, compObj); }
    SpotLightReferenceNode CompositionExtensions::GetReference(winrt::SpotLight const& compObj) { return SpotLightReferenceNode(std::nullopt, compObj); }
    SurfaceBrushReferenceNode CompositionExtensions::GetReference(winrt::CompositionSurfaceBrush const& compObj) { return SurfaceBrushReferenceNode(std::nullopt, compObj); }
    VisualReferenceNode CompositionExtensions::GetReference(winrt::Visual const& compObj) { return VisualReferenceNode(std::nullopt, compObj); }

    void CompositionExtensions::StartAnimation(winrt::CompositionObject const& compObject, winrt::hstring const& propertyName, ExpressionNode& expressionNode)
    {
        compObject.StartAnimation(propertyName, CreateExpressionAnimationFromNode(compObject.Compositor(), expressionNode));
    }

    void CompositionExtensions::InsertExpressionKeyFrame(winrt::KeyFrameAnimation const& keyframeAnimation, float normalizedProgressKey, ExpressionNode& expressionNode, winrt::CompositionEasingFunction const& easing)
    {
        expressionNode.ClearReferenceInfo();
        keyframeAnimation.InsertExpressionKeyFrame(normalizedProgressKey, expressionNode.ToExpressionString(), easing);
        expressionNode.SetAllParameters(keyframeAnimation);
    }

    void CompositionExtensions::SetCondition(winrt::InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Condition(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetRestingValue(winrt::InteractionTrackerInertiaRestingValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.RestingValue(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetCondition(winrt::InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Condition(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetMotion(winrt::InteractionTrackerInertiaMotion const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Motion(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetCondition(winrt::CompositionConditionalValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Condition(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    void CompositionExtensions::SetValue(winrt::CompositionConditionalValue const& modifier, ExpressionNode& expressionNode)
    {
        modifier.Value(CreateExpressionAnimationFromNode(modifier.Compositor(), expressionNode));
    }

    winrt::ExpressionAnimation CompositionExtensions::CreateExpressionAnimationFromNode(winrt::Compositor const& compositor, ExpressionNode& expressionNode)
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
