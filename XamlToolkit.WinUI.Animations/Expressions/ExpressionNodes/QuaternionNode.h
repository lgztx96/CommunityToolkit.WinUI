#pragma once

#include "ExpressionNode.h"
#include "ExpressionNodeType.h"
#include "../../Extensions/System/FloatExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Numerics.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation::Numerics;
    using namespace XamlToolkit::WinUI::Animations;
}

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class QuaternionNode final : public ExpressionNodeBase<QuaternionNode>
    {
    public:
        QuaternionNode() = default;

        explicit QuaternionNode(quaternion const& value)
            : value(value)
        {
            NodeType = ExpressionNodeType::ConstantValue;
        }

        explicit QuaternionNode(winrt::hstring const& paramName)
        {
            ParamName = paramName;
            NodeType = ExpressionNodeType::ConstantParameter;
        }

        QuaternionNode(winrt::hstring const& paramName, quaternion const& value)
            : value(value)
        {
            ParamName = paramName;
            NodeType = ExpressionNodeType::ConstantParameter;
            SetQuaternionParameter(paramName, value);
        }

    protected:
        winrt::hstring GetValue() const override
        {
            return winrt::format(
                L"Quaternion({},{},{},{})",
                winrt::FloatExtensions::ToCompositionString(value.x),
                winrt::FloatExtensions::ToCompositionString(value.y),
                winrt::FloatExtensions::ToCompositionString(value.z),
                winrt::FloatExtensions::ToCompositionString(value.w));
        }

    private:
        winrt::quaternion value{};
    };
}
