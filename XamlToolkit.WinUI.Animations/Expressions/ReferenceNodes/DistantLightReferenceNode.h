#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/ColorNode.h"
#include "../ExpressionNodes/Vector3Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class DistantLightReferenceNode final : public ReferenceNodeBase<DistantLightReferenceNode>
    {
    public:
        DistantLightReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::DistantLight const& light = nullptr) : ReferenceNodeBase(paramName, light) {}
        static DistantLightReferenceNode CreateTargetReference() { DistantLightReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ColorNode Color() const { return ReferenceProperty<ColorNode>(L"Color"); }
        Vector3Node Direction() const { return ReferenceProperty<Vector3Node>(L"Direction"); }
    };
}
