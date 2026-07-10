#pragma once

#include "ExpressionNode.h"
#include "ExpressionNodeType.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    class ColorNode final : public ExpressionNodeBase<ColorNode>
    {
    public:
        ColorNode() = default;

        explicit ColorNode(winrt::Windows::UI::Color const& value)
            : value(value)
        {
            NodeType = ExpressionNodeType::ConstantValue;
        }

        explicit ColorNode(winrt::hstring const& paramName)
        {
            ParamName = paramName;
            NodeType = ExpressionNodeType::ConstantParameter;
        }

        ColorNode(winrt::hstring const& paramName, winrt::Windows::UI::Color const& value)
            : value(value)
        {
            ParamName = paramName;
            NodeType = ExpressionNodeType::ConstantParameter;
            SetColorParameter(paramName, value);
        }

    protected:
        winrt::hstring GetValue() const override
        {
            return winrt::format(L"ColorRgb({},{},{},{})", value.A, value.R, value.G, value.B);
        }

    private:
        winrt::Windows::UI::Color value{};
    };
}
