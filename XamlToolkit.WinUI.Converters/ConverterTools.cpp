#include "pch.h"
#include "ConverterTools.h"
#include <variant>

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Markup;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
#define CMP_ARRAY(type, method) \
{ \
    winrt::com_array<type> a1, a2; \
    valueA.method(a1); \
    valueB.method(a2); \
    return a1 == a2; \
}

	bool ConverterTools::TryParseBool(winrt::IInspectable const& parameter)
	{
		if (auto value = parameter.try_as<winrt::hstring>())
		{
			if (*value == L"True" || *value == L"true" || *value == L"TRUE")
			{
				return true;
			}
		}

		return false;
	}

	winrt::IInspectable ConverterTools::Convert(winrt::IInspectable const& value, winrt::TypeName const& targetType)
	{
		if (auto type = winrt::TypeName{ winrt::get_class_name(value) }; type == targetType)
		{
			return value;
		}

		return winrt::XamlBindingHelper::ConvertValue(targetType, value);
	}

	std::optional<winrt::hstring> ConverterTools::TryConvertToString(winrt::IInspectable const& value)
	{
		if (!value)
		{
			return std::nullopt;
		}

		auto converted = Convert(value, winrt::xaml_typename<winrt::hstring>());

		return converted.try_as<winrt::hstring>();
	}

	bool ConverterTools::ValueEquals(winrt::IPropertyValue const& valueA, winrt::IPropertyValue const& valueB)
	{
		auto typeA = valueA.Type();
		auto typeB = valueB.Type();

		auto toDouble = [](winrt::IPropertyValue const& v) -> double
			{
				switch (v.Type())
				{
				case PropertyType::Single: return static_cast<double>(v.GetSingle());
				case PropertyType::Double: return v.GetDouble();
				default: return std::numeric_limits<double>::quiet_NaN();
				}
			};

		auto almost_equal = [](double x, double y)
			{
				if (std::isnan(x) || std::isnan(y)) return false;
				if (std::isinf(x) || std::isinf(y)) return x == y;

				double diff = std::fabs(x - y);
				double norm = std::max<double>({ 1.0, std::fabs(x), std::fabs(y) });
				return diff < 1e-9 * norm;
			};

		if ((typeA == PropertyType::Single || typeA == PropertyType::Double) &&
			(typeB == PropertyType::Single || typeB == PropertyType::Double))
		{
			return almost_equal(toDouble(valueA), toDouble(valueB));
		}

		auto isInteger = [](PropertyType t)
			{
				switch (t)
				{
				case PropertyType::UInt8:
				case PropertyType::Int16:
				case PropertyType::UInt16:
				case PropertyType::Int32:
				case PropertyType::UInt32:
				case PropertyType::Int64:
				case PropertyType::UInt64:
					return true;
				default:
					return false;
				}
			};

		auto toInteger = [](winrt::IPropertyValue const& v) -> std::variant<int64_t, uint64_t>
			{
				switch (v.Type())
				{
				case PropertyType::UInt8:  return static_cast<uint64_t>(v.GetUInt8());
				case PropertyType::UInt16: return static_cast<uint64_t>(v.GetUInt16());
				case PropertyType::UInt32: return static_cast<uint64_t>(v.GetUInt32());
				case PropertyType::UInt64: return v.GetUInt64();
				case PropertyType::Int16:  return static_cast<int64_t>(v.GetInt16());
				case PropertyType::Int32:  return static_cast<int64_t>(v.GetInt32());
				case PropertyType::Int64:  return v.GetInt64();
				default: std::unreachable();
				}
			};

		if (isInteger(typeA) && isInteger(typeB))
		{
			auto i1 = toInteger(valueA);
			auto i2 = toInteger(valueB);

			return std::visit([](auto x, auto y)
				{
					using A = decltype(x);
					using B = decltype(y);

					if constexpr (std::signed_integral<A> && std::signed_integral<B>)
						return x == y;
					else if constexpr (std::unsigned_integral<A> && std::unsigned_integral<B>)
						return x == y;
					else if constexpr (std::signed_integral<A> && std::unsigned_integral<B>)
						return x >= 0 && static_cast<uint64_t>(x) == y;
					else if constexpr (std::unsigned_integral<A> && std::signed_integral<B>)
						return y >= 0 && x == static_cast<uint64_t>(y);
					else
						return false;
				}, i1, i2);
		}

		if (typeA != typeB)
			return false;

		switch (typeA)
		{
		case PropertyType::Empty:    return true;
		case PropertyType::Boolean:  return valueA.GetBoolean() == valueB.GetBoolean();
		case PropertyType::Char16:   return valueA.GetChar16() == valueB.GetChar16();
		case PropertyType::String:   return valueA.GetString() == valueB.GetString();
		case PropertyType::Guid:     return valueA.GetGuid() == valueB.GetGuid();
		case PropertyType::DateTime: return valueA.GetDateTime() == valueB.GetDateTime();
		case PropertyType::TimeSpan: return valueA.GetTimeSpan() == valueB.GetTimeSpan();
		case PropertyType::Point:    return valueA.GetPoint() == valueB.GetPoint();
		case PropertyType::Size:     return valueA.GetSize() == valueB.GetSize();
		case PropertyType::Rect:     return valueA.GetRect() == valueB.GetRect();
		case PropertyType::UInt8Array:   CMP_ARRAY(uint8_t, GetUInt8Array);
		case PropertyType::Int16Array:   CMP_ARRAY(int16_t, GetInt16Array);
		case PropertyType::UInt16Array:  CMP_ARRAY(uint16_t, GetUInt16Array);
		case PropertyType::Int32Array:   CMP_ARRAY(int32_t, GetInt32Array);
		case PropertyType::UInt32Array:  CMP_ARRAY(uint32_t, GetUInt32Array);
		case PropertyType::Int64Array:   CMP_ARRAY(int64_t, GetInt64Array);
		case PropertyType::UInt64Array:  CMP_ARRAY(uint64_t, GetUInt64Array);
		case PropertyType::SingleArray:  CMP_ARRAY(float, GetSingleArray);
		case PropertyType::DoubleArray:  CMP_ARRAY(double, GetDoubleArray);
		case PropertyType::Char16Array:  CMP_ARRAY(char16_t, GetChar16Array);
		case PropertyType::BooleanArray: CMP_ARRAY(bool, GetBooleanArray);
		case PropertyType::StringArray:  CMP_ARRAY(winrt::hstring, GetStringArray);
		case PropertyType::GuidArray:    CMP_ARRAY(winrt::guid, GetGuidArray);
		case PropertyType::DateTimeArray:CMP_ARRAY(winrt::Windows::Foundation::DateTime, GetDateTimeArray);
		case PropertyType::TimeSpanArray:CMP_ARRAY(winrt::Windows::Foundation::TimeSpan, GetTimeSpanArray);
		case PropertyType::PointArray:   CMP_ARRAY(winrt::Windows::Foundation::Point, GetPointArray);
		case PropertyType::SizeArray:    CMP_ARRAY(winrt::Windows::Foundation::Size, GetSizeArray);
		case PropertyType::RectArray:    CMP_ARRAY(winrt::Windows::Foundation::Rect, GetRectArray);

		case PropertyType::OtherType:
		{
			auto ia = valueA.as<winrt::IInspectable>();
			auto ib = valueB.as<winrt::IInspectable>();

			auto enumA = valueA.try_as<uint64_t>();
			auto enumB = valueB.try_as<uint64_t>();

			if (enumA && enumB)
				return *enumA == *enumB;

			return ia == ib;
		}

		default:
			return false;
		}
	}
}