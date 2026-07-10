#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/ColorNode.h"
#include "../ExpressionNodes/ScalarNode.h"
#include "../ExpressionNodes/Vector3Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class PointLightReferenceNode final : public ReferenceNodeBase<PointLightReferenceNode>
    {
    public:
        PointLightReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::PointLight const& light = nullptr) : ReferenceNodeBase(paramName, light) {}
        static PointLightReferenceNode CreateTargetReference() { PointLightReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ScalarNode ConstantAttenuation() const { return ReferenceProperty<ScalarNode>(L"ConstantAttenuation"); }
        ScalarNode LinearAttenuation() const { return ReferenceProperty<ScalarNode>(L"LinearAttenuation"); }
        ScalarNode QuadraticAttentuation() const { return ReferenceProperty<ScalarNode>(L"QuadraticAttentuation"); }
        ColorNode Color() const { return ReferenceProperty<ColorNode>(L"Color"); }
        Vector3Node Direction() const { return ReferenceProperty<Vector3Node>(L"Direction"); }
        Vector3Node Offset() const { return ReferenceProperty<Vector3Node>(L"Offset"); }
    };
}
