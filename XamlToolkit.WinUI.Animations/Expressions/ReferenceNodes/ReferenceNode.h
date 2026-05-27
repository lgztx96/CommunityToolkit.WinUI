#pragma once

#include "../ExpressionNodes/ExpressionNode.h"
#include "../ExpressionNodes/ExpressionNodeType.h"
#include "../ExpressionForward.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <optional>
#include <memory>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    using namespace Windows::UI::Composition;

    /// <summary>
    /// Base type for nodes that point at composition object references.
    /// </summary>
    class ReferenceNode : public ExpressionNode
    {
    public:
        ReferenceNode() = default;

        ReferenceNode(std::optional<winrt::hstring> paramName, CompositionObject const& compObj = nullptr)
            : Reference(compObj)
        {
            NodeType = ExpressionNodeType::Reference;
            ParamName = std::move(paramName);
        }

        CompositionObject Reference{ nullptr };

        BooleanNode GetBooleanProperty(winrt::hstring const& propertyName) const;
        ScalarNode GetScalarProperty(winrt::hstring const& propertyName) const;
        Vector2Node GetVector2Property(winrt::hstring const& propertyName) const;
        Vector3Node GetVector3Property(winrt::hstring const& propertyName) const;
        Vector4Node GetVector4Property(winrt::hstring const& propertyName) const;
        ColorNode GetColorProperty(winrt::hstring const& propertyName) const;
        QuaternionNode GetQuaternionProperty(winrt::hstring const& propertyName) const;
        Matrix3x2Node GetMatrix3x2Property(winrt::hstring const& propertyName) const;
        Matrix4x4Node GetMatrix4x4Property(winrt::hstring const& propertyName) const;

        std::optional<winrt::hstring> GetReferenceNodeString() const;

    protected:
        template<typename T>
        T ReferenceProperty(winrt::hstring const& propertyName) const
            requires std::is_base_of_v<ExpressionNode, T>
        {
            T newNode = ExpressionNode::CreateExpressionNode<T>();
            newNode.NodeType = ExpressionNodeType::ReferenceProperty;
            newNode.AddChild(*this);
            newNode.PropertyName = propertyName;
            return newNode;
        }

        winrt::hstring GetValue() const override
        {
            throw winrt::hresult_not_implemented(L"GetValue is not implemented for ReferenceNode and shouldn't be called");
        }
    };

    template<typename TDerived>
    class ReferenceNodeBase : public ReferenceNode
    {
    public:
        using ReferenceNode::ReferenceNode;

        std::shared_ptr<ExpressionNode> Clone() const override
        {
            return std::make_shared<TDerived>(static_cast<TDerived const&>(*this));
        }
    };
}
