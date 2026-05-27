#pragma once

#include "AnimationDictionary.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <vector>
#include <memory>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A collection of animations that can be defined from XAML.
    /// </summary>
	struct AnimationDictionary : AnimationDictionaryT<AnimationDictionary, winrt::Windows::Foundation::Collections::IVectorView<Animations::AnimationSet>>
    {
    public:
        AnimationDictionary() = default;

        /// <summary>
        /// Sets the parent UIElement for the current animation dictionary.
        /// </summary>
        void Parent(Microsoft::UI::Xaml::UIElement const& value);

        /// <inheritdoc/>
        uint32_t Size() const;

        /// <inheritdoc/>
        bool IndexOf(Animations::AnimationSet const& value, uint32_t& index);

        /// <inheritdoc/>
        void SetAt(uint32_t index, Animations::AnimationSet const& value);

        /// <inheritdoc/>
        Animations::AnimationSet GetAt(uint32_t index);

        /// <inheritdoc/>
        Windows::Foundation::Collections::IVectorView<Animations::AnimationSet> GetView();

        /// <inheritdoc/>
        void Append(Animations::AnimationSet const& value);

        /// <inheritdoc/>
        void InsertAt(uint32_t index, Animations::AnimationSet const& value);

        /// <inheritdoc/>
        void RemoveAt(uint32_t index);

        /// <inheritdoc/>
        void RemoveAtEnd();

        /// <inheritdoc/>
        void Clear();

        /// <inheritdoc/>
        uint32_t GetMany(uint32_t startIndex, array_view<Animations::AnimationSet> items);

        /// <inheritdoc/>
        void ReplaceAll(array_view<Animations::AnimationSet const> items);

        /// <summary>
        /// Gets the first animation set.
        /// </summary>
        Windows::Foundation::Collections::IIterator<Animations::AnimationSet> First();

        struct iterator : winrt::implements<iterator, Windows::Foundation::Collections::IIterator<Animations::AnimationSet>>
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

            Animations::AnimationSet Current() const
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

            uint32_t GetMany(array_view<Animations::AnimationSet> values)
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
            typename std::vector<Animations::AnimationSet>::const_iterator current;
            typename std::vector<Animations::AnimationSet>::const_iterator const end;
        };

    private:
        uint32_t version {0};
        std::vector<Animations::AnimationSet> list;
        winrt::weak_ref<Microsoft::UI::Xaml::UIElement> parent;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct AnimationDictionary : AnimationDictionaryT<AnimationDictionary, implementation::AnimationDictionary> {};
}
