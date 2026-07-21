#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/Matrix4x4Node.h"
#include "../ExpressionNodes/QuaternionNode.h"
#include "../ExpressionNodes/ScalarNode.h"
#include "../ExpressionNodes/Vector2Node.h"
#include "../ExpressionNodes/Vector3Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class VisualReferenceNode final : public ReferenceNodeBase<VisualReferenceNode>
    {
    public:
        VisualReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::Visual const& v = nullptr) : ReferenceNodeBase(paramName, v) {}
        static VisualReferenceNode CreateTargetReference() { VisualReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ScalarNode Opacity() const { return ReferenceProperty<ScalarNode>(L"Opacity"); }
        ScalarNode RotationAngle() const { return ReferenceProperty<ScalarNode>(L"RotationAngle"); }
        ScalarNode RotationAngleInDegrees() const { return ReferenceProperty<ScalarNode>(L"RotationAngleInDegrees"); }
        Vector2Node AnchorPoint() const { return ReferenceProperty<Vector2Node>(L"AnchorPoint"); }
        Vector2Node RelativeSize() const { return ReferenceProperty<Vector2Node>(L"RelativeSize"); }
        Vector2Node Size() const { return ReferenceProperty<Vector2Node>(L"Size"); }
        Vector3Node CenterPoint() const { return ReferenceProperty<Vector3Node>(L"CenterPoint"); }
        Vector3Node Offset() const { return ReferenceProperty<Vector3Node>(L"Offset"); }
        Vector3Node RelativeOffset() const { return ReferenceProperty<Vector3Node>(L"RelativeOffset"); }
        Vector3Node RotationAxis() const { return ReferenceProperty<Vector3Node>(L"RotationAxis"); }
        Vector3Node Scale() const { return ReferenceProperty<Vector3Node>(L"Scale"); }
        Vector3Node Translation() const { return GetVector3Property(L"Translation"); }
        QuaternionNode Orientation() const { return ReferenceProperty<QuaternionNode>(L"Orientation"); }
        Matrix4x4Node TransformMatrix() const { return ReferenceProperty<Matrix4x4Node>(L"TransformMatrix"); }
    };
}
