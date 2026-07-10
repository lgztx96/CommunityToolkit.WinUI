#pragma once

#include "ReferenceNode.h"
#include "../ExpressionNodes/ColorNode.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class AmbientLightReferenceNode final : public ReferenceNodeBase<AmbientLightReferenceNode>
    {
    public:
        AmbientLightReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::AmbientLight const& light = nullptr) : ReferenceNodeBase(paramName, light) {}
        static AmbientLightReferenceNode CreateTargetReference() { AmbientLightReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ColorNode Color() const { return ReferenceProperty<ColorNode>(L"Color"); }
    };
}
