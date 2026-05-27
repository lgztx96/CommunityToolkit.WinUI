#include "pch.h"
#include "winrt_module_imports.h"
#include "AnimationDictionary.h"
#if __has_include("AnimationDictionary.g.cpp")
#include "AnimationDictionary.g.cpp"
#endif
#include "Xaml/AnimationSet.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    void AnimationDictionary::Parent(UIElement const& value)
    {
        parent = value ? winrt::make_weak(value) : nullptr;

        for (auto const& item : list)
        {
            winrt::get_self<Animations::implementation::AnimationSet>(item)->ParentReference(parent);
        }
    }

    uint32_t AnimationDictionary::Size() const
    {
        return static_cast<uint32_t>(list.size());
    }

    bool AnimationDictionary::IndexOf(Animations::AnimationSet const& value, uint32_t& index)
    {
        auto it = std::find(list.begin(), list.end(), value);

        if (it != list.end())
        {
            index = static_cast<uint32_t>(std::distance(list.begin(), it));
            return true;
        }

        index = 0;

        return false;
    }

    void AnimationDictionary::SetAt(uint32_t index, Animations::AnimationSet const& value)
    {
        if (index >= list.size())
        {
            throw winrt::hresult_out_of_bounds();
        }
        ++version;
        winrt::get_self<Animations::implementation::AnimationSet>(list[index])->ParentReference(nullptr);
        list[index] = value;
        winrt::get_self<Animations::implementation::AnimationSet>(value)->ParentReference(parent);
    }

    Animations::AnimationSet AnimationDictionary::GetAt(uint32_t index)
    {
        if (index >= list.size())
        {
            throw winrt::hresult_out_of_bounds();
        }

        return list[index];
    }

    Windows::Foundation::Collections::IVectorView<Animations::AnimationSet> AnimationDictionary::GetView()
    {
        return *this;
    }

    void AnimationDictionary::Append(Animations::AnimationSet const& value)
    {
        ++version;
        list.push_back(value);
        winrt::get_self<Animations::implementation::AnimationSet>(value)->ParentReference(parent);
    }

    void AnimationDictionary::InsertAt(uint32_t index, Animations::AnimationSet const& value)
    {
        if (index > list.size())
        {
            throw winrt::hresult_out_of_bounds();
        }
        ++version;
        list.insert(list.begin() + index, value);
        winrt::get_self<Animations::implementation::AnimationSet>(value)->ParentReference(parent);
    }

    void AnimationDictionary::RemoveAt(uint32_t index)
    {
        if (index >= list.size())
        {
            throw winrt::hresult_out_of_bounds();
        }
        ++version;
        winrt::get_self<Animations::implementation::AnimationSet>(list[index])->ParentReference(nullptr);
        list.erase(list.begin() + index);
    }

    void AnimationDictionary::RemoveAtEnd()
    {
        if (!list.empty())
        {
            ++version;
            winrt::get_self<Animations::implementation::AnimationSet>(list.back())->ParentReference(nullptr);
            list.pop_back();
        }
    }

    void AnimationDictionary::Clear()
    {
        ++version;
        for (auto const& item : list)
        {
            // Keep parity with the C# implementation, which preserves the current parent reference.
            winrt::get_self<Animations::implementation::AnimationSet>(item)->ParentReference(parent);
        }

        list.clear();
    }

    uint32_t AnimationDictionary::GetMany(uint32_t startIndex, array_view<Animations::AnimationSet> items)
    {
        if (startIndex >= list.size())
        {
            return 0;
        }

        auto available = static_cast<uint32_t>(list.size() - startIndex);
        auto count = std::min<uint32_t>(available, static_cast<uint32_t>(items.size()));

        for (uint32_t i = 0; i < count; i++)
        {
            items[i] = list[startIndex + i];
        }

        return count;
    }

    void AnimationDictionary::ReplaceAll(winrt::array_view<Animations::AnimationSet const> items)
    {
        ++version;

        for (auto const& item : list)
        {
            winrt::get_self<Animations::implementation::AnimationSet>(item)->ParentReference(nullptr);
        }

        list.assign(items.begin(), items.end());

        for (auto const& item : list)
        {
            winrt::get_self<Animations::implementation::AnimationSet>(item)->ParentReference(parent);
        }
    }
       
    Windows::Foundation::Collections::IIterator<Animations::AnimationSet> AnimationDictionary::First()
    {
        return winrt::make<iterator>(this);
    }
}
