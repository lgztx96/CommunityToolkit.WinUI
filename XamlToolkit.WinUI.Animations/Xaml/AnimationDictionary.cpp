#include "pch.h"
#include "winrt_module_imports.h"
#include "AnimationDictionary.h"
#if __has_include("AnimationDictionary.g.cpp")
#include "AnimationDictionary.g.cpp"
#endif
#include "Xaml/AnimationSet.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    void AnimationDictionary::Parent(winrt::UIElement const& value)
    {
        parent = value;

        for (const auto& item : list)
        {
            winrt::get_self<implementation::AnimationSet>(item)->ParentReference(parent);
        }
    }

    void AnimationDictionary::SetAt(uint32_t index, winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value)
    {
        if (index >= list.size())
        {
            throw winrt::hresult_out_of_bounds();
        }

        winrt::get_self<implementation::AnimationSet>(list[index])->ParentReference(nullptr);
        list[index] = value;
        winrt::get_self<implementation::AnimationSet>(value)->ParentReference(parent);
    }

    winrt::IVectorView<winrt::XamlToolkit::WinUI::Animations::AnimationSet> AnimationDictionary::GetView() const noexcept
    {
        return *this;
    }

    void AnimationDictionary::Append(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value)
    {
        list.push_back(value);
        winrt::get_self<implementation::AnimationSet>(value)->ParentReference(parent);
    }

    void AnimationDictionary::InsertAt(uint32_t index, winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value)
    {
        if (index > list.size())
        {
            throw winrt::hresult_out_of_bounds();
        }

        list.insert(list.begin() + index, value);
        winrt::get_self<implementation::AnimationSet>(value)->ParentReference(parent);
    }

    void AnimationDictionary::RemoveAt(uint32_t index)
    {
        if (index >= list.size())
        {
            throw winrt::hresult_out_of_bounds();
        }

        winrt::get_self<implementation::AnimationSet>(list[index])->ParentReference(nullptr);
        list.erase(list.begin() + index);
    }

    void AnimationDictionary::RemoveAtEnd()
    {
        if (!list.empty())
        {
            winrt::get_self<implementation::AnimationSet>(list.back())->ParentReference(nullptr);
            list.pop_back();
        }
    }

    void AnimationDictionary::Clear()
    {
        for (const auto& item : list)
        {
            winrt::get_self<implementation::AnimationSet>(item)->ParentReference(nullptr);
        }

        list.clear();
    }

    void AnimationDictionary::ReplaceAll(winrt::array_view<winrt::XamlToolkit::WinUI::Animations::AnimationSet const> items)
    {
        for (const auto& item : list)
        {
            winrt::get_self<implementation::AnimationSet>(item)->ParentReference(nullptr);
        }

        list.assign(items.begin(), items.end());

        for (const auto& item : list)
        {
            winrt::get_self<implementation::AnimationSet>(item)->ParentReference(parent);
        }
    }
}
