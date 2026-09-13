#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <iterator>
#include <vector>
#endif
#include "InterspersedObservableVector.h"
#if __has_include("InterspersedObservableVector.g.cpp")
#include "InterspersedObservableVector.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    namespace
    {
        struct VectorChangedEventArgs : winrt::implements<VectorChangedEventArgs, winrt::IVectorChangedEventArgs>
        {
            VectorChangedEventArgs(
                winrt::CollectionChange const change,
                const uint32_t index) :
                _change(change),
                _index(index)
            {}

            winrt::CollectionChange CollectionChange() const
            {
                return _change;
            }

            uint32_t Index() const
            {
                return _index;
            }

        private:
            winrt::CollectionChange const _change;
            const uint32_t _index;
        };
    }

    InterspersedObservableVector::InterspersedObservableVector(
        winrt::IInspectable const& source)
    {
        if (!source)
        {
            throw winrt::hresult_invalid_argument(L"The input items source cannot be null.");
        }

        if (auto items = source.try_as<winrt::IVector<winrt::IInspectable>>())
        {
            _itemsSource = items;
        }
        else if (auto bindable = source.try_as<muxi::IBindableVector>())
        {
            _itemsSource = reinterpret_cast<winrt::IVector<winrt::IInspectable> const&>(bindable);
        }
        else
        {
            throw winrt::hresult_invalid_argument(L"The input items source must implement IVector or IBindableVector.");
        }

        if (auto notify = source.try_as<muxi::INotifyCollectionChanged>())
        {
            _notifyCollectionChanged = notify;

            _sourceEventToken = notify.CollectionChanged({ get_weak(), &InterspersedObservableVector::OnCollectionChanged });
        } 
        else if (auto bindableObservable = source.try_as<muxi::IBindableObservableVector>())
        {
            _bindableObservableVector = bindableObservable;

            _sourceEventToken = bindableObservable.VectorChanged(
                {
                    get_weak(),
                    [weak = get_weak()](
                        muxi::IBindableObservableVector const& sender,
                        winrt::IInspectable const& args)
                    {
                        if (auto self = weak.get())
                        {
                            self->OnBindableVectorChanged(sender, args);
                        }
                    }
                });
        }
        else if (auto observable = source.try_as<winrt::IObservableVector<winrt::IInspectable>>())
        {
            _observableVector = observable;

            _sourceEventToken = observable.VectorChanged({ get_weak(), &InterspersedObservableVector::OnVectorChanged });
        }
    }

    InterspersedObservableVector::~InterspersedObservableVector()
    {
        if (_notifyCollectionChanged)
        {
            _notifyCollectionChanged.CollectionChanged(_sourceEventToken);
        }
        else if (_bindableObservableVector)
        {
            _bindableObservableVector.VectorChanged(_sourceEventToken);
        }
        else if (_observableVector)
        {
            _observableVector.VectorChanged(_sourceEventToken);
        }
    }

    winrt::event_token InterspersedObservableVector::VectorChanged(
        VectorChangedHandler const& handler)
    {
        return _vectorChanged.add(handler);
    }

    void InterspersedObservableVector::VectorChanged(
        winrt::event_token const& token) noexcept
    {
        _vectorChanged.remove(token);
    }

    uint32_t InterspersedObservableVector::Size() const noexcept
    {
        return static_cast<uint32_t>(
            _itemsSource.Size() + _interspersedObjects.size());
    }

    winrt::IInspectable InterspersedObservableVector::GetAt(
        const uint32_t index) const
    {
        if (const auto it = _interspersedObjects.find(index); it != _interspersedObjects.end())
        {
            return it->second;
        }

        return _itemsSource.GetAt(ToInnerIndex(index));
    }

    winrt::IVectorView<winrt::IInspectable> InterspersedObservableVector::GetView() const
    {
        auto snapshot = winrt::single_threaded_vector<winrt::IInspectable>();

        for (uint32_t index = 0; index < Size(); ++index)
        {
            snapshot.Append(GetAt(index));
        }

        return snapshot.GetView();
    }

    bool InterspersedObservableVector::IndexOf(winrt::IInspectable const& value, uint32_t& index) const
    {
        for (const auto& [key, item] : _interspersedObjects)
        {
            if (item == value)
            {
                index = key;
                return true;
            }
        }

        uint32_t innerIndex{};

        if (_itemsSource.IndexOf(value, innerIndex))
        {
            index = ToOuterIndex(innerIndex);
            return true;
        }

        return false;
    }

    void InterspersedObservableVector::SetAt(uint32_t, winrt::IInspectable const&)
    {
        throw winrt::hresult_not_implemented();
    }

    void InterspersedObservableVector::InsertAt(const uint32_t outerIndex, winrt::IInspectable const& value)
    {
        uint32_t innerIndex = outerIndex;

        for (const auto& [key, item] : _interspersedObjects)
        {
            if (key < outerIndex)
            {
                --innerIndex;
            }
        }

        if (innerIndex != outerIndex)
        {
            MoveKeysForward(outerIndex, 1);
            _isInsertingOriginal = true;
        }

        _itemsSource.InsertAt(innerIndex, value);
    }

    void InterspersedObservableVector::RemoveAt(uint32_t)
    {
        throw winrt::hresult_not_implemented();
    }

    void InterspersedObservableVector::Append(winrt::IInspectable const& value)
    {
        const uint32_t index = _itemsSource.Size();

        _itemsSource.Append(value);

        // C# Add() returns this value, while the actual notification
        // comes from the underlying observable collection.
        //
        // Keep the same observable behavior here.
        static_cast<void>(ToOuterIndex(index));
    }

    void InterspersedObservableVector::RemoveAtEnd()
    {
        throw winrt::hresult_not_implemented();
    }

    void InterspersedObservableVector::Clear()
    {
        _itemsSource.Clear();
        _interspersedObjects.clear();
    }

    uint32_t InterspersedObservableVector::GetMany(uint32_t, winrt::array_view<winrt::IInspectable>) const
    {
        throw winrt::hresult_not_implemented();
    }

    void InterspersedObservableVector::ReplaceAll(winrt::array_view<const winrt::IInspectable>)
    {
        throw winrt::hresult_not_implemented();
    }

    winrt::IIterator<winrt::IInspectable> InterspersedObservableVector::First() const
    {
        auto snapshot = winrt::single_threaded_vector<winrt::IInspectable>();

        uint32_t i = 0;
        uint32_t realized = 0;

        for (uint32_t innerIndex = 0;
            innerIndex < _itemsSource.Size();
            ++innerIndex)
        {
            while (true)
            {
                auto it = _interspersedObjects.find(i);

                if (it == _interspersedObjects.end())
                {
                    break;
                }

                snapshot.Append(it->second);
                ++i;
                ++realized;
            }

            snapshot.Append(
                _itemsSource.GetAt(innerIndex));

            ++i;
        }

        if (realized < _interspersedObjects.size())
        {
            for (const auto& [key, value] : _interspersedObjects)
            {
                if (key >= i)
                {
                    snapshot.Append(value);
                }
            }
        }

        return snapshot.First();
    }

    void InterspersedObservableVector::Insert(const uint32_t index, winrt::IInspectable const& value)
    {
        MoveKeysForward(index, 1);

        _interspersedObjects[index] = value;

        RaiseVectorChanged(winrt::CollectionChange::ItemInserted, index);
    }

    bool InterspersedObservableVector::Contains(winrt::IInspectable const& value) const
    {
        for (const auto& [key, item] : _interspersedObjects)
        {
            if (item == value)
            {
                return true;
            }
        }

        uint32_t index{};

        return _itemsSource.IndexOf(value, index);
    }

    winrt::IVector<winrt::IInspectable> InterspersedObservableVector::ItemsSource() const noexcept
    {
        return _itemsSource;
    }

    bool InterspersedObservableVector::RemoveItem(winrt::IInspectable const& value)
    {
        for (auto it = _interspersedObjects.begin(); it != _interspersedObjects.end(); ++it)
        {
            if (it->second == value)
            {
                const uint32_t index = it->first;

                _interspersedObjects.erase(it);

                MoveKeysBackward(index, 1);

                RaiseVectorChanged(winrt::CollectionChange::ItemRemoved, index);

                return true;
            }
        }

        uint32_t innerIndex{};

        if (_itemsSource.IndexOf(value, innerIndex))
        {
            _itemsSource.RemoveAt(innerIndex);
            return true;
        }

        return false;
    }

    void InterspersedObservableVector::MoveKeysForward(const uint32_t pivot, const uint32_t amount)
    {
        std::map<uint32_t, winrt::IInspectable> updated;

        for (const auto& [key, value] : _interspersedObjects)
        {
            if (key < pivot)
            {
                updated.emplace(key, value);
            }
            else
            {
                updated.emplace(key + amount, value);
            }
        }

        _interspersedObjects.swap(updated);
    }

    void InterspersedObservableVector::MoveKeysBackward(const uint32_t pivot, const uint32_t amount)
    {
        std::map<uint32_t, winrt::IInspectable> updated;

        for (const auto& [key, value] : _interspersedObjects)
        {
            if (key <= pivot)
            {
                updated.emplace(key, value);
            }
            else
            {
                updated.emplace(key - amount, value);
            }
        }

        _interspersedObjects.swap(updated);
    }

    void InterspersedObservableVector::ReadjustKeys()
    {
        const uint32_t count = _itemsSource.Size();
        uint32_t existing = 0;

        std::map<uint32_t, winrt::IInspectable> updated;

        for (const auto& [key, value] : _interspersedObjects)
        {
            if (key <= count)
            {
                updated.emplace(key, value);
                ++existing;
            }
            else
            {
                updated.emplace(count + existing, value);
                ++existing;
            }
        }

        _interspersedObjects.swap(updated);
    }

    uint32_t InterspersedObservableVector::ToInnerIndex(const uint32_t outerIndex) const
    {
        if (outerIndex >= Size())
        {
            throw winrt::hresult_out_of_bounds();
        }

        if (_interspersedObjects.contains(outerIndex))
        {
            throw winrt::hresult_invalid_argument(
                L"The outer index can't be inserted as a key to the original collection.");
        }

        uint32_t offset = 0;

        for (const auto& [key, value] : _interspersedObjects)
        {
            if (key <= outerIndex)
            {
                ++offset;
            }
            else
            {
                break;
            }
        }

        return outerIndex - offset;
    }

    uint32_t InterspersedObservableVector::ToOuterIndex(const uint32_t innerIndex) const
    {
        if (innerIndex >= _itemsSource.Size())
        {
            throw winrt::hresult_out_of_bounds();
        }

        uint32_t outerIndex = innerIndex;

        for (const auto& [key, value] : _interspersedObjects)
        {
            if (outerIndex >= key)
            {
                ++outerIndex;
            }
            else
            {
                break;
            }
        }

        return outerIndex;
    }

    uint32_t InterspersedObservableVector::ToOuterIndexAfterRemoval(const uint32_t innerIndexToProject) const
    {
        uint32_t projected = innerIndexToProject;

        for (const auto& [key, value] : _interspersedObjects)
        {
            if (projected >= key)
            {
                ++projected;
            }
            else
            {
                break;
            }
        }

        return projected;
    }

    void InterspersedObservableVector::RaiseVectorChanged(winrt::CollectionChange const change, const uint32_t index)
    {
        _vectorChanged(*this, winrt::make<VectorChangedEventArgs>(change, index));
    }

    void InterspersedObservableVector::OnCollectionChanged(
        winrt::IInspectable const&,
        muxi::NotifyCollectionChangedEventArgs const& args)
    {
        switch (args.Action())
        {
        case muxi::NotifyCollectionChangedAction::Add:
        {
            const auto newItems = args.NewItems();

            const uint32_t count = static_cast<uint32_t>(newItems.Size());

            if (count == 0)
            {
                break;
            }

            const uint32_t index = static_cast<uint32_t>(args.NewStartingIndex());

            if (!_isInsertingOriginal)
            {
                MoveKeysForward(index, count);
            }

            _isInsertingOriginal = false;

            for (uint32_t i = 0; i < count; ++i)
            {
                RaiseVectorChanged(winrt::CollectionChange::ItemInserted, ToOuterIndex(index + i));
            }

            break;
        }

        case muxi::NotifyCollectionChangedAction::Remove:
        {
            const auto oldItems = args.OldItems();

            const uint32_t count = static_cast<uint32_t>(oldItems.Size());

            if (count == 0)
            {
                break;
            }

            const uint32_t index = static_cast<uint32_t>(args.OldStartingIndex());

            const uint32_t outerIndex = ToOuterIndexAfterRemoval(index);

            MoveKeysBackward(outerIndex, count);

            for (uint32_t i = 0; i < count; ++i)
            {
                RaiseVectorChanged(winrt::CollectionChange::ItemRemoved, outerIndex);
            }

            break;
        }

        case muxi::NotifyCollectionChangedAction::Reset:
            ReadjustKeys();

            RaiseVectorChanged(winrt::CollectionChange::Reset, 0);

            break;

        default:
            break;
        }
    }

    void InterspersedObservableVector::OnBindableVectorChanged(
        muxi::IBindableObservableVector const& sender,
        winrt::IInspectable const& args)
    {
        OnVectorChanged(
            reinterpret_cast<
            winrt::IObservableVector<winrt::IInspectable> const&>(
                sender),
            reinterpret_cast<
            winrt::IVectorChangedEventArgs const&>(
                args));
    }

    void InterspersedObservableVector::OnVectorChanged(
        winrt::IObservableVector<winrt::IInspectable> const&,
        winrt::IVectorChangedEventArgs const& args)
    {
        switch (args.CollectionChange())
        {
        case winrt::CollectionChange::ItemInserted:
            if (!_isInsertingOriginal)
            {
                MoveKeysForward(args.Index(), 1);
            }

            _isInsertingOriginal = false;
            RaiseVectorChanged(winrt::CollectionChange::ItemInserted, ToOuterIndex(args.Index()));
            break;

        case winrt::CollectionChange::ItemRemoved:
        {
            const uint32_t outerIndex = ToOuterIndexAfterRemoval(args.Index());
            MoveKeysBackward(outerIndex, 1);
            RaiseVectorChanged(winrt::CollectionChange::ItemRemoved, outerIndex);
            break;
        }

        case winrt::CollectionChange::ItemChanged:
            RaiseVectorChanged(winrt::CollectionChange::ItemChanged, ToOuterIndex(args.Index()));
            break;

        case winrt::CollectionChange::Reset:
            ReadjustKeys();
            RaiseVectorChanged(winrt::CollectionChange::Reset, 0);
            break;

        default:
            break;
        }
    }
}
