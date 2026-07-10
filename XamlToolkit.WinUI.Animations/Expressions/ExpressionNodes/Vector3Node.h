#pragma once

#include "ExpressionNode.h"
#include "ExpressionNodeType.h"
#include "../../Extensions/System/FloatExtensions.h"
#include "../ExpressionForward.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Numerics.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    using namespace Windows::Foundation::Numerics;

    class Vector3Node final : public ExpressionNodeBase<Vector3Node>
    {
    public:
        enum class Subchannel { X, Y, Z };

        Vector3Node() = default;
        explicit Vector3Node(winrt::float3 const& value);
        explicit Vector3Node(winrt::hstring const& paramName);
        Vector3Node(winrt::hstring const& paramName, winrt::float3 const& value);

        ScalarNode X() const;
        ScalarNode Y() const;
        ScalarNode Z() const;
        Vector2Node XY() const;
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
        winrt::float3 value{};
    };
}
