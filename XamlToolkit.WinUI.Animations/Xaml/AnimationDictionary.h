#pragma once

#include "AnimationDictionary.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <vector>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A collection of animations that can be defined from XAML.
    /// </summary>
	struct AnimationDictionary : AnimationDictionaryT<AnimationDictionary, winrt::IVectorView<winrt::XamlToolkit::WinUI::Animations::AnimationSet>>,
        winrt::vector_view_base<AnimationDictionary, winrt::XamlToolkit::WinUI::Animations::AnimationSet>
    {
        AnimationDictionary() = default;

        auto& get_container() const noexcept
        {
            return list;
        }

        auto& get_container() noexcept
        {
            return list;
        }

        /// <summary>
        /// Sets the parent UIElement for the current animation dictionary.
        /// </summary>
        void Parent(winrt::UIElement const& value);

        void SetAt(uint32_t index, winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value);

        winrt::IVectorView<Animations::AnimationSet> GetView() const noexcept;

        void Append(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value);

        void InsertAt(uint32_t index, winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value);

        void RemoveAt(uint32_t index);

        void RemoveAtEnd();

        void Clear();

        void ReplaceAll(winrt::array_view<winrt::XamlToolkit::WinUI::Animations::AnimationSet const> items);

    private:
        std::vector<winrt::XamlToolkit::WinUI::Animations::AnimationSet> list;
        winrt::weak_ref<winrt::UIElement> parent;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct AnimationDictionary : AnimationDictionaryT<AnimationDictionary, implementation::AnimationDictionary> {};
}
