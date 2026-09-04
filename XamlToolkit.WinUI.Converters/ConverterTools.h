#pragma once

#ifdef __INTELLISENSE__
#include <optional>
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

        static bool AreValuesEqual(winrt::IInspectable const& left, winrt::IInspectable const& right)
        {
            return EqualsHelper::ConvertibleEquals(left, right);
        }
	};
}
