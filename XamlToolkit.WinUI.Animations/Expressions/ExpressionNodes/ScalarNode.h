#pragma once

#include "ExpressionNode.h"
#include "ExpressionNodeType.h"
#include "../../Extensions/System/FloatExtensions.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class ScalarNode final : public ExpressionNodeBase<ScalarNode>
    {
    public:
        ScalarNode() = default;

        explicit ScalarNode(float value)
            : value(value)
        {
            NodeType = ExpressionNodeType::ConstantValue;
        }

        explicit ScalarNode(int value)
            : value(static_cast<float>(value))
        {
            NodeType = ExpressionNodeType::ConstantValue;
        }

        explicit ScalarNode(winrt::hstring const& paramName)
        {
            ParamName = paramName;
            NodeType = ExpressionNodeType::ConstantParameter;
        }

        ScalarNode(winrt::hstring const& paramName, float value)
            : value(value)
        {
            ParamName = paramName;
            NodeType = ExpressionNodeType::ConstantParameter;
            SetScalarParameter(paramName, value);
        }

    protected:
        winrt::hstring GetValue() const override
        {
            return winrt::XamlToolkit::WinUI::Animations::FloatExtensions::ToCompositionString(value);
        }

    private:
        float value{};
    };
}
