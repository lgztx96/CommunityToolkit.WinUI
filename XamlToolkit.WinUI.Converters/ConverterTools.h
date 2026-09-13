#pragma once

#ifdef __INTELLISENSE__
#include <optional>
#include <variant>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#endif
#include "../XamlToolkit.WinUI/Extensions/Foundation/EqualsHelper.h"

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
	using namespace Microsoft::UI::Xaml::Markup;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
	struct ConverterTools
	{
        using FormatValue = std::variant<std::monostate, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, std::wstring>;

        static bool TryParseBool(winrt::IInspectable const& parameter)
        {
			return ConverterTools::TryConvertValue<bool>(parameter).value_or(false);
        }

        static winrt::IInspectable TryConvertValue(winrt::IInspectable const& value, winrt::TypeName const& targetType)
        {
            if (!value)
            {
                return nullptr;
            }

            try
            {
                return winrt::XamlBindingHelper::ConvertValue(targetType, value);
            }
            catch (...)
            {
                return nullptr;
            }
        }

        template<typename T>
        static std::optional<T> TryConvertValue(winrt::IInspectable const& value)
        {
            if (const auto converted = TryConvertValue(value, winrt::xaml_typename<T>()))
            {
                return converted.template as<T>();
            }

            return std::nullopt;
        }

        static FormatValue TryConvertToFormatValue(winrt::IInspectable const& value)
        {
            if (!value)
            {
                return std::monostate{};
            }

            if (const auto propertyValue = value.try_as<winrt::IPropertyValue>())
            {
                switch (propertyValue.Type())
                {
                case winrt::PropertyType::Boolean:
                    return propertyValue.GetBoolean();

                case winrt::PropertyType::UInt8:
                    return propertyValue.GetUInt8();

                case winrt::PropertyType::Int16:
                    return propertyValue.GetInt16();

                case winrt::PropertyType::UInt16:
                    return propertyValue.GetUInt16();

                case winrt::PropertyType::Int32:
                    return propertyValue.GetInt32();

                case winrt::PropertyType::UInt32:
                    return propertyValue.GetUInt32();

                case winrt::PropertyType::Int64:
                    return propertyValue.GetInt64();

                case winrt::PropertyType::UInt64:
                    return propertyValue.GetUInt64();

                case winrt::PropertyType::Single:
                    return propertyValue.GetSingle();

                case winrt::PropertyType::Double:
                    return propertyValue.GetDouble();

                case winrt::PropertyType::String:
                    return std::wstring(propertyValue.GetString());

                default:
                    break;
                }
            }

            if (const auto valueStr = TryConvertValue<winrt::hstring>(value))
            {
                return std::wstring(*valueStr);
            }

            return std::monostate{};
        }

        static bool AreValuesEqual(winrt::IInspectable const& left, winrt::IInspectable const& right)
        {
            return EqualsHelper::ConvertibleEquals(left, right);
        }
	};
}
