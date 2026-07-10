#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/ColorNode.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class ColorBrushReferenceNode final : public ReferenceNodeBase<ColorBrushReferenceNode>
    {
    public:
        ColorBrushReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::CompositionColorBrush const& brush = nullptr) : ReferenceNodeBase(paramName, brush) {}
        static ColorBrushReferenceNode CreateTargetReference() { ColorBrushReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        ColorNode Color() const { return ReferenceProperty<ColorNode>(L"Color"); }
    };
}
