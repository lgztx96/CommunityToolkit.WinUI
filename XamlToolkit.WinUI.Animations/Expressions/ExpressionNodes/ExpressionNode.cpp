#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <map>
#include <set>
#include <vector>
#endif

#include "ExpressionNode.h"
#include "../ExpressionFunctions.h"
#include "../ReferenceNodes/ReferenceNode.h"
#include "../../Extensions/System/FloatExtensions.h"

namespace winrt
{
    using namespace Windows::UI;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    void ExpressionNode::AddChild(ExpressionNode const& child)
    {
        Children.push_back(child.Clone());
    }

    void ExpressionNode::SetReferenceParameter(winrt::hstring const& parameterName, winrt::CompositionObject const& compObj)
    {
        EnsureReferenceInfo();

        for (auto& entry : *referenceInfo)
        {
            if (_wcsicmp(entry.ParameterName.c_str(), parameterName.c_str()) == 0)
            {
                entry.CompObject = compObj;
            }
        }
    }

    void ExpressionNode::SetBooleanParameter(winrt::hstring const& parameterName, bool value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetScalarParameter(winrt::hstring const& parameterName, float value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetVector2Parameter(winrt::hstring const& parameterName, winrt::float2 const& value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetVector3Parameter(winrt::hstring const& parameterName, winrt::float3 const& value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetVector4Parameter(winrt::hstring const& parameterName, winrt::float4 const& value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetColorParameter(winrt::hstring const& parameterName, winrt::Color const& value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetQuaternionParameter(winrt::hstring const& parameterName, winrt::quaternion const& value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetMatrix3x2Parameter(winrt::hstring const& parameterName, winrt::float3x2 const& value) { constantParameters[parameterName] = winrt::box_value(value); }
    void ExpressionNode::SetMatrix4x4Parameter(winrt::hstring const& parameterName, winrt::float4x4 const& value) { constantParameters[parameterName] = winrt::box_value(value); }

    void ExpressionNode::Dispose()
    {
        referenceInfo.reset();
        compObjToNodeNameMap.reset();
        constantParameters.clear();
        PropertyName.reset();
        NodeType = ExpressionNodeType::Count;
        Children.clear();
        ParamName.reset();
        NodeName.reset();
        ExpressionAnimation = nullptr;
        Subchannels.clear();
    }

    winrt::hstring ExpressionNode::ToExpressionString()
    {
        if (!referenceInfo.has_value())
        {
            EnsureReferenceInfo();
        }

        return ToExpressionStringInternal();
    }

    void ExpressionNode::ClearReferenceInfo()
    {
        referenceInfo.reset();
        compObjToNodeNameMap.reset();
        NodeName.reset();

        for (auto const& child : Children)
        {
            child->ClearReferenceInfo();
        }
    }

    void ExpressionNode::EnsureReferenceInfo()
    {
        if (referenceInfo.has_value())
        {
            return;
        }

        std::set<ReferenceNode*> referenceNodes;
        std::map<winrt::hstring, winrt::IInspectable, std::less<>> rootParameters = constantParameters;
        PopulateParameterNodes(rootParameters, referenceNodes);
        constantParameters = std::move(rootParameters);

        std::set<winrt::CompositionObject> compositionObjects;

        for (auto* refNode : referenceNodes)
        {
            if (refNode->Reference && !refNode->GetReferenceNodeString().has_value())
            {
                compositionObjects.insert(refNode->Reference);
            }
        }

        compObjToNodeNameMap = std::map<winrt::CompositionObject, winrt::hstring>{};
        uint32_t paramCount = 0;

        for (auto const& compObj : compositionObjects)
        {
            winrt::hstring nodeName = (ParamName.has_value() && !ParamName->empty()) ? *ParamName : CreateUniqueNodeNameFromIndex(paramCount++);
            compObjToNodeNameMap->emplace(compObj, nodeName);
        }

        referenceInfo = std::vector<ReferenceInfo>{};

        for (auto* refNode : referenceNodes)
        {
            auto nodeName = refNode->GetReferenceNodeString();

            if (refNode->Reference == nullptr && !nodeName.has_value())
            {
                throw winrt::hresult_invalid_argument(L"Reference and nodeName can't both be null");
            }

            if (!nodeName.has_value())
            {
                nodeName = compObjToNodeNameMap->at(refNode->Reference);
            }

            referenceInfo->push_back(ReferenceInfo{ *nodeName, refNode->Reference });
            refNode->NodeName = nodeName;
        }
    }

    winrt::hstring ExpressionNode::CreateUniqueNodeNameFromIndex(uint32_t index)
    {
        constexpr uint32_t alphabetLength = 26;
        std::wstring characters;
        characters.reserve(8);

        characters.push_back(static_cast<wchar_t>(L'A' + (index % alphabetLength)));

        while ((index /= alphabetLength) > 0)
        {
            index--;
            characters.push_back(static_cast<wchar_t>(L'A' + (index % alphabetLength)));
        }

        return winrt::hstring{ characters };
    }

    void ExpressionNode::SetAllParameters(winrt::CompositionAnimation const& animation)
    {
        EnsureReferenceInfo();

        for (auto const& ref : *referenceInfo)
        {
            animation.SetReferenceParameter(ref.ParameterName, ref.CompObject);
        }

        for (auto const& [key, value] : constantParameters)
        {
            if (auto boxedBool = value.try_as<winrt::IReference<bool>>())
            {
                animation.SetBooleanParameter(key, boxedBool.Value());
            }
            else if (auto boxedScalar = value.try_as<winrt::IReference<float>>())
            {
                animation.SetScalarParameter(key, boxedScalar.Value());
            }
            else if (auto boxedVector2 = value.try_as<winrt::IReference<winrt::float2>>())
            {
                animation.SetVector2Parameter(key, boxedVector2.Value());
            }
            else if (auto boxedVector3 = value.try_as<winrt::IReference<winrt::float3>>())
            {
                animation.SetVector3Parameter(key, boxedVector3.Value());
            }
            else if (auto boxedVector4 = value.try_as<winrt::IReference<winrt::float4>>())
            {
                animation.SetVector4Parameter(key, boxedVector4.Value());
            }
            else if (auto boxedColor = value.try_as<winrt::IReference<winrt::Color>>())
            {
                animation.SetColorParameter(key, boxedColor.Value());
            }
            else if (auto boxedQuaternion = value.try_as<winrt::IReference<winrt::quaternion>>())
            {
                animation.SetQuaternionParameter(key, boxedQuaternion.Value());
            }
            else if (auto boxedMatrix3x2 = value.try_as<winrt::IReference<winrt::float3x2>>())
            {
                animation.SetMatrix3x2Parameter(key, boxedMatrix3x2.Value());
            }
            else if (auto boxedMatrix4x4 = value.try_as<winrt::IReference<winrt::float4x4>>())
            {
                animation.SetMatrix4x4Parameter(key, boxedMatrix4x4.Value());
            }
            else
            {
                throw winrt::hresult_invalid_argument(L"Unexpected constant parameter datatype");
            }
        }
    }

    void ExpressionNode::PopulateParameterNodes(
        std::map<winrt::hstring, winrt::IInspectable, std::less<>>& constParamMap,
        std::set<ReferenceNode*>& referenceNodes)
    {
        if (auto* refNode = dynamic_cast<ReferenceNode*>(this))
        {
            if (refNode->NodeType != ExpressionNodeType::TargetReference)
            {
                referenceNodes.insert(refNode);
            }
        }

        for (auto const& [key, value] : constantParameters)
        {
            if (!constParamMap.contains(key))
            {
                constParamMap.emplace(key, value);
            }
        }

        for (auto const& child : Children)
        {
            child->PopulateParameterNodes(constParamMap, referenceNodes);
        }
    }

    winrt::hstring ExpressionNode::ToExpressionStringInternal()
    {
        auto const nodeInfo = ExpressionFunctions::GetNodeInfoFromType(NodeType);
        winrt::hstring result;

        switch (nodeInfo.NodeOperationKind)
        {
        case OperationType::Function:
            if (Children.empty())
            {
                throw winrt::hresult_invalid_argument(L"Can't have an expression function with no params");
            }

            result = *nodeInfo.OperationString + L"(" + Children[0]->ToExpressionStringInternal();
            for (size_t i = 1; i < Children.size(); ++i)
            {
                result = result + L"," + Children[i]->ToExpressionStringInternal();
            }
            result = result + L")";
            break;

        case OperationType::Operator:
            if (Children.size() != 2)
            {
                throw winrt::hresult_invalid_argument(L"Can't have an operator that doesn't have exactly 2 params");
            }

            result = L"(" + Children[0]->ToExpressionStringInternal() + L" " + *nodeInfo.OperationString + L" " + Children[1]->ToExpressionStringInternal() + L")";
            break;

        case OperationType::UnaryOperator:
            if (Children.size() != 1)
            {
                throw winrt::hresult_invalid_argument(L"Can't have a unary operator that doesn't have exactly one param");
            }

            result = L"( " + *nodeInfo.OperationString + L" " + Children[0]->ToExpressionStringInternal() + L" )";
            break;

        case OperationType::Constant:
            if (!Children.empty())
            {
                throw winrt::hresult_invalid_argument(L"Constants must have 0 children");
            }

            result = ParamName.has_value() ? *ParamName : GetValue();
            break;

        case OperationType::Swizzle:
            if (Children.size() != 1)
            {
                throw winrt::hresult_invalid_argument(L"Swizzles should have exactly 1 child");
            }

            {
                winrt::hstring swizzle;
                for (auto const& sub : Subchannels)
                {
                    swizzle = swizzle + sub;
                }
                result = Children[0]->ToExpressionStringInternal() + L"." + swizzle;
            }
            break;

        case OperationType::Reference:
            if (NodeType == ExpressionNodeType::Reference || NodeType == ExpressionNodeType::TargetReference)
            {
                if (!Children.empty())
                {
                    throw winrt::hresult_invalid_argument(L"References cannot have children");
                }

                auto* refNode = dynamic_cast<ReferenceNode*>(this);
                result = refNode->GetReferenceNodeString().value_or(L"");
            }
            else if (NodeType == ExpressionNodeType::ReferenceProperty)
            {
                if (Children.size() != 1)
                {
                    throw winrt::hresult_invalid_argument(L"Reference properties must have exactly one child");
                }
                if (!PropertyName.has_value())
                {
                    throw winrt::hresult_invalid_argument(L"Reference properties must have a property name");
                }

                result = Children[0]->ToExpressionStringInternal() + L"." + *PropertyName;
            }
            else if (NodeType == ExpressionNodeType::StartingValueProperty)
            {
                if (!Children.empty())
                {
                    throw winrt::hresult_invalid_argument(L"StartingValue references cannot have children");
                }
                result = L"this.StartingValue";
            }
            else if (NodeType == ExpressionNodeType::CurrentValueProperty)
            {
                if (!Children.empty())
                {
                    throw winrt::hresult_invalid_argument(L"CurrentValue references cannot have children");
                }
                result = L"this.CurrentValue";
            }
            else
            {
                throw winrt::hresult_invalid_argument(L"Unexpected NodeType for OperationType.Reference");
            }
            break;

        case OperationType::Conditional:
            if (Children.size() != 3)
            {
                throw winrt::hresult_invalid_argument(L"Conditionals must have exactly 3 children");
            }
            result = L"((" + Children[0]->ToExpressionStringInternal() + L") ? (" + Children[1]->ToExpressionStringInternal() + L") : (" + Children[2]->ToExpressionStringInternal() + L"))";
            break;

        default:
            throw winrt::hresult_invalid_argument(L"Unexpected operation type");
        }

        return result;
    }
}
