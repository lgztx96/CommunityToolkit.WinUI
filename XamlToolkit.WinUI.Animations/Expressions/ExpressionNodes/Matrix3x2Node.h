#pragma once

#include "ExpressionNode.h"
#include "ExpressionNodeType.h"
#include "../../Extensions/System/FloatExtensions.h"
#include "../ExpressionForward.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    using namespace Windows::Foundation::Numerics;

    class Matrix3x2Node final : public ExpressionNodeBase<Matrix3x2Node>
    {
    public:
        enum class Subchannel { Channel11, Channel12, Channel21, Channel22, Channel31, Channel32 };

        Matrix3x2Node() = default;
        explicit Matrix3x2Node(float3x2 const& value);
        explicit Matrix3x2Node(winrt::hstring const& paramName);
        Matrix3x2Node(winrt::hstring const& paramName, float3x2 const& value);

        ScalarNode Channel11() const;
        ScalarNode Channel12() const;
        ScalarNode Channel21() const;
        ScalarNode Channel22() const;
        ScalarNode Channel31() const;
        ScalarNode Channel32() const;
        ScalarNode GetSubchannels(Subchannel s) const;
        Vector2Node GetSubchannels(Subchannel s1, Subchannel s2) const;
        Vector3Node GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3) const;
        Vector4Node GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4) const;
        Matrix3x2Node GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6) const;
        Matrix4x4Node GetSubchannels(Subchannel s1, Subchannel s2, Subchannel s3, Subchannel s4, Subchannel s5, Subchannel s6, Subchannel s7, Subchannel s8, Subchannel s9, Subchannel s10, Subchannel s11, Subchannel s12, Subchannel s13, Subchannel s14, Subchannel s15, Subchannel s16) const;

        static winrt::hstring ToString(Subchannel value);

    protected:
        winrt::hstring GetValue() const override;

    private:
        float3x2 value{};
    };
}
