#pragma once

#include "ExpressionNodes/BooleanNode.h"
#include "ExpressionNodes/ColorNode.h"
#include "ExpressionNodes/Matrix3x2Node.h"
#include "ExpressionNodes/Matrix4x4Node.h"
#include "ExpressionNodes/QuaternionNode.h"
#include "ExpressionNodes/ScalarNode.h"
#include "ExpressionNodes/Vector2Node.h"
#include "ExpressionNodes/Vector3Node.h"
#include "ExpressionNodes/Vector4Node.h"
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

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    /// <summary>
    /// Provides factory helpers for well-known expression values and parameters.
    /// </summary>
    struct ExpressionValues final
    {
        /// <summary>
        /// Creates named constant parameter nodes.
        /// </summary>
        struct Constant final
        {
            static BooleanNode CreateConstantBoolean(winrt::hstring const& paramName) { return BooleanNode(paramName); }
            static ScalarNode CreateConstantScalar(winrt::hstring const& paramName) { return ScalarNode(paramName); }
            static Vector2Node CreateConstantVector2(winrt::hstring const& paramName) { return Vector2Node(paramName); }
            static Vector3Node CreateConstantVector3(winrt::hstring const& paramName) { return Vector3Node(paramName); }
            static Vector4Node CreateConstantVector4(winrt::hstring const& paramName) { return Vector4Node(paramName); }
            static ColorNode CreateConstantColor(winrt::hstring const& paramName) { return ColorNode(paramName); }
            static QuaternionNode CreateConstantQuaternion(winrt::hstring const& paramName) { return QuaternionNode(paramName); }
            static Matrix3x2Node CreateConstantMatrix3x2(winrt::hstring const& paramName) { return Matrix3x2Node(paramName); }
            static Matrix4x4Node CreateConstantMatrix4x4(winrt::hstring const& paramName) { return Matrix4x4Node(paramName); }

            static BooleanNode CreateConstantBoolean(winrt::hstring const& paramName, bool value) { return BooleanNode(paramName, value); }
            static ScalarNode CreateConstantScalar(winrt::hstring const& paramName, float value) { return ScalarNode(paramName, value); }
            static Vector2Node CreateConstantVector2(winrt::hstring const& paramName, Windows::Foundation::Numerics::float2 const& value) { return Vector2Node(paramName, value); }
            static Vector3Node CreateConstantVector3(winrt::hstring const& paramName, Windows::Foundation::Numerics::float3 const& value) { return Vector3Node(paramName, value); }
            static Vector4Node CreateConstantVector4(winrt::hstring const& paramName, Windows::Foundation::Numerics::float4 const& value) { return Vector4Node(paramName, value); }
            static ColorNode CreateConstantColor(winrt::hstring const& paramName, Windows::UI::Color const& value) { return ColorNode(paramName, value); }
            static QuaternionNode CreateConstantQuaternion(winrt::hstring const& paramName, Windows::Foundation::Numerics::quaternion const& value) { return QuaternionNode(paramName, value); }
            static Matrix3x2Node CreateConstantMatrix3x2(winrt::hstring const& paramName, Windows::Foundation::Numerics::float3x2 const& value) { return Matrix3x2Node(paramName, value); }
            static Matrix4x4Node CreateConstantMatrix4x4(winrt::hstring const& paramName, Windows::Foundation::Numerics::float4x4 const& value) { return Matrix4x4Node(paramName, value); }
        };

        /// <summary>
        /// Creates nodes that refer to the current animated value.
        /// </summary>
        struct CurrentValue final
        {
            static BooleanNode CreateBooleanCurrentValue() { return ExpressionNode::CreateValueKeyword<BooleanNode>(ValueKeywordKind::CurrentValue); }
            static ScalarNode CreateScalarCurrentValue() { return ExpressionNode::CreateValueKeyword<ScalarNode>(ValueKeywordKind::CurrentValue); }
            static Vector2Node CreateVector2CurrentValue() { return ExpressionNode::CreateValueKeyword<Vector2Node>(ValueKeywordKind::CurrentValue); }
            static Vector3Node CreateVector3CurrentValue() { return ExpressionNode::CreateValueKeyword<Vector3Node>(ValueKeywordKind::CurrentValue); }
            static Vector4Node CreateVector4CurrentValue() { return ExpressionNode::CreateValueKeyword<Vector4Node>(ValueKeywordKind::CurrentValue); }
            static ColorNode CreateColorCurrentValue() { return ExpressionNode::CreateValueKeyword<ColorNode>(ValueKeywordKind::CurrentValue); }
            static QuaternionNode CreateQuaternionCurrentValue() { return ExpressionNode::CreateValueKeyword<QuaternionNode>(ValueKeywordKind::CurrentValue); }
            static Matrix3x2Node CreateMatrix3x2CurrentValue() { return ExpressionNode::CreateValueKeyword<Matrix3x2Node>(ValueKeywordKind::CurrentValue); }
            static Matrix4x4Node CreateMatrix4x4CurrentValue() { return ExpressionNode::CreateValueKeyword<Matrix4x4Node>(ValueKeywordKind::CurrentValue); }
        };

        /// <summary>
        /// Creates nodes that refer to the value sampled at the first frame.
        /// </summary>
        struct StartingValue final
        {
            static BooleanNode CreateBooleanStartingValue() { return ExpressionNode::CreateValueKeyword<BooleanNode>(ValueKeywordKind::StartingValue); }
            static ScalarNode CreateScalarStartingValue() { return ExpressionNode::CreateValueKeyword<ScalarNode>(ValueKeywordKind::StartingValue); }
            static Vector2Node CreateVector2StartingValue() { return ExpressionNode::CreateValueKeyword<Vector2Node>(ValueKeywordKind::StartingValue); }
            static Vector3Node CreateVector3StartingValue() { return ExpressionNode::CreateValueKeyword<Vector3Node>(ValueKeywordKind::StartingValue); }
            static Vector4Node CreateVector4StartingValue() { return ExpressionNode::CreateValueKeyword<Vector4Node>(ValueKeywordKind::StartingValue); }
            static ColorNode CreateColorStartingValue() { return ExpressionNode::CreateValueKeyword<ColorNode>(ValueKeywordKind::StartingValue); }
            static QuaternionNode CreateQuaternionStartingValue() { return ExpressionNode::CreateValueKeyword<QuaternionNode>(ValueKeywordKind::StartingValue); }
            static Matrix3x2Node CreateMatrix3x2StartingValue() { return ExpressionNode::CreateValueKeyword<Matrix3x2Node>(ValueKeywordKind::StartingValue); }
            static Matrix4x4Node CreateMatrix4x4StartingValue() { return ExpressionNode::CreateValueKeyword<Matrix4x4Node>(ValueKeywordKind::StartingValue); }
        };

        /// <summary>
        /// Creates named composition object reference parameters.
        /// </summary>
        struct Reference final
        {
            static AmbientLightReferenceNode CreateAmbientLightReference(winrt::hstring const& parameterName) { return AmbientLightReferenceNode(parameterName); }
            static ColorBrushReferenceNode CreateColorBrushReference(winrt::hstring const& parameterName) { return ColorBrushReferenceNode(parameterName); }
            static DistantLightReferenceNode CreateDistantLightReference(winrt::hstring const& parameterName) { return DistantLightReferenceNode(parameterName); }
            static DropShadowReferenceNode CreateDropShadowReference(winrt::hstring const& parameterName) { return DropShadowReferenceNode(parameterName); }
            static InsetClipReferenceNode CreateInsetClipReference(winrt::hstring const& parameterName) { return InsetClipReferenceNode(parameterName); }
            static InteractionTrackerReferenceNode CreateInteractionTrackerReference(winrt::hstring const& parameterName) { return InteractionTrackerReferenceNode(parameterName); }
            static NineGridBrushReferenceNode CreateNineGridBrushReference(winrt::hstring const& parameterName) { return NineGridBrushReferenceNode(parameterName); }
            static PointLightReferenceNode CreatePointLightReference(winrt::hstring const& parameterName) { return PointLightReferenceNode(parameterName); }
            static PropertySetReferenceNode CreatePropertySetReference(winrt::hstring const& parameterName) { return PropertySetReferenceNode(parameterName); }
            static SpotLightReferenceNode CreateSpotLightReference(winrt::hstring const& parameterName) { return SpotLightReferenceNode(parameterName); }
            static SurfaceBrushReferenceNode CreateSurfaceBrushReference(winrt::hstring const& parameterName) { return SurfaceBrushReferenceNode(parameterName); }
            static VisualReferenceNode CreateVisualReference(winrt::hstring const& parameterName) { return VisualReferenceNode(parameterName); }
        };

        /// <summary>
        /// Creates references to the target composition object.
        /// </summary>
        struct Target final
        {
            static AmbientLightReferenceNode CreateAmbientLightTarget() { return AmbientLightReferenceNode::CreateTargetReference(); }
            static ColorBrushReferenceNode CreateColorBrushTarget() { return ColorBrushReferenceNode::CreateTargetReference(); }
            static DistantLightReferenceNode CreateDistantLightTarget() { return DistantLightReferenceNode::CreateTargetReference(); }
            static DropShadowReferenceNode CreateDropShadowTarget() { return DropShadowReferenceNode::CreateTargetReference(); }
            static InsetClipReferenceNode CreateInsetClipTarget() { return InsetClipReferenceNode::CreateTargetReference(); }
            static InteractionTrackerReferenceNode CreateInteractionTrackerTarget() { return InteractionTrackerReferenceNode::CreateTargetReference(); }
            static NineGridBrushReferenceNode CreateNineGridBrushTarget() { return NineGridBrushReferenceNode::CreateTargetReference(); }
            static PointLightReferenceNode CreatePointLightTarget() { return PointLightReferenceNode::CreateTargetReference(); }
            static PropertySetReferenceNode CreatePropertySetTarget() { return PropertySetReferenceNode::CreateTargetReference(); }
            static SpotLightReferenceNode CreateSpotLightTarget() { return SpotLightReferenceNode::CreateTargetReference(); }
            static SurfaceBrushReferenceNode CreateSurfaceBrushTarget() { return SurfaceBrushReferenceNode::CreateTargetReference(); }
            static VisualReferenceNode CreateVisualTarget() { return VisualReferenceNode::CreateTargetReference(); }
        };
    };
}
