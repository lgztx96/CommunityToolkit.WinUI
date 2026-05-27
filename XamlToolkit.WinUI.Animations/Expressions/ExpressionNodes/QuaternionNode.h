#pragma once

#include "ExpressionNode.h"
#include "ExpressionNodeType.h"
#include "../../Extensions/System/FloatExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    using namespace Windows::Foundation::Numerics;

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
                winrt::XamlToolkit::WinUI::Animations::FloatExtensions::ToCompositionString(value.x),
                winrt::XamlToolkit::WinUI::Animations::FloatExtensions::ToCompositionString(value.y),
                winrt::XamlToolkit::WinUI::Animations::FloatExtensions::ToCompositionString(value.z),
                winrt::XamlToolkit::WinUI::Animations::FloatExtensions::ToCompositionString(value.w));
        }

    private:
        quaternion value{};
    };
}
