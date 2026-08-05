#pragma once

#include <initializer_list>
#include "ExpressionNode.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    struct ExpressionNodeHelper final
    {
        template<typename T>
        static T CreateExpressionNode()
            requires std::is_base_of_v<ExpressionNode, T>
        {
            return ExpressionNode::CreateExpressionNode<T>();
        }

        template<typename T>
        static T SubchannelsInternal(ExpressionNode const& node, std::initializer_list<std::wstring_view> subchannels)
            requires std::is_base_of_v<ExpressionNode, T>
        {
            T newNode = ExpressionNode::CreateExpressionNode<T>();
            newNode.NodeType = ExpressionNodeType::Swizzle;
            newNode.AddChild(node);
            newNode.Subchannels.clear();

            for (const auto& channel : subchannels)
            {
                newNode.Subchannels.emplace_back(channel);
            }

            return newNode;
        }
    };
}
