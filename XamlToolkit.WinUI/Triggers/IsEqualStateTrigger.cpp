#include "pch.h"
#include "winrt_module_imports.h"
#include "IsEqualStateTrigger.h"
#if __has_include("IsEqualStateTrigger.g.cpp")
#include "IsEqualStateTrigger.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <cmath>
#include <limits>
#include <variant>
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    void IsEqualStateTrigger::UpdateTrigger()
    {
        SetActive(AreValuesEqual(Value(), To(), true));
    }

    void IsEqualStateTrigger::OnValuePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        winrt::get_self<IsEqualStateTrigger>(d.as<class_type>())->UpdateTrigger();
    }

    bool IsEqualStateTrigger::AreValuesEqual(winrt::IInspectable const& value1, winrt::IInspectable const& value2, bool convertType)
    {
        if (value1 == value2)
        {
            return true;
        }

        if (value1 && value2 && convertType)
        {
            return ConvertTypeEquals(value1, value2) || ConvertTypeEquals(value2, value1);
        }

        return false;
    }

    bool IsEqualStateTrigger::ConvertTypeEquals(winrt::IInspectable const& value1, winrt::IInspectable const& value2)
    {
        auto v1 = value1.try_as<winrt::IPropertyValue>();
        auto v2 = value2.try_as<winrt::IPropertyValue>();
        if (!v1 || !v2)
        {
            return false;
        }

        auto typeA = v1.Type();
        auto typeB = v2.Type();

        // Float/Double comparison with epsilon
        auto toDouble = [](winrt::IPropertyValue const& v) -> double
        {
            switch (v.Type())
            {
            case winrt::PropertyType::Single: return static_cast<double>(v.GetSingle());
            case winrt::PropertyType::Double: return v.GetDouble();
            default: return std::numeric_limits<double>::quiet_NaN();
            }
        };

        if ((typeA == winrt::PropertyType::Single || typeA == winrt::PropertyType::Double) &&
            (typeB == winrt::PropertyType::Single || typeB == winrt::PropertyType::Double))
        {
            return std::fabs(toDouble(v1) - toDouble(v2)) < 1e-9;
        }

        // Integer comparison with proper signed/unsigned handling
        auto isInteger = [](PropertyType t)
        {
            switch (t)
            {
            case winrt::PropertyType::UInt8:
            case winrt::PropertyType::Int16:
            case winrt::PropertyType::UInt16:
            case winrt::PropertyType::Int32:
            case winrt::PropertyType::UInt32:
            case winrt::PropertyType::Int64:
            case winrt::PropertyType::UInt64:
                return true;
            default:
                return false;
            }
        };

        auto toInteger = [](winrt::IPropertyValue const& v) -> std::variant<int64_t, uint64_t>
        {
            switch (v.Type())
            {
            case winrt::PropertyType::UInt8:  return static_cast<uint64_t>(v.GetUInt8());
            case winrt::PropertyType::UInt16: return static_cast<uint64_t>(v.GetUInt16());
            case winrt::PropertyType::UInt32: return static_cast<uint64_t>(v.GetUInt32());
            case winrt::PropertyType::UInt64: return v.GetUInt64();
            case winrt::PropertyType::Int16:  return static_cast<int64_t>(v.GetInt16());
            case winrt::PropertyType::Int32:  return static_cast<int64_t>(v.GetInt32());
            case winrt::PropertyType::Int64:  return v.GetInt64();
            default: std::unreachable();
            }
        };

        if (isInteger(typeA) && isInteger(typeB))
        {
            auto i1 = toInteger(v1);
            auto i2 = toInteger(v2);

            return std::visit([](auto a, auto b) -> bool
            {
                using A = decltype(a);
                using B = decltype(b);
                if constexpr (std::signed_integral<A> && std::signed_integral<B>)
                    return a == b;
                else if constexpr (std::unsigned_integral<A> && std::unsigned_integral<B>)
                    return a == b;
                else if constexpr (std::signed_integral<A> && std::unsigned_integral<B>)
                    return a >= 0 && static_cast<uint64_t>(a) == b;
                else if constexpr (std::unsigned_integral<A> && std::signed_integral<B>)
                    return b >= 0 && a == static_cast<uint64_t>(b);
                else
                    return false;
            }, i1, i2);
        }

        // Enum comparison (OtherType with matching class names)
        if (typeA == winrt::PropertyType::OtherType && typeB == winrt::PropertyType::OtherType)
        {
            if (winrt::get_class_name(v1) == winrt::get_class_name(v2))
            {
                auto maybeEnumA = v1.try_as<uint64_t>();
                auto maybeEnumB = v2.try_as<uint64_t>();
                if (maybeEnumA && maybeEnumB)
                {
                    return *maybeEnumA == *maybeEnumB;
                }
            }
        }

        if (typeA != typeB)
        {
            return false;
        }

        switch (typeA)
        {
        case winrt::PropertyType::Empty:    return true;
        case winrt::PropertyType::Boolean:  return v1.GetBoolean() == v2.GetBoolean();
        case winrt::PropertyType::Char16:   return v1.GetChar16() == v2.GetChar16();
        case winrt::PropertyType::String:   return v1.GetString() == v2.GetString();
        case winrt::PropertyType::Guid:     return v1.GetGuid() == v2.GetGuid();
        case winrt::PropertyType::DateTime: return v1.GetDateTime() == v2.GetDateTime();
        case winrt::PropertyType::TimeSpan: return v1.GetTimeSpan() == v2.GetTimeSpan();
        case winrt::PropertyType::Point:    return v1.GetPoint() == v2.GetPoint();
        case winrt::PropertyType::Size:     return v1.GetSize() == v2.GetSize();
        case winrt::PropertyType::Rect:     return v1.GetRect() == v2.GetRect();
        default: return false;
        }
    }
}
