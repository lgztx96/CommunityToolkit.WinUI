#include "pch.h"
#include "winrt_module_imports.h"

#include "BooleanNode.h"
#include "ColorNode.h"
#include "ExpressionNodeHelper.h"
#include "Matrix3x2Node.h"
#include "Matrix4x4Node.h"
#include "QuaternionNode.h"
#include "ScalarNode.h"
#include "Vector2Node.h"
#include "Vector3Node.h"
#include "Vector4Node.h"

namespace winrt
{
    using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    using winrt::XamlToolkit::WinUI::Animations::FloatExtensions;

    template<typename TEnum>
    static winrt::hstring EnumLabel(TEnum value, std::initializer_list<std::pair<TEnum, std::wstring_view>> map)
    {
        for (auto const& [candidate, label] : map)
        {
            if (candidate == value)
            {
                return winrt::hstring{ label };
            }
        }

        return winrt::hstring{ L"Unknown" };
    }

    Vector2Node::Vector2Node(winrt::float2 const& value) : value(value) { NodeType = ExpressionNodeType::ConstantValue; }
    Vector2Node::Vector2Node(winrt::hstring const& paramName) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; }
    Vector2Node::Vector2Node(winrt::hstring const& paramName, winrt::float2 const& value) : value(value) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; SetVector2Parameter(paramName, value); }
    winrt::hstring Vector2Node::ToString(Subchannel value) { return EnumLabel(value, { {Subchannel::X, L"X"}, {Subchannel::Y, L"Y"} }); }
    ScalarNode Vector2Node::X() const { return GetSubchannels(Subchannel::X); }
    ScalarNode Vector2Node::Y() const { return GetSubchannels(Subchannel::Y); }
    ScalarNode Vector2Node::GetSubchannels(Subchannel s) const { return ExpressionNodeHelper::SubchannelsInternal<ScalarNode>(*this, { ToString(s).c_str() }); }
    Vector2Node Vector2Node::GetSubchannels(Subchannel s1, Subchannel s2) const { return ExpressionNodeHelper::SubchannelsInternal<Vector2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str() }); }
    Vector3Node Vector2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3) const { return ExpressionNodeHelper::SubchannelsInternal<Vector3Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str() }); }
    Vector4Node Vector2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4) const { return ExpressionNodeHelper::SubchannelsInternal<Vector4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str() }); }
    Matrix3x2Node Vector2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix3x2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str() }); }
    Matrix4x4Node Vector2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6, Subchannel s7, Subchannel s8, Subchannel s9, Subchannel s10, Subchannel s11, Subchannel s12, Subchannel s13, Subchannel s14, Subchannel s15, Subchannel s16) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix4x4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str(), ToString(s7).c_str(), ToString(s8).c_str(), ToString(s9).c_str(), ToString(s10).c_str(), ToString(s11).c_str(), ToString(s12).c_str(), ToString(s13).c_str(), ToString(s14).c_str(), ToString(s15).c_str(), ToString(s16).c_str() }); }
    winrt::hstring Vector2Node::GetValue() const { return winrt::format(L"Vector2({},{})", FloatExtensions::ToCompositionString(value.x), FloatExtensions::ToCompositionString(value.y)); }

    Vector3Node::Vector3Node(winrt::float3 const& value) : value(value) { NodeType = ExpressionNodeType::ConstantValue; }
    Vector3Node::Vector3Node(winrt::hstring const& paramName) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; }
    Vector3Node::Vector3Node(winrt::hstring const& paramName, winrt::float3 const& value) : value(value) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; SetVector3Parameter(paramName, value); }
    winrt::hstring Vector3Node::ToString(Subchannel value) { return EnumLabel(value, { {Subchannel::X, L"X"}, {Subchannel::Y, L"Y"}, {Subchannel::Z, L"Z"} }); }
    ScalarNode Vector3Node::X() const { return GetSubchannels(Subchannel::X); }
    ScalarNode Vector3Node::Y() const { return GetSubchannels(Subchannel::Y); }
    ScalarNode Vector3Node::Z() const { return GetSubchannels(Subchannel::Z); }
    Vector2Node Vector3Node::XY() const { return GetSubchannels(Subchannel::X, Subchannel::Y); }
    ScalarNode Vector3Node::GetSubchannels(Subchannel s) const { return ExpressionNodeHelper::SubchannelsInternal<ScalarNode>(*this, { ToString(s).c_str() }); }
    Vector2Node Vector3Node::GetSubchannels(Subchannel s1, Subchannel s2) const { return ExpressionNodeHelper::SubchannelsInternal<Vector2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str() }); }
    Vector3Node Vector3Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3) const { return ExpressionNodeHelper::SubchannelsInternal<Vector3Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str() }); }
    Vector4Node Vector3Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4) const { return ExpressionNodeHelper::SubchannelsInternal<Vector4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str() }); }
    Matrix3x2Node Vector3Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix3x2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str() }); }
    Matrix4x4Node Vector3Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6, Subchannel s7, Subchannel s8, Subchannel s9, Subchannel s10, Subchannel s11, Subchannel s12, Subchannel s13, Subchannel s14, Subchannel s15, Subchannel s16) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix4x4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str(), ToString(s7).c_str(), ToString(s8).c_str(), ToString(s9).c_str(), ToString(s10).c_str(), ToString(s11).c_str(), ToString(s12).c_str(), ToString(s13).c_str(), ToString(s14).c_str(), ToString(s15).c_str(), ToString(s16).c_str() }); }
    winrt::hstring Vector3Node::GetValue() const { return winrt::format(L"Vector3({},{},{})", FloatExtensions::ToCompositionString(value.x), FloatExtensions::ToCompositionString(value.y), FloatExtensions::ToCompositionString(value.z)); }

    Vector4Node::Vector4Node(winrt::float4 const& value) : value(value) { NodeType = ExpressionNodeType::ConstantValue; }
    Vector4Node::Vector4Node(winrt::hstring const& paramName) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; }
    Vector4Node::Vector4Node(winrt::hstring const& paramName, winrt::float4 const& value) : value(value) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; SetVector4Parameter(paramName, value); }
    winrt::hstring Vector4Node::ToString(Subchannel value) { return EnumLabel(value, { {Subchannel::X, L"X"}, {Subchannel::Y, L"Y"}, {Subchannel::Z, L"Z"}, {Subchannel::W, L"W"} }); }
    ScalarNode Vector4Node::X() const { return GetSubchannels(Subchannel::X); }
    ScalarNode Vector4Node::Y() const { return GetSubchannels(Subchannel::Y); }
    ScalarNode Vector4Node::Z() const { return GetSubchannels(Subchannel::Z); }
    ScalarNode Vector4Node::W() const { return GetSubchannels(Subchannel::W); }
    Vector2Node Vector4Node::XY() const { return GetSubchannels(Subchannel::X, Subchannel::Y); }
    Vector3Node Vector4Node::XYZ() const { return GetSubchannels(Subchannel::X, Subchannel::Y, Subchannel::Z); }
    ScalarNode Vector4Node::GetSubchannels(Subchannel s) const { return ExpressionNodeHelper::SubchannelsInternal<ScalarNode>(*this, { ToString(s).c_str() }); }
    Vector2Node Vector4Node::GetSubchannels(Subchannel s1, Subchannel s2) const { return ExpressionNodeHelper::SubchannelsInternal<Vector2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str() }); }
    Vector3Node Vector4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3) const { return ExpressionNodeHelper::SubchannelsInternal<Vector3Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str() }); }
    Vector4Node Vector4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4) const { return ExpressionNodeHelper::SubchannelsInternal<Vector4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str() }); }
    Matrix3x2Node Vector4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix3x2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str() }); }
    Matrix4x4Node Vector4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6, Subchannel s7, Subchannel s8, Subchannel s9, Subchannel s10, Subchannel s11, Subchannel s12, Subchannel s13, Subchannel s14, Subchannel s15, Subchannel s16) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix4x4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str(), ToString(s7).c_str(), ToString(s8).c_str(), ToString(s9).c_str(), ToString(s10).c_str(), ToString(s11).c_str(), ToString(s12).c_str(), ToString(s13).c_str(), ToString(s14).c_str(), ToString(s15).c_str(), ToString(s16).c_str() }); }
    winrt::hstring Vector4Node::GetValue() const { return winrt::format(L"Vector4({},{},{},{})", FloatExtensions::ToCompositionString(value.x), FloatExtensions::ToCompositionString(value.y), FloatExtensions::ToCompositionString(value.z), FloatExtensions::ToCompositionString(value.w)); }

    Matrix3x2Node::Matrix3x2Node(winrt::float3x2 const& value) : value(value) { NodeType = ExpressionNodeType::ConstantValue; }
    Matrix3x2Node::Matrix3x2Node(winrt::hstring const& paramName) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; }
    Matrix3x2Node::Matrix3x2Node(winrt::hstring const& paramName, winrt::float3x2 const& value) : value(value) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; SetMatrix3x2Parameter(paramName, value); }
    winrt::hstring Matrix3x2Node::ToString(Subchannel value) { return EnumLabel(value, { {Subchannel::Channel11, L"M11"}, {Subchannel::Channel12, L"M12"}, {Subchannel::Channel21, L"M21"}, {Subchannel::Channel22, L"M22"}, {Subchannel::Channel31, L"M31"}, {Subchannel::Channel32, L"M32"} }); }
    ScalarNode Matrix3x2Node::Channel11() const { return GetSubchannels(Subchannel::Channel11); }
    ScalarNode Matrix3x2Node::Channel12() const { return GetSubchannels(Subchannel::Channel12); }
    ScalarNode Matrix3x2Node::Channel21() const { return GetSubchannels(Subchannel::Channel21); }
    ScalarNode Matrix3x2Node::Channel22() const { return GetSubchannels(Subchannel::Channel22); }
    ScalarNode Matrix3x2Node::Channel31() const { return GetSubchannels(Subchannel::Channel31); }
    ScalarNode Matrix3x2Node::Channel32() const { return GetSubchannels(Subchannel::Channel32); }
    ScalarNode Matrix3x2Node::GetSubchannels(Subchannel s) const { return ExpressionNodeHelper::SubchannelsInternal<ScalarNode>(*this, { ToString(s).c_str() }); }
    Vector2Node Matrix3x2Node::GetSubchannels(Subchannel s1, Subchannel s2) const { return ExpressionNodeHelper::SubchannelsInternal<Vector2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str() }); }
    Vector3Node Matrix3x2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3) const { return ExpressionNodeHelper::SubchannelsInternal<Vector3Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str() }); }
    Vector4Node Matrix3x2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4) const { return ExpressionNodeHelper::SubchannelsInternal<Vector4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str() }); }
    Matrix3x2Node Matrix3x2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix3x2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str() }); }
    Matrix4x4Node Matrix3x2Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6, Subchannel s7, Subchannel s8, Subchannel s9, Subchannel s10, Subchannel s11, Subchannel s12, Subchannel s13, Subchannel s14, Subchannel s15, Subchannel s16) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix4x4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str(), ToString(s7).c_str(), ToString(s8).c_str(), ToString(s9).c_str(), ToString(s10).c_str(), ToString(s11).c_str(), ToString(s12).c_str(), ToString(s13).c_str(), ToString(s14).c_str(), ToString(s15).c_str(), ToString(s16).c_str() }); }
    winrt::hstring Matrix3x2Node::GetValue() const { return winrt::format(L"Matrix3x2({},{},{},{},{},{})", FloatExtensions::ToCompositionString(value.m11), FloatExtensions::ToCompositionString(value.m12), FloatExtensions::ToCompositionString(value.m21), FloatExtensions::ToCompositionString(value.m22), FloatExtensions::ToCompositionString(value.m31), FloatExtensions::ToCompositionString(value.m32)); }

    Matrix4x4Node::Matrix4x4Node(winrt::float4x4 const& value) : value(value) { NodeType = ExpressionNodeType::ConstantValue; }
    Matrix4x4Node::Matrix4x4Node(winrt::hstring const& paramName) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; }
    Matrix4x4Node::Matrix4x4Node(winrt::hstring const& paramName, winrt::float4x4 const& value) : value(value) { ParamName = paramName; NodeType = ExpressionNodeType::ConstantParameter; SetMatrix4x4Parameter(paramName, value); }
    winrt::hstring Matrix4x4Node::ToString(Subchannel value) { return EnumLabel(value, { {Subchannel::Channel11, L"M11"}, {Subchannel::Channel12, L"M12"}, {Subchannel::Channel13, L"M13"}, {Subchannel::Channel14, L"M14"}, {Subchannel::Channel21, L"M21"}, {Subchannel::Channel22, L"M22"}, {Subchannel::Channel23, L"M23"}, {Subchannel::Channel24, L"M24"}, {Subchannel::Channel31, L"M31"}, {Subchannel::Channel32, L"M32"}, {Subchannel::Channel33, L"M33"}, {Subchannel::Channel34, L"M34"}, {Subchannel::Channel41, L"M41"}, {Subchannel::Channel42, L"M42"}, {Subchannel::Channel43, L"M43"}, {Subchannel::Channel44, L"M44"} }); }
    ScalarNode Matrix4x4Node::Channel11() const { return GetSubchannels(Subchannel::Channel11); } ScalarNode Matrix4x4Node::Channel12() const { return GetSubchannels(Subchannel::Channel12); } ScalarNode Matrix4x4Node::Channel13() const { return GetSubchannels(Subchannel::Channel13); } ScalarNode Matrix4x4Node::Channel14() const { return GetSubchannels(Subchannel::Channel14); }
    ScalarNode Matrix4x4Node::Channel21() const { return GetSubchannels(Subchannel::Channel21); } ScalarNode Matrix4x4Node::Channel22() const { return GetSubchannels(Subchannel::Channel22); } ScalarNode Matrix4x4Node::Channel23() const { return GetSubchannels(Subchannel::Channel23); } ScalarNode Matrix4x4Node::Channel24() const { return GetSubchannels(Subchannel::Channel24); }
    ScalarNode Matrix4x4Node::Channel31() const { return GetSubchannels(Subchannel::Channel31); } ScalarNode Matrix4x4Node::Channel32() const { return GetSubchannels(Subchannel::Channel32); } ScalarNode Matrix4x4Node::Channel33() const { return GetSubchannels(Subchannel::Channel33); } ScalarNode Matrix4x4Node::Channel34() const { return GetSubchannels(Subchannel::Channel34); }
    ScalarNode Matrix4x4Node::Channel41() const { return GetSubchannels(Subchannel::Channel41); } ScalarNode Matrix4x4Node::Channel42() const { return GetSubchannels(Subchannel::Channel42); } ScalarNode Matrix4x4Node::Channel43() const { return GetSubchannels(Subchannel::Channel43); } ScalarNode Matrix4x4Node::Channel44() const { return GetSubchannels(Subchannel::Channel44); }
    Vector3Node Matrix4x4Node::Channel11Channel22Channel33() const { return GetSubchannels(Subchannel::Channel11, Subchannel::Channel22, Subchannel::Channel33); }
    Vector3Node Matrix4x4Node::Channel41Channel42Channel43() const { return GetSubchannels(Subchannel::Channel41, Subchannel::Channel42, Subchannel::Channel43); }
    ScalarNode Matrix4x4Node::GetSubchannels(Subchannel s) const { return ExpressionNodeHelper::SubchannelsInternal<ScalarNode>(*this, { ToString(s).c_str() }); }
    Vector2Node Matrix4x4Node::GetSubchannels(Subchannel s1, Subchannel s2) const { return ExpressionNodeHelper::SubchannelsInternal<Vector2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str() }); }
    Vector3Node Matrix4x4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3) const { return ExpressionNodeHelper::SubchannelsInternal<Vector3Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str() }); }
    Vector4Node Matrix4x4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4) const { return ExpressionNodeHelper::SubchannelsInternal<Vector4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str() }); }
    Matrix3x2Node Matrix4x4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix3x2Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str() }); }
    Matrix4x4Node Matrix4x4Node::GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6, Subchannel s7, Subchannel s8, Subchannel s9, Subchannel s10, Subchannel s11, Subchannel s12, Subchannel s13, Subchannel s14, Subchannel s15, Subchannel s16) const { return ExpressionNodeHelper::SubchannelsInternal<Matrix4x4Node>(*this, { ToString(s1).c_str(), ToString(s2).c_str(), ToString(s3).c_str(), ToString(s4).c_str(), ToString(s5).c_str(), ToString(s6).c_str(), ToString(s7).c_str(), ToString(s8).c_str(), ToString(s9).c_str(), ToString(s10).c_str(), ToString(s11).c_str(), ToString(s12).c_str(), ToString(s13).c_str(), ToString(s14).c_str(), ToString(s15).c_str(), ToString(s16).c_str() }); }
    winrt::hstring Matrix4x4Node::GetValue() const { return winrt::format(L"Matrix4x4({},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{})", FloatExtensions::ToCompositionString(value.m11), FloatExtensions::ToCompositionString(value.m12), FloatExtensions::ToCompositionString(value.m13), FloatExtensions::ToCompositionString(value.m14), FloatExtensions::ToCompositionString(value.m21), FloatExtensions::ToCompositionString(value.m22), FloatExtensions::ToCompositionString(value.m23), FloatExtensions::ToCompositionString(value.m24), FloatExtensions::ToCompositionString(value.m31), FloatExtensions::ToCompositionString(value.m32), FloatExtensions::ToCompositionString(value.m33), FloatExtensions::ToCompositionString(value.m34), FloatExtensions::ToCompositionString(value.m41), FloatExtensions::ToCompositionString(value.m42), FloatExtensions::ToCompositionString(value.m43), FloatExtensions::ToCompositionString(value.m44)); }
}
