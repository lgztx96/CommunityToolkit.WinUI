#pragma once

#include "AnimationDictionary.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <vector>
#include <memory>
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
	struct AnimationDictionary : AnimationDictionaryT<AnimationDictionary, winrt::IVectorView<winrt::XamlToolkit::WinUI::Animations::AnimationSet>>
    {
    public:
        AnimationDictionary() = default;

        /// <summary>
        /// Sets the parent UIElement for the current animation dictionary.
        /// </summary>
        void Parent(winrt::UIElement const& value);

        /// <inheritdoc/>
        uint32_t Size() const;

        /// <inheritdoc/>
        bool IndexOf(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value, uint32_t& index);

        /// <inheritdoc/>
        void SetAt(uint32_t index, winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value);

        /// <inheritdoc/>
        winrt::XamlToolkit::WinUI::Animations::AnimationSet GetAt(uint32_t index);

        /// <inheritdoc/>
        winrt::IVectorView<Animations::AnimationSet> GetView();

        /// <inheritdoc/>
        void Append(winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value);

        /// <inheritdoc/>
        void InsertAt(uint32_t index, winrt::XamlToolkit::WinUI::Animations::AnimationSet const& value);

        /// <inheritdoc/>
        void RemoveAt(uint32_t index);

        /// <inheritdoc/>
        void RemoveAtEnd();

        /// <inheritdoc/>
        void Clear();

        /// <inheritdoc/>
        uint32_t GetMany(uint32_t startIndex, winrt::array_view<winrt::XamlToolkit::WinUI::Animations::AnimationSet> items);

        /// <inheritdoc/>
        void ReplaceAll(winrt::array_view<winrt::XamlToolkit::WinUI::Animations::AnimationSet const> items);

        /// <summary>
        /// Gets the first animation set.
        /// </summary>
        winrt::IIterator<winrt::XamlToolkit::WinUI::Animations::AnimationSet> First();

        struct iterator : winrt::implements<iterator, winrt::IIterator<winrt::XamlToolkit::WinUI::Animations::AnimationSet>>
        {
            explicit iterator(AnimationDictionary* owner) noexcept :
                version(owner->version),
                current(owner->list.begin()),
                end(owner->list.end())
            {
                this->owner.copy_from(owner);
            }

            void abi_enter() const
            {
                if (version != owner->version)
                {
                    throw hresult_changed_state();
                }
            }

            winrt::XamlToolkit::WinUI::Animations::AnimationSet Current() const
            {
                if (current == end)
                {
                    throw hresult_out_of_bounds();
                }

                return *current;
            }

            bool HasCurrent() const noexcept
            {
                return current != end;
            }

            bool MoveNext() noexcept
            {
                if (current != end)
                {
                    ++current;
                }

                return HasCurrent();
            }

            uint32_t GetMany(winrt::array_view<winrt::XamlToolkit::WinUI::Animations::AnimationSet> values)
            {
                uint32_t actual = static_cast<uint32_t>(std::distance(current, end));

                if (actual > values.size())
                {
                    actual = values.size();
                }

                std::copy_n(current, actual, values.begin());
                std::advance(current, actual);
                return actual;
            }

        private:

            winrt::com_ptr<AnimationDictionary> owner;
            uint32_t const version;
            typename std::vector<winrt::XamlToolkit::WinUI::Animations::AnimationSet>::const_iterator current;
            typename std::vector<winrt::XamlToolkit::WinUI::Animations::AnimationSet>::const_iterator const end;
        };

    private:
        uint32_t version {0};
        std::vector<winrt::XamlToolkit::WinUI::Animations::AnimationSet> list;
        winrt::weak_ref<winrt::UIElement> parent;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct AnimationDictionary : AnimationDictionaryT<AnimationDictionary, implementation::AnimationDictionary> {};
}
