#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/ColorNode.h"
#include "../ExpressionNodes/ScalarNode.h"
#include "../ExpressionNodes/Vector3Node.h"
namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class DropShadowReferenceNode final : public ReferenceNodeBase<DropShadowReferenceNode>
    {
    public:
        DropShadowReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::DropShadow const& source = nullptr) : ReferenceNodeBase(paramName, source) {}
        static DropShadowReferenceNode CreateTargetReference() { DropShadowReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ScalarNode BlurRadius() const { return ReferenceProperty<ScalarNode>(L"BlurRadius"); }
        ScalarNode Opacity() const { return ReferenceProperty<ScalarNode>(L"Opacity"); }
        Vector3Node Offset() const { return ReferenceProperty<Vector3Node>(L"Offset"); }
        ColorNode Color() const { return ReferenceProperty<ColorNode>(L"Color"); }
    };
}
