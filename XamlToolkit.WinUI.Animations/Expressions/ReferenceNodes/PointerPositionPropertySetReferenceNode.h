#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/Vector3Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class PointerPositionPropertySetReferenceNode final : public ReferenceNodeBase<PointerPositionPropertySetReferenceNode>
    {
    public:
        PointerPositionPropertySetReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::CompositionPropertySet const& ic = nullptr) : ReferenceNodeBase(paramName, ic) {}
        static PointerPositionPropertySetReferenceNode CreateTargetReference() { PointerPositionPropertySetReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        Vector3Node Position() const { return ReferenceProperty<Vector3Node >(L"Position"); }
    };
}
