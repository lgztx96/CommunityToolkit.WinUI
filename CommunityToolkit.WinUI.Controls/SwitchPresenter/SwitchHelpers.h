#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/CommunityToolkit.WinUI.Controls.h>
#include <variant>

namespace winrt::CommunityToolkit::WinUI::Controls
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Microsoft::UI::Xaml::Markup;

	class SwitchHelpers
	{
	public:
		static bool ValueEquals(IPropertyValue const& valueA, IPropertyValue const& valueB)
		{
			auto typeA = valueA.Type();
			auto typeB = valueB.Type();

			auto toDouble = [](IPropertyValue const& v) -> double
			{
				switch (v.Type()) {
				case PropertyType::Single: return static_cast<double>(v.GetSingle());
				case PropertyType::Double: return v.GetDouble();
				default: return std::numeric_limits<double>::quiet_NaN();
				}
			};

			if ((typeA == PropertyType::Single || typeA == PropertyType::Double) &&
				(typeB == PropertyType::Single || typeB == PropertyType::Double))
			{
				return std::fabs(toDouble(valueA) - toDouble(valueB)) < 1e-9;
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

			auto toInteger = [](IPropertyValue const& v) -> std::variant<int64_t, uint64_t>
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

			if (typeA != typeB)
				return false;

			if (typeA == PropertyType::OtherType)
			{
				if (winrt::get_class_name(valueA) == winrt::get_class_name(valueB))
				{
					auto maybeEnumA = valueA.try_as<uint64_t>();
					auto maybeEnumB = valueB.try_as<uint64_t>();
					if (maybeEnumA && maybeEnumB)
					{
						return *maybeEnumA == *maybeEnumB;
					}
				}
			}

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
			default:
				return false;
			}
		}

		static bool Equals(IInspectable const& objA, IInspectable const& objB)
		{
			if (objA == objB) return true;

			auto valueA = objA.try_as<IPropertyValue>();
			auto valueB = objB.try_as<IPropertyValue>();
			if (!valueA || !valueB)
				return false;

			return ValueEquals(valueA, valueB);
		}

		static Case EvaluateCases(CaseCollection const& switchCases, IInspectable const& value, std::optional<winrt::Windows::UI::Xaml::Interop::TypeName> const& targetType)
		{
			if (switchCases == nullptr ||
				switchCases.Size() == 0)
			{
				// If we have no cases, then we can't match anything.
				return nullptr;
			}

			Case xdefault = nullptr;
			Case newcase = nullptr;

			auto collection = switchCases
				| std::views::transform([](auto&& item) { return item.template as<Case>(); });

			for (const Case& xcase : collection)
			{
				if (xcase.IsDefault())
				{
					// If there are multiple default cases provided, this will override and just grab the last one, the developer will have to fix this in their XAML. We call this out in the case comments.
					xdefault = xcase;
					continue;
				}

				if (CompareValues(value, xcase.Value(), targetType))
				{
					newcase = xcase;
					break;
				}
			}

			if (newcase == nullptr && xdefault != nullptr)
			{
				// Inject default if we found one without matching anything
				newcase = xdefault;
			}

			return newcase;
		}

		static bool CompareValues(IInspectable const& compare, IInspectable const& value, std::optional<winrt::Windows::UI::Xaml::Interop::TypeName> const& targetType)
		{
			if (compare == nullptr || value == nullptr)
			{
				return compare == value;
			}

			if (!targetType ||
				(targetType->Name == winrt::get_class_name(compare) && targetType->Name == winrt::get_class_name(value)))
			{
				// Default direct object comparison or we're all the proper type
				return SwitchHelpers::Equals(compare, value);
			}
			else if (winrt::get_class_name(compare) == targetType->Name)
			{
				// If we have a TargetType and the first value is the right type
				// Then our 2nd value isn't, so convert to string and coerce.
				auto valueBase2 = SwitchHelpers::ConvertValue(*targetType, value);

				return SwitchHelpers::Equals(compare, valueBase2);
			}

			// Neither of our two values matches the type so
			// we'll convert both to a String and try and coerce it to the proper type.
			auto compareBase = SwitchHelpers::ConvertValue(*targetType, compare);

			auto valueBase = SwitchHelpers::ConvertValue(*targetType, value);

			return SwitchHelpers::Equals(compareBase, valueBase);
		}

		/// <summary>
		/// Helper method to convert a value from a source type to a target type.
		/// </summary>
		/// <param name="targetType">The target type</param>
		/// <param name="value">The value to convert</param>
		/// <returns>The converted value</returns>
		static IInspectable ConvertValue(winrt::Windows::UI::Xaml::Interop::TypeName targetType, IInspectable const& value)
		{
		    //if (targetType.IsInstanceOfType(value))
		    //{
		    //    return value;
		    //}
		    //else if (targetType.IsEnum && value is string str)
		    //{
		    //    if (Enum.TryParse(targetType, str, out object ? result))
		    //    {
		    //        return result!;
		    //    }

		    //    static auto ThrowExceptionForKeyNotFound = []() -> IInspectable
		    //        {
		    //            throw winrt::hresult_invalid_argument(L"The requested enum value was not present in the provided type.");
		    //        };

		    //    return ThrowExceptionForKeyNotFound();
		    //}
		    //else
		    {
		        return XamlBindingHelper::ConvertValue(targetType, value);
		    }
		}
	};
}