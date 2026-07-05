#pragma once

#include "InterspersedObservableVector.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <map>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct InterspersedObservableVector : InterspersedObservableVectorT<InterspersedObservableVector>
    {
        using VectorChangedHandler = winrt::VectorChangedEventHandler<winrt::IInspectable>;

        InterspersedObservableVector(winrt::IInspectable const& source);

        winrt::event_token VectorChanged(VectorChangedHandler const& handler);

        void VectorChanged(event_token const& token) noexcept;

        uint32_t Size() const noexcept;

        winrt::IInspectable GetAt(uint32_t index) const;

        winrt::IVectorView<winrt::IInspectable> GetView() const;

        bool IndexOf(winrt::IInspectable const& value, uint32_t& index) const;

        void SetAt(uint32_t, winrt::IInspectable const&);

        void InsertAt(uint32_t outerIndex, winrt::IInspectable const& value);

        void RemoveAt(uint32_t index);

        bool Remove(winrt::IInspectable const& value);

        void RemoveAtEnd();

        void Append(winrt::IInspectable const& value);

        void ReplaceAll(winrt::array_view<const winrt::IInspectable> newItems);

        uint32_t GetMany(uint32_t startIndex, winrt::array_view<winrt::IInspectable> values) const;

        void Clear();

        winrt::IIterator<winrt::IInspectable> First() const;

        void Insert(uint32_t index, winrt::IInspectable const& value);

        bool Contains(winrt::IInspectable const& value) const;

        winrt::IVector<winrt::IInspectable> ItemsSource() const;

    private:
        void ReadjustKeys();

        void MoveKeysForward(uint32_t pivot, uint32_t amount);

        void MoveKeysBackward(uint32_t pivot, uint32_t amount);

        uint32_t ToInnerIndex(uint32_t outerIndex) const;

        uint32_t ToOuterIndex(uint32_t innerIndex) const;

        uint32_t ToOuterIndexAfterRemoval(uint32_t innerIndexToProject) const;

        void RaiseVectorChanged(winrt::CollectionChange change, uint32_t index);

        void OnItemsSourceChanged(winrt::IObservableVector<winrt::IInspectable> const&, winrt::IVectorChangedEventArgs const& args);

    private:
        winrt::IVector<winrt::IInspectable> _itemsSource{ nullptr };
        std::map<uint32_t, winrt::IInspectable> _interspersedObjects;
        bool _isInsertingOriginal = false;
        winrt::event<VectorChangedHandler> _vectorChanged;
        winrt::IObservableVector<winrt::IInspectable>::VectorChanged_revoker _sourceRevoker;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct InterspersedObservableVector : InterspersedObservableVectorT<InterspersedObservableVector, implementation::InterspersedObservableVector>
    {
    };
}
