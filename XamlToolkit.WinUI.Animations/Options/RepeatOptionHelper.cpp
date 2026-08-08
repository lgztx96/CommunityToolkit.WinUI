#include "pch.h"
#include "winrt_module_imports.h"
#include "RepeatOptionHelper.h"
#if __has_include("RepeatOptionHelper.g.cpp")
#include "RepeatOptionHelper.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    RepeatOption RepeatOptionHelper::Count(int count)
    {
        if (count < 0)
        {
            throw winrt::hresult_invalid_argument(L"The parameter \"count\" must be greater than or equal to 0.");
        }
        return RepeatOption{ count };
    }

    winrt::RepeatBehavior RepeatOptionHelper::ToRepeatBehavior(RepeatOption const& option)
    {
        if (option.Value < 0)
        {
            return winrt::RepeatBehaviorHelper::Forever();
        }
        return winrt::RepeatBehaviorHelper::FromCount(option.Value);
    }

    void RepeatOptionHelper::ToBehaviorAndCount(RepeatOption const& option, winrt::AnimationIterationBehavior& behavior, int& count)
    {
        if (option.Value < 0)
        {
            behavior = winrt::AnimationIterationBehavior::Forever;
            count = 1;
        }
        else {
            behavior = winrt::AnimationIterationBehavior::Count;
            count = option.Value;
        }
    }

    RepeatOption RepeatOptionHelper::CreateFromString(winrt::hstring const& text)
    {
        std::wstring_view sv{ text };

        const auto start = sv.find_first_not_of(L" \t\r\n");
        if (start == std::wstring_view::npos)
        {
            goto fail;
        }

        {
            sv.remove_prefix(start);
            const auto end = sv.find_last_not_of(L" \t\r\n");
            sv.remove_suffix(sv.size() - end - 1);

            std::wstring temp{ sv };

            try
            {
                size_t pos{};
                const int count = std::stoi(temp, &pos);

                if (pos == temp.size())
                {
                    return Count(count);
                }
            }
            catch (...)
            {
            }

            if (_wcsicmp(temp.data(), L"Forever") == 0)
            {
                return Forever();
            }
        }
    fail:
        throw std::invalid_argument("The input text is not valid to parse a new RepeatOption instance. It must be either a natural number or \"Forever\".");
    }
}