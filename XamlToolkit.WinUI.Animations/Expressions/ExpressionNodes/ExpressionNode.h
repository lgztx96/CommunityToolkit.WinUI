#pragma once

#include "../ExpressionForward.h"
#include "ExpressionNodeType.h"
#include "ValueKeywordKind.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <memory>
#include <vector>
#include <optional>
#include <map>
#include <set>
#include <concepts>
#include <functional>
#endif

namespace winrt
{
    using namespace Windows::UI;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    struct ReferenceInfo
    {
        winrt::hstring ParameterName;
        winrt::CompositionObject CompObject{ nullptr };
    };

    /// <summary>
    /// Base type for all expression tree nodes.
    /// </summary>
    class ExpressionNode
    {
    public:
        ExpressionNode() = default;
        virtual ~ExpressionNode() = default;

        ExpressionNode(ExpressionNode const&) = default;
        ExpressionNode(ExpressionNode&&) noexcept = default;
        ExpressionNode& operator=(ExpressionNode const&) = default;
        ExpressionNode& operator=(ExpressionNode&&) noexcept = default;

        template<typename T>
        static T CreateExpressionNode()
            requires std::is_base_of_v<ExpressionNode, T>
        {
            return T();
        }

        template<typename T>
        static T CreateValueKeyword(ValueKeywordKind keywordKind)
            requires std::is_base_of_v<ExpressionNode, T>
        {
            T node = CreateExpressionNode<T>();
            node.ParamName.reset();

            switch (keywordKind)
            {
            case ValueKeywordKind::CurrentValue:
                node.NodeType = ExpressionNodeType::CurrentValueProperty;
                break;
            case ValueKeywordKind::StartingValue:
                node.NodeType = ExpressionNodeType::StartingValueProperty;
                break;
            default:
                throw winrt::hresult_invalid_argument(L"Invalid ValueKeywordKind");
            }

            return node;
        }

        virtual std::shared_ptr<ExpressionNode> Clone() const = 0;

        void AddChild(ExpressionNode const& child);

        void SetReferenceParameter(winrt::hstring const& parameterName, winrt::CompositionObject const& compObj);
        void SetBooleanParameter(winrt::hstring const& parameterName, bool value);
        void SetScalarParameter(winrt::hstring const& parameterName, float value);
        void SetVector2Parameter(winrt::hstring const& parameterName, winrt::float2 const& value);
        void SetVector3Parameter(winrt::hstring const& parameterName, winrt::float3 const& value);
        void SetVector4Parameter(winrt::hstring const& parameterName, winrt::float4 const& value);
        void SetColorParameter(winrt::hstring const& parameterName, winrt::Color const& value);
        void SetQuaternionParameter(winrt::hstring const& parameterName, winrt::quaternion const& value);
        void SetMatrix3x2Parameter(winrt::hstring const& parameterName, winrt::float3x2 const& value);
        void SetMatrix4x4Parameter(winrt::hstring const& parameterName, winrt::float4x4 const& value);

        void Dispose();
        winrt::hstring ToExpressionString();
        void ClearReferenceInfo();
        void EnsureReferenceInfo();
        void SetAllParameters(winrt::CompositionAnimation const& animation);

        std::optional<winrt::hstring> PropertyName;
        ExpressionNodeType NodeType{ ExpressionNodeType::Count };
        std::vector<std::shared_ptr<ExpressionNode>> Children;
        std::optional<winrt::hstring> ParamName;
        std::optional<winrt::hstring> NodeName;
        winrt::ExpressionAnimation ExpressionAnimation{ nullptr };
        std::vector<winrt::hstring> Subchannels;

    protected:
        virtual winrt::hstring GetValue() const = 0;
        void PopulateParameterNodes(
            std::map<winrt::hstring, winrt::IInspectable, std::less<>>& constParamMap,
            std::set<ReferenceNode*>& referenceNodes);

    private:
        static winrt::hstring CreateUniqueNodeNameFromIndex(uint32_t index);
        winrt::hstring ToExpressionStringInternal();

        std::optional<std::vector<ReferenceInfo>> referenceInfo;
        std::optional<std::map<winrt::CompositionObject, winrt::hstring>> compObjToNodeNameMap;
        std::map<winrt::hstring, winrt::IInspectable, std::less<>> constantParameters;
    };

    template<typename TDerived>
    /// <summary>
    /// Base type for all expression tree nodes.
    /// </summary>
    class ExpressionNodeBase : public ExpressionNode
    {
    public:
        std::shared_ptr<ExpressionNode> Clone() const override
        {
            return std::make_shared<TDerived>(static_cast<TDerived const&>(*this));
        }
    };
}
