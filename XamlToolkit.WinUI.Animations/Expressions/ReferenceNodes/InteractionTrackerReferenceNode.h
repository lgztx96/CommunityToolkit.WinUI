#pragma once
#include "ReferenceNode.h"
#include "../ExpressionNodes/BooleanNode.h"
#include "../ExpressionNodes/ScalarNode.h"
#include "../ExpressionNodes/Vector3Node.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Composition.Interactions.h>
#else
import winrt.Microsoft.UI.Composition.Interactions;
#endif

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class InteractionTrackerReferenceNode final : public ReferenceNodeBase<InteractionTrackerReferenceNode>
    {
    public:
        InteractionTrackerReferenceNode(std::optional<winrt::hstring> paramName = std::nullopt, winrt::Microsoft::UI::Composition::Interactions::InteractionTracker const& it = nullptr) : ReferenceNodeBase(paramName, it) {}
        static InteractionTrackerReferenceNode CreateTargetReference() { InteractionTrackerReferenceNode node; node.NodeType = ExpressionNodeType::TargetReference; return node; }
        BooleanNode IsPositionRoundingSuggested() const { return ReferenceProperty<BooleanNode>(L"IsPositionRoundingSuggested"); }
        ScalarNode MinScale() const { return ReferenceProperty<ScalarNode>(L"MinScale"); }
        ScalarNode MaxScale() const { return ReferenceProperty<ScalarNode>(L"MaxScale"); }
        ScalarNode NaturalRestingScale() const { return ReferenceProperty<ScalarNode>(L"NaturalRestingScale"); }
        ScalarNode Scale() const { return ReferenceProperty<ScalarNode>(L"Scale"); }
        ScalarNode ScaleInertiaDecayRate() const { return ReferenceProperty<ScalarNode>(L"ScaleInertiaDecayRate"); }
        ScalarNode ScaleVelocityInPercentPerSecond() const { return ReferenceProperty<ScalarNode>(L"ScaleVelocityInPercentPerSecond"); }
        Vector3Node MinPosition() const { return ReferenceProperty<Vector3Node>(L"MinPosition"); }
        Vector3Node MaxPosition() const { return ReferenceProperty<Vector3Node>(L"MaxPosition"); }
        Vector3Node NaturalRestingPosition() const { return ReferenceProperty<Vector3Node>(L"NaturalRestingPosition"); }
        Vector3Node Position() const { return ReferenceProperty<Vector3Node>(L"Position"); }
        Vector3Node PositionInertiaDecayRate() const { return ReferenceProperty<Vector3Node>(L"PositionInertiaDecayRate"); }
        Vector3Node PositionVelocityInPixelsPerSecond() const { return ReferenceProperty<Vector3Node>(L"PositionVelocityInPixelsPerSecond"); }
    };
}
