#include "pch.h"
#include "winrt_module_imports.h"
#include "RegexStateTrigger.h"
#if __has_include("RegexStateTrigger.g.cpp")
#include "RegexStateTrigger.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <regex>
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    RegexStateTrigger::RegexStateTrigger()
    {
        UpdateTrigger();
    }

    const wil::single_threaded_property<winrt::DependencyProperty> RegexStateTrigger::ValueProperty =
        winrt::DependencyProperty::Register(
            L"Value",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &RegexStateTrigger::OnValuePropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> RegexStateTrigger::ExpressionProperty =
        winrt::DependencyProperty::Register(
            L"Expression",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &RegexStateTrigger::OnValuePropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> RegexStateTrigger::OptionsProperty =
        winrt::DependencyProperty::Register(
            L"Options",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::RegexOptions>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::RegexOptions::None), &RegexStateTrigger::OnValuePropertyChanged));

    void RegexStateTrigger::OnValuePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        winrt::get_self<RegexStateTrigger>(d.as<class_type>())->UpdateTrigger();
    }

    void RegexStateTrigger::UpdateTrigger()
    {
        auto value = Value();
        auto expression = Expression();

        bool isActive = !value.empty() && !expression.empty();

        if (isActive)
        {
            try
            {
                auto options = ToStdRegexOptions(Options());
                std::wregex pattern(expression.begin(), expression.end(), options);
                isActive = std::regex_search(value.begin(), value.end(), pattern);
            }
            catch (...)
            {
                isActive = false;
            }
        }

        SetActive(isActive);
    }

    std::regex_constants::syntax_option_type RegexStateTrigger::ToStdRegexOptions(winrt::XamlToolkit::WinUI::RegexOptions options)
    {
        using namespace std::regex_constants;

        auto flags = std::regex_constants::ECMAScript;

        if (static_cast<int>(options) & static_cast<int>(winrt::XamlToolkit::WinUI::RegexOptions::IgnoreCase))
            flags |= icase;
        if (static_cast<int>(options) & static_cast<int>(winrt::XamlToolkit::WinUI::RegexOptions::Multiline))
            flags |= multiline;
        if (static_cast<int>(options) & static_cast<int>(winrt::XamlToolkit::WinUI::RegexOptions::Optimize))
            flags |= optimize;

        return flags;
    }
}
