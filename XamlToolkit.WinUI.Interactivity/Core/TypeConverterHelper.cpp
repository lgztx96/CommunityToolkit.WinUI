#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <string>
#endif
#include "TypeConverterHelper.h"

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    winrt::IInspectable TypeConverterHelper::Convert(winrt::hstring const& value, winrt::hstring const& destinationTypeFullName)
    {
        static constexpr std::wstring_view ContentControlFormatString = L"<ContentControl xmlns='http://schemas.microsoft.com/winfx/2006/xaml/presentation' xmlns:c='using:{0}'><c:{1}>{2}</c:{1}></ContentControl>";

        if (destinationTypeFullName.empty())
        {
            throw winrt::hresult_invalid_argument(L"destinationTypeFullName");
        }

        auto normalizedTypeName = NormalizeTypeName(destinationTypeFullName);
        if (normalizedTypeName == L"System.String") return winrt::box_value(value);
        if (normalizedTypeName == L"System.Boolean") return winrt::box_value(value == L"True" || value == L"true" || value == L"1");
        if (normalizedTypeName == L"System.Int32") return winrt::box_value(std::stoi(std::wstring{ value.c_str() }));
        if (normalizedTypeName == L"System.UInt32") return winrt::box_value(static_cast<uint32_t>(std::stoul(std::wstring{ value.c_str() })));
        if (normalizedTypeName == L"System.Int64") return winrt::box_value(static_cast<int64_t>(std::stoll(std::wstring{ value.c_str() })));
        if (normalizedTypeName == L"System.UInt64") return winrt::box_value(static_cast<uint64_t>(std::stoull(std::wstring{ value.c_str() })));
        if (normalizedTypeName == L"System.Single") return winrt::box_value(std::stof(std::wstring{ value.c_str() }));
        if (normalizedTypeName == L"System.Double") return winrt::box_value(std::stod(std::wstring{ value.c_str() }));

        auto scope = GetScope(normalizedTypeName);
        auto type = GetType(normalizedTypeName);
        auto xaml = winrt::format(ContentControlFormatString, scope, type, value);

        auto contentControl = winrt::Markup::XamlReader::Load(xaml).try_as<winrt::ContentControl>();
        if (contentControl)
        {
            return contentControl.Content();
        }

        return nullptr;
    }

    winrt::hstring TypeConverterHelper::GetScope(winrt::hstring const& name)
    {
        auto typeName = std::wstring_view{ name };
        auto indexOfLastPeriod = typeName.find_last_of(L'.');
        if (indexOfLastPeriod != std::wstring::npos && indexOfLastPeriod + 1 < typeName.size())
        {
            return winrt::hstring{ typeName.substr(0, indexOfLastPeriod) };
        }

        return name;
    }

    winrt::hstring TypeConverterHelper::GetType(winrt::hstring const& name)
    {
        auto typeName = std::wstring_view{ name };
        auto indexOfLastPeriod = typeName.find_last_of(L'.');
        if (indexOfLastPeriod != std::wstring::npos && indexOfLastPeriod + 1 < typeName.size())
        {
            return winrt::hstring{ typeName.substr(indexOfLastPeriod + 1) };
        }

        return name;
    }

    winrt::hstring TypeConverterHelper::NormalizeTypeName(winrt::hstring const& name)
    {
        using namespace std::literals;

        std::wstring_view value = name.c_str();
        if (value == L"String"sv) return L"System.String";
        if (value == L"Boolean"sv) return L"System.Boolean";
        if (value == L"Int32"sv) return L"System.Int32";
        if (value == L"UInt32"sv) return L"System.UInt32";
        if (value == L"Int64"sv) return L"System.Int64";
        if (value == L"UInt64"sv) return L"System.UInt64";
        if (value == L"Single"sv) return L"System.Single";
        if (value == L"Double"sv) return L"System.Double";
        return name;
    }
}
