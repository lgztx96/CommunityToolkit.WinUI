#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <optional>
#endif

#include "ReferenceNode.h"
#include "../ExpressionNodes/BooleanNode.h"
#include "../ExpressionNodes/ColorNode.h"
#include "../ExpressionNodes/Matrix3x2Node.h"
#include "../ExpressionNodes/Matrix4x4Node.h"
#include "../ExpressionNodes/QuaternionNode.h"
#include "../ExpressionNodes/ScalarNode.h"
#include "../ExpressionNodes/Vector2Node.h"
#include "../ExpressionNodes/Vector3Node.h"
#include "../ExpressionNodes/Vector4Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    BooleanNode ReferenceNode::GetBooleanProperty(winrt::hstring const& propertyName) const { return ReferenceProperty<BooleanNode>(propertyName); }
    ScalarNode ReferenceNode::GetScalarProperty(winrt::hstring const& propertyName) const { return ReferenceProperty<ScalarNode>(propertyName); }
    Vector2Node ReferenceNode::GetVector2Property(winrt::hstring const& propertyName) const { return ReferenceProperty<Vector2Node>(propertyName); }
    Vector3Node ReferenceNode::GetVector3Property(winrt::hstring const& propertyName) const { return ReferenceProperty<Vector3Node>(propertyName); }
    Vector4Node ReferenceNode::GetVector4Property(winrt::hstring const& propertyName) const { return ReferenceProperty<Vector4Node>(propertyName); }
    ColorNode ReferenceNode::GetColorProperty(winrt::hstring const& propertyName) const { return ReferenceProperty<ColorNode>(propertyName); }
    QuaternionNode ReferenceNode::GetQuaternionProperty(winrt::hstring const& propertyName) const { return ReferenceProperty<QuaternionNode>(propertyName); }
    Matrix3x2Node ReferenceNode::GetMatrix3x2Property(winrt::hstring const& propertyName) const { return ReferenceProperty<Matrix3x2Node>(propertyName); }
    Matrix4x4Node ReferenceNode::GetMatrix4x4Property(winrt::hstring const& propertyName) const { return ReferenceProperty<Matrix4x4Node>(propertyName); }

    std::optional<winrt::hstring> ReferenceNode::GetReferenceNodeString() const
    {
        if (NodeType == ExpressionNodeType::TargetReference)
        {
            return winrt::hstring{ L"this.target" };
        }

        return NodeName;
    }
}
