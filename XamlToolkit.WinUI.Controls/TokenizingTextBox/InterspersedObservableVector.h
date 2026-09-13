#pragma once

#include "InterspersedObservableVector.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <map>
#endif

namespace winrt
{
    using namespace ::winrt::Windows::Foundation;
    using namespace ::winrt::Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    namespace muxi = ::winrt::Microsoft::UI::Xaml::Interop;

    struct InterspersedObservableVector : InterspersedObservableVectorT<InterspersedObservableVector>
    {
        using VectorChangedHandler = winrt::VectorChangedEventHandler<winrt::IInspectable>;

        explicit InterspersedObservableVector(winrt::IInspectable const& source);

        ~InterspersedObservableVector();

        // IVector
        winrt::IInspectable GetAt(uint32_t index) const;

        winrt::IVectorView<winrt::IInspectable> GetView() const;

        bool IndexOf(winrt::IInspectable const& value, uint32_t& index) const;

        void SetAt(uint32_t index, winrt::IInspectable const& value);

        void InsertAt(uint32_t outerIndex, winrt::IInspectable const& value);

        void RemoveAt(uint32_t index);

        void Append(winrt::IInspectable const& value);

        void RemoveAtEnd();

        void Clear();

        uint32_t Size() const noexcept;

        uint32_t GetMany(uint32_t startIndex, winrt::array_view<winrt::IInspectable> values) const;

        void ReplaceAll(winrt::array_view<const winrt::IInspectable> newItems);

        // IIterable
        winrt::IIterator<winrt::IInspectable> First() const;

        // IObservableVector
        winrt::event_token VectorChanged(VectorChangedHandler const& handler);

        void VectorChanged(winrt::event_token const& token) noexcept;

        // InterspersedObservableCollection API
        void Insert(uint32_t index, winrt::IInspectable const& value);

        bool Contains(winrt::IInspectable const& value) const;

        bool RemoveItem(winrt::IInspectable const& value);

        winrt::IVector<winrt::IInspectable> ItemsSource() const noexcept;

    private:
        void OnCollectionChanged(
            winrt::IInspectable const& sender,
            muxi::NotifyCollectionChangedEventArgs const& args);

        void OnBindableVectorChanged(
            muxi::IBindableObservableVector const& sender,
            winrt::IInspectable const& args);

        void OnVectorChanged(
            winrt::IObservableVector<winrt::IInspectable> const& sender,
            winrt::IVectorChangedEventArgs const& args);

        void RaiseVectorChanged(
            winrt::CollectionChange change,
            uint32_t index);

        void MoveKeysForward(uint32_t pivot, uint32_t amount);

        void MoveKeysBackward(uint32_t pivot, uint32_t amount);

        void ReadjustKeys();

        uint32_t ToInnerIndex(uint32_t outerIndex) const;

        uint32_t ToOuterIndex(uint32_t innerIndex) const;

        uint32_t ToOuterIndexAfterRemoval(uint32_t innerIndexToProject) const;

    private:
        winrt::IVector<winrt::IInspectable> _itemsSource{ nullptr };

        std::map<uint32_t, winrt::IInspectable> _interspersedObjects;

        bool _isInsertingOriginal{};

        muxi::INotifyCollectionChanged _notifyCollectionChanged{ nullptr };
        muxi::IBindableObservableVector _bindableObservableVector{ nullptr };
        winrt::IObservableVector<winrt::IInspectable> _observableVector{ nullptr };

        winrt::event_token _sourceEventToken;

        winrt::event<VectorChangedHandler> _vectorChanged;
    };
}

