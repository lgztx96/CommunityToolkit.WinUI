#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/Matrix3x2Node.h"
#include "../ExpressionNodes/ScalarNode.h"
#include "../ExpressionNodes/Vector2Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class SurfaceBrushReferenceNode final : public ReferenceNodeBase<SurfaceBrushReferenceNode>
    {
    public:
        SurfaceBrushReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::CompositionSurfaceBrush const& brush = nullptr) : ReferenceNodeBase(paramName, brush) {}
        static SurfaceBrushReferenceNode CreateTargetReference() { SurfaceBrushReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ScalarNode HorizontalAlignmentRatio() const { return ReferenceProperty<ScalarNode>(L"HorizontalAlignmentRatio"); }
        ScalarNode VerticalAlignmentRatio() const { return ReferenceProperty<ScalarNode>(L"VerticalAlignmentRatio"); }
        ScalarNode BottomInset() const { return ReferenceProperty<ScalarNode>(L"BottomInset"); }
        ScalarNode LeftInset() const { return ReferenceProperty<ScalarNode>(L"LeftInset"); }
        ScalarNode RightInset() const { return ReferenceProperty<ScalarNode>(L"RightInset"); }
        ScalarNode TopInset() const { return ReferenceProperty<ScalarNode>(L"TopInset"); }
        ScalarNode RotationAngle() const { return ReferenceProperty<ScalarNode>(L"RotationAngle"); }
        ScalarNode RotationAngleInDegrees() const { return ReferenceProperty<ScalarNode>(L"RotationAngleInDegrees"); }
        Vector2Node AnchorPoint() const { return ReferenceProperty<Vector2Node>(L"AnchorPoint"); }
        Vector2Node CenterPoint() const { return ReferenceProperty<Vector2Node>(L"CenterPoint"); }
        Vector2Node Offset() const { return ReferenceProperty<Vector2Node>(L"Offset"); }
        Vector2Node Scale() const { return ReferenceProperty<Vector2Node>(L"Scale"); }
        Matrix3x2Node TransformMatrix() const { return ReferenceProperty<Matrix3x2Node>(L"TransformMatrix"); }
    };
}
