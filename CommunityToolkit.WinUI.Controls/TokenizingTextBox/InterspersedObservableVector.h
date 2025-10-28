#pragma once

#include "InterspersedObservableVector.g.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <map>
#include <algorithm>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct InterspersedObservableVector : InterspersedObservableVectorT<InterspersedObservableVector>
    {
    private:
        using VectorChangedHandler = VectorChangedEventHandler<IInspectable>;

        IVector<IInspectable> _itemsSource{ nullptr };
        std::map<uint32_t, IInspectable> _interspersedObjects;
        bool _isInsertingOriginal = false;
        winrt::event<VectorChangedHandler> _vectorChanged;
        IObservableVector<IInspectable>::VectorChanged_revoker _sourceRevoker;

    public:
        InterspersedObservableVector(IInspectable const& source);

        winrt::event_token VectorChanged(VectorChangedHandler const& handler);

        void VectorChanged(event_token const& token) noexcept;

        uint32_t Size() const noexcept;

        IInspectable GetAt(uint32_t index) const;

        IVectorView<IInspectable> GetView() const;

        bool IndexOf(IInspectable const& value, uint32_t& index) const;

        void SetAt(uint32_t, IInspectable const&);

        void InsertAt(uint32_t outerIndex, IInspectable const& value);

        void RemoveAt(uint32_t index);

        void Remove(IInspectable const& value);

        void RemoveAtEnd();

        void Append(IInspectable const& value);

        void ReplaceAll(winrt::array_view<const IInspectable> newItems);

        uint32_t GetMany(uint32_t startIndex, array_view<IInspectable> values) const;

        void Clear();

        IIterator<IInspectable> First() const;

        void Insert(uint32_t index, IInspectable const& value);

        bool Contains(IInspectable const& value) const;

        IVector<IInspectable> ItemsSource() const;

    private:

        void ReadjustKeys();

        void MoveKeysForward(uint32_t pivot, uint32_t amount);

        void MoveKeysBackward(uint32_t pivot, uint32_t amount);

        uint32_t ToInnerIndex(uint32_t outerIndex) const;

        uint32_t ToOuterIndex(uint32_t innerIndex) const;

        uint32_t ToOuterIndexAfterRemoval(uint32_t innerIndexToProject) const;

        void RaiseVectorChanged(CollectionChange change, uint32_t index);

        void OnItemsSourceChanged(IObservableVector<IInspectable> const&, IVectorChangedEventArgs const& args);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct InterspersedObservableVector : InterspersedObservableVectorT<InterspersedObservableVector, implementation::InterspersedObservableVector>
    {
    };
}
