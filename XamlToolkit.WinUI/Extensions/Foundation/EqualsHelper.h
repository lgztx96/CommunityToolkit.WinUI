#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <array>
#include <charconv>
#include <concepts>
#include <optional>
#include <string_view>
#else
import std;
import winrt.Windows.Foundation;
#endif

namespace winrt
{
	using namespace ::winrt::Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI
{
	struct EqualsHelper
	{
		static constexpr bool IsInteger(winrt::PropertyType type) noexcept
		{
			switch (type)
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
		}

		static constexpr bool IsFloatingPoint(winrt::PropertyType type) noexcept
		{
			return type == winrt::PropertyType::Single ||
				type == winrt::PropertyType::Double;
		}

		static constexpr bool IsNumeric(winrt::PropertyType type) noexcept
		{
			return IsInteger(type) || IsFloatingPoint(type);
		}

		template<std::integral A, std::integral B>
		static constexpr bool IntegerEquals(A a, B b) noexcept
		{
			if constexpr (std::is_signed_v<A> == std::is_signed_v<B>)
			{
				return a == b;
			}
			else if constexpr (std::signed_integral<A>)
			{
				return a >= 0 &&
					static_cast<std::make_unsigned_t<A>>(a) == b;
			}
			else
			{
				return b >= 0 &&
					a == static_cast<std::make_unsigned_t<B>>(b);
			}
		}

		template<std::integral I, std::floating_point F>
		static constexpr bool IntegerFloatEquals(I i, F f) noexcept
		{
			if (!std::isfinite(f) || std::trunc(f) != f)
			{
				return false;
			}

			if constexpr (
				std::numeric_limits<F>::digits <
				std::numeric_limits<I>::digits)
			{
				if constexpr (std::signed_integral<I>)
				{
					if (f < std::ldexp(
						F{ 1 },
						std::numeric_limits<I>::digits) *
						-1 ||
						f >= std::ldexp(
							F{ 1 },
							std::numeric_limits<I>::digits))
					{
						return false;
					}
				}
				else
				{
					if (f < F{ 0 } ||
						f >= std::ldexp(
							F{ 1 },
							std::numeric_limits<I>::digits))
					{
						return false;
					}
				}

				return static_cast<I>(f) == i;
			}
			else
			{
				return static_cast<F>(i) == f;
			}
		}

		static bool IntegerEquals(
			winrt::IPropertyValue const& left,
			winrt::PropertyType lt,
			winrt::IPropertyValue const& right,
			winrt::PropertyType rt)
		{
			auto compare = [&]<typename T>(T value)
			{
				switch (rt)
				{
				case winrt::PropertyType::UInt8:
					return IntegerEquals(value, right.GetUInt8());

				case winrt::PropertyType::Int16:
					return IntegerEquals(value, right.GetInt16());

				case winrt::PropertyType::UInt16:
					return IntegerEquals(value, right.GetUInt16());

				case winrt::PropertyType::Int32:
					return IntegerEquals(value, right.GetInt32());

				case winrt::PropertyType::UInt32:
					return IntegerEquals(value, right.GetUInt32());

				case winrt::PropertyType::Int64:
					return IntegerEquals(value, right.GetInt64());

				case winrt::PropertyType::UInt64:
					return IntegerEquals(value, right.GetUInt64());

				default:
					return false;
				}
			};

			switch (lt)
			{
			case winrt::PropertyType::UInt8:
				return compare(left.GetUInt8());

			case winrt::PropertyType::Int16:
				return compare(left.GetInt16());

			case winrt::PropertyType::UInt16:
				return compare(left.GetUInt16());

			case winrt::PropertyType::Int32:
				return compare(left.GetInt32());

			case winrt::PropertyType::UInt32:
				return compare(left.GetUInt32());

			case winrt::PropertyType::Int64:
				return compare(left.GetInt64());

			case winrt::PropertyType::UInt64:
				return compare(left.GetUInt64());

			default:
				return false;
			}
		}

		static bool IntegerFloatEquals(
			winrt::IPropertyValue const& integerValue,
			winrt::PropertyType integerType,
			winrt::IPropertyValue const& floatValue,
			winrt::PropertyType floatType)
		{
			auto compare = [&]<typename T>(T value)
			{
				switch (floatType)
				{
				case winrt::PropertyType::Single:
					return IntegerFloatEquals(value, floatValue.GetSingle());

				case winrt::PropertyType::Double:
					return IntegerFloatEquals(value, floatValue.GetDouble());

				default:
					return false;
				}
			};

			switch (integerType)
			{
			case winrt::PropertyType::UInt8:
				return compare(integerValue.GetUInt8());

			case winrt::PropertyType::Int16:
				return compare(integerValue.GetInt16());

			case winrt::PropertyType::UInt16:
				return compare(integerValue.GetUInt16());

			case winrt::PropertyType::Int32:
				return compare(integerValue.GetInt32());

			case winrt::PropertyType::UInt32:
				return compare(integerValue.GetUInt32());

			case winrt::PropertyType::Int64:
				return compare(integerValue.GetInt64());

			case winrt::PropertyType::UInt64:
				return compare(integerValue.GetUInt64());

			default:
				return false;
			}
		}

		static bool FloatingPointEquals(
			winrt::IPropertyValue const& left,
			winrt::PropertyType lt,
			winrt::IPropertyValue const& right,
			winrt::PropertyType rt)
		{
			auto compare = [&]<typename T>(T value)
			{
				switch (rt)
				{
				case winrt::PropertyType::Single:
					return value == right.GetSingle();

				case winrt::PropertyType::Double:
					return value == right.GetDouble();

				default:
					return false;
				}
			};

			switch (lt)
			{
			case winrt::PropertyType::Single:
				return compare(left.GetSingle());

			case winrt::PropertyType::Double:
				return compare(left.GetDouble());

			default:
				return false;
			}
		}

		static bool NumericEquals(
			winrt::IPropertyValue const& left,
			winrt::PropertyType lt,
			winrt::IPropertyValue const& right,
			winrt::PropertyType rt)
		{
			if (IsInteger(lt) && IsInteger(rt))
			{
				return IntegerEquals(left, lt, right, rt);
			}

			if (IsInteger(lt) && IsFloatingPoint(rt))
			{
				return IntegerFloatEquals(left, lt, right, rt);
			}

			if (IsFloatingPoint(lt) && IsInteger(rt))
			{
				return IntegerFloatEquals(right, rt, left, lt);
			}

			if (IsFloatingPoint(lt) && IsFloatingPoint(rt))
			{
				return FloatingPointEquals(left, lt, right, rt);
			}

			return false;
		}

		static bool TryParseBoolean(std::wstring_view value, bool& result)
		{
			if (value == L"true" || value == L"True" || value == L"TRUE")
			{
				result = true;
				return true;
			}

			if (value == L"false" || value == L"False" || value == L"FALSE")
			{
				result = false;
				return true;
			}

			return false;
		}

		template<size_t N, typename T>
		static bool TryParseChars(std::wstring_view value, T& result)
		{
			std::array<char, N> buffer;

			if (value.size() > buffer.size())
			{
				return false;
			}

			for (size_t i = 0; i < value.size(); ++i)
			{
				buffer[i] = static_cast<char>(value[i]);
			}

			auto [ptr, ec] =
				std::from_chars(
					buffer.data(),
					buffer.data() + value.size(),
					result);

			return ec == std::errc{} &&
				ptr == buffer.data() + value.size();
		}

		template<std::integral T>
		static bool TryParseInteger(std::wstring_view value, T& result)
		{
			return TryParseChars<32>(value, result);
		}

		template<std::floating_point T>
		static bool TryParseFloat(std::wstring_view value, T& result)
		{
			return TryParseChars<64>(value, result);
		}

		static bool TryParseGuid(std::wstring_view value, winrt::guid& result)
		{
			try
			{
				result = winrt::guid(value);
				return true;
			}
			catch (...)
			{
				return false;
			}
		}

		static std::optional<bool> TryStringConvertibleEquals(
			winrt::IPropertyValue const& left,
			winrt::PropertyType lt,
			winrt::IPropertyValue const& right,
			winrt::PropertyType rt)
		{
			if (lt != winrt::PropertyType::String &&
				rt != winrt::PropertyType::String)
			{
				return std::nullopt;
			}

			auto text = lt == winrt::PropertyType::String
				? left.GetString()
				: right.GetString();

			auto compare =
				[&](winrt::PropertyType type,
					winrt::IPropertyValue const& value)
				-> std::optional<bool>
				{
					switch (type)
					{
					case winrt::PropertyType::Boolean:
					{
						bool v;

						if (!TryParseBoolean(text, v))
							return std::nullopt;

						return v == value.GetBoolean();
					}

					case winrt::PropertyType::UInt8:
					{
						uint8_t v;

						if (!TryParseInteger(text, v))
							return std::nullopt;

						return v == value.GetUInt8();
					}

					case winrt::PropertyType::Int16:
					{
						int16_t v;

						if (!TryParseInteger(text, v))
							return std::nullopt;

						return v == value.GetInt16();
					}

					case winrt::PropertyType::UInt16:
					{
						uint16_t v;

						if (!TryParseInteger(text, v))
							return std::nullopt;

						return v == value.GetUInt16();
					}

					case winrt::PropertyType::Int32:
					{
						int32_t v;

						if (!TryParseInteger(text, v))
							return std::nullopt;

						return v == value.GetInt32();
					}

					case winrt::PropertyType::UInt32:
					{
						uint32_t v;

						if (!TryParseInteger(text, v))
							return std::nullopt;

						return v == value.GetUInt32();
					}

					case winrt::PropertyType::Int64:
					{
						int64_t v;

						if (!TryParseInteger(text, v))
							return std::nullopt;

						return v == value.GetInt64();
					}

					case winrt::PropertyType::UInt64:
					{
						uint64_t v;

						if (!TryParseInteger(text, v))
							return std::nullopt;

						return v == value.GetUInt64();
					}

					case winrt::PropertyType::Single:
					{
						float v;

						if (!TryParseFloat(text, v))
							return std::nullopt;

						return v == value.GetSingle();
					}

					case winrt::PropertyType::Double:
					{
						double v;

						if (!TryParseFloat(text, v))
							return std::nullopt;

						return v == value.GetDouble();
					}

					case winrt::PropertyType::Guid:
					{
						winrt::guid v;

						if (!TryParseGuid(text, v))
							return std::nullopt;

						return v == value.GetGuid();
					}

					default:
						return std::nullopt;
					}
				};

			if (lt == winrt::PropertyType::String)
			{
				return compare(rt, right);
			}

			return compare(lt, left);
		}

		static bool ValueTypeEquals(
			winrt::IPropertyValue const& left,
			winrt::IPropertyValue const& right,
			winrt::PropertyType type)
		{
			switch (type)
			{
			case winrt::PropertyType::UInt8:
				return left.GetUInt8() == right.GetUInt8();

			case winrt::PropertyType::Int16:
				return left.GetInt16() == right.GetInt16();

			case winrt::PropertyType::UInt16:
				return left.GetUInt16() == right.GetUInt16();

			case winrt::PropertyType::Int32:
				return left.GetInt32() == right.GetInt32();

			case winrt::PropertyType::UInt32:
				return left.GetUInt32() == right.GetUInt32();

			case winrt::PropertyType::Int64:
				return left.GetInt64() == right.GetInt64();

			case winrt::PropertyType::UInt64:
				return left.GetUInt64() == right.GetUInt64();

			case winrt::PropertyType::Single:
				return left.GetSingle() == right.GetSingle();

			case winrt::PropertyType::Double:
				return left.GetDouble() == right.GetDouble();

			case winrt::PropertyType::Boolean:
				return left.GetBoolean() == right.GetBoolean();

			case winrt::PropertyType::String:
				return left.GetString() == right.GetString();

			case winrt::PropertyType::Guid:
				return left.GetGuid() == right.GetGuid();

			case winrt::PropertyType::DateTime:
				return left.GetDateTime() == right.GetDateTime();

			case winrt::PropertyType::TimeSpan:
				return left.GetTimeSpan() == right.GetTimeSpan();

			case winrt::PropertyType::Point:
				return left.GetPoint() == right.GetPoint();

			case winrt::PropertyType::Size:
				return left.GetSize() == right.GetSize();

			case winrt::PropertyType::Rect:
				return left.GetRect() == right.GetRect();

			case winrt::PropertyType::Char16:
				return left.GetChar16() == right.GetChar16();

			case winrt::PropertyType::OtherType:
			{
				if (left.IsNumericScalar() && right.IsNumericScalar())
				{
					return winrt::get_class_name(left) ==
						winrt::get_class_name(right) &&
						left.GetUInt32() == right.GetUInt32();
				}
			}
			[[fallthrough]];

			default:
				throw winrt::hresult_not_implemented();
			}
		}

		static bool AreEqual(winrt::IInspectable const& left, winrt::IInspectable const& right)
		{
			if (left == right)
			{
				return true;
			}

			if (!left || !right)
			{
				return false;
			}

			auto leftPv = left.try_as<winrt::IPropertyValue>();
			auto rightPv = right.try_as<winrt::IPropertyValue>();

			if (leftPv == nullptr || rightPv == nullptr)
			{
				return false;
			}

			auto leftType = leftPv.Type();
			auto rightType = rightPv.Type();

			if (leftType != rightType)
			{
				return false;
			}

			return ValueTypeEquals(leftPv, rightPv, leftType);
		}

		static bool ConvertibleEquals(
			winrt::IInspectable const& left,
			winrt::IInspectable const& right)
		{
			if (left == right)
			{
				return true;
			}

			if (!left || !right)
			{
				return false;
			}

			auto leftPv = left.try_as<winrt::IPropertyValue>();
			auto rightPv = right.try_as<winrt::IPropertyValue>();

			if (!leftPv || !rightPv)
			{
				throw winrt::hresult_not_implemented();
			}

			auto lt = leftPv.Type();
			auto rt = rightPv.Type();

			if (lt == rt)
			{
				return ValueTypeEquals(leftPv, rightPv, lt);
			}

			if (IsNumeric(lt) && IsNumeric(rt))
			{
				return NumericEquals(leftPv, lt, rightPv, rt);
			}

			if (auto result =
				TryStringConvertibleEquals(
					leftPv,
					lt,
					rightPv,
					rt))
			{
				return *result;
			}

			throw winrt::hresult_not_implemented();
		}
	};
}