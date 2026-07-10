#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/ScalarNode.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class NineGridBrushReferenceNode final : public ReferenceNodeBase<NineGridBrushReferenceNode>
    {
    public:
        NineGridBrushReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::CompositionNineGridBrush const& brush = nullptr) : ReferenceNodeBase(paramName, brush) {}
        static NineGridBrushReferenceNode CreateTargetReference() { NineGridBrushReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ScalarNode BottomInset() const { return ReferenceProperty<ScalarNode>(L"BottomInset"); }
        ScalarNode BottomInsetScale() const { return ReferenceProperty<ScalarNode>(L"BottomInsetScale"); }
        ScalarNode LeftInset() const { return ReferenceProperty<ScalarNode>(L"LeftInset"); }
        ScalarNode LeftInsetScale() const { return ReferenceProperty<ScalarNode>(L"LeftInsetScale"); }
        ScalarNode RightInset() const { return ReferenceProperty<ScalarNode>(L"RightInset"); }
        ScalarNode RightInsetScale() const { return ReferenceProperty<ScalarNode>(L"RightInsetScale"); }
        ScalarNode TopInset() const { return ReferenceProperty<ScalarNode>(L"TopInset"); }
        ScalarNode TopInsetScale() const { return ReferenceProperty<ScalarNode>(L"TopInsetScale"); }
    };
}
