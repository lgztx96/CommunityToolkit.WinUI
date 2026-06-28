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

    RepeatBehavior RepeatOptionHelper::ToRepeatBehavior(RepeatOption const& option)
    {
        if (option.Value < 0)
        {
            return RepeatBehaviorHelper::Forever();
        }
        return RepeatBehaviorHelper::FromCount(option.Value);
    }

    std::tuple<AnimationIterationBehavior, int> RepeatOptionHelper::ToBehaviorAndCount(RepeatOption const& option)
    {
        if (option.Value < 0)
        {
            return std::make_tuple(AnimationIterationBehavior::Forever, 1);
        }
        return std::make_tuple(AnimationIterationBehavior::Count, option.Value);
    }

    RepeatOption RepeatOptionHelper::Parse(winrt::hstring const& text)
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