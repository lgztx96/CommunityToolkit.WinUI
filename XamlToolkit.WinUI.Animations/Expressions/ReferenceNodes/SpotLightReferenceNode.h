#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/ColorNode.h"
#include "../ExpressionNodes/ScalarNode.h"
#include "../ExpressionNodes/Vector3Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class SpotLightReferenceNode final : public ReferenceNodeBase<SpotLightReferenceNode>
    {
    public:
        SpotLightReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::SpotLight const& light = nullptr) : ReferenceNodeBase(paramName, light) {}
        static SpotLightReferenceNode CreateTargetReference() { SpotLightReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ScalarNode ConstantAttenuation() const { return ReferenceProperty<ScalarNode>(L"ConstantAttenuation"); }
        ScalarNode LinearAttenuation() const { return ReferenceProperty<ScalarNode>(L"LinearAttenuation"); }
        ScalarNode QuadraticAttentuation() const { return ReferenceProperty<ScalarNode>(L"QuadraticAttentuation"); }
        ScalarNode InnerConeAngle() const { return ReferenceProperty<ScalarNode>(L"InnerConeAngle"); }
        ScalarNode InnerConeAngleInDegrees() const { return ReferenceProperty<ScalarNode>(L"InnerConeAngleInDegrees"); }
        ScalarNode OuterConeAngle() const { return ReferenceProperty<ScalarNode>(L"OuterConeAngle"); }
        ScalarNode OuterConeAngleInDegrees() const { return ReferenceProperty<ScalarNode>(L"OuterConeAngleInDegrees"); }
        ColorNode Color() const { return ReferenceProperty<ColorNode>(L"Color"); }
        ColorNode InnerConeColor() const { return ReferenceProperty<ColorNode>(L"InnerConeColor"); }
        ColorNode OuterConeColor() const { return ReferenceProperty<ColorNode>(L"OuterConeColor"); }
        Vector3Node Direction() const { return ReferenceProperty<Vector3Node>(L"Direction"); }
        Vector3Node Offset() const { return ReferenceProperty<Vector3Node>(L"Offset"); }
    };
}
