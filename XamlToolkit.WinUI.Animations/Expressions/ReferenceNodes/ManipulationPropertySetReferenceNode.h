#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/Matrix4x4Node.h"
#include "../ExpressionNodes/Vector3Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class ManipulationPropertySetReferenceNode final : public ReferenceNodeBase<ManipulationPropertySetReferenceNode>
    {
    public:
        ManipulationPropertySetReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::CompositionPropertySet const& ic = nullptr) : ReferenceNodeBase(paramName, ic) {}
        static ManipulationPropertySetReferenceNode CreateTargetReference() { ManipulationPropertySetReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        Vector3Node CenterPoint() const { return ReferenceProperty<Vector3Node >(L"CenterPoint"); }
        Vector3Node Pan() const { return ReferenceProperty<Vector3Node>(L"Pan"); }
        Vector3Node Scale() const { return ReferenceProperty<Vector3Node>(L"Scale"); }
        Vector3Node Translation() const { return ReferenceProperty<Vector3Node>(L"Translation"); }
        Matrix4x4Node Matrix() const { return ReferenceProperty<Matrix4x4Node>(L"Matrix"); }
    };
}
