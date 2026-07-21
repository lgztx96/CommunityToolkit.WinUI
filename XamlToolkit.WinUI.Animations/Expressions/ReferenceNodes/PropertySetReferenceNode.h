#pragma once
#include "ReferenceNode.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class PropertySetReferenceNode : public ReferenceNodeBase<PropertySetReferenceNode>
    {
    public:
        PropertySetReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::CompositionPropertySet const& ps = nullptr) : ReferenceNodeBase(paramName, ps), Source(ps) {}
        winrt::Microsoft::UI::Composition::CompositionPropertySet Source{ nullptr };
        static PropertySetReferenceNode CreateTargetReference() { PropertySetReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
    };
}
