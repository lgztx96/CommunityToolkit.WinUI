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
	struct args : winrt::implements<args, winrt::IVectorChangedEventArgs>
	{
		args(winrt::CollectionChange const change, uint32_t const index) :
			_change(change),
			_index(index)
		{
		}

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
		uint32_t const _index;
	};

	InterspersedObservableVector::InterspersedObservableVector(winrt::IInspectable const& source)
	{
		if (auto items = source.try_as<winrt::IVector<winrt::IInspectable>>())
		{
			_itemsSource = items;
		}
		else {
			throw winrt::hresult_invalid_argument(L"The input items source must implement IVector.");
		}

		if (auto vec = source.try_as<winrt::IObservableVector<winrt::IInspectable>>())
		{
			_sourceRevoker = vec.VectorChanged(winrt::auto_revoke, [weak = get_weak()](auto& sender, auto& args)
			{
				if (auto self = weak.get())
				{
					self->OnItemsSourceChanged(sender, args);
				}
			});
		}
	}

	winrt::event_token InterspersedObservableVector::VectorChanged(VectorChangedHandler const& handler)
	{
		return _vectorChanged.add(handler);
	}

	void InterspersedObservableVector::VectorChanged(winrt::event_token const& token) noexcept
	{
		_vectorChanged.remove(token);
	}

	uint32_t InterspersedObservableVector::Size() const noexcept
	{
		return static_cast<uint32_t>(_itemsSource.Size() + _interspersedObjects.size());
	}

	winrt::IInspectable InterspersedObservableVector::GetAt(uint32_t index) const
	{
		if (auto it = _interspersedObjects.find(index); it != _interspersedObjects.end())
		{
			return it->second;
		}

		uint32_t innerIndex = ToInnerIndex(index);
		return _itemsSource.GetAt(innerIndex);
	}

	winrt::IVectorView<winrt::IInspectable> InterspersedObservableVector::GetView() const
	{
		auto snapshot = winrt::single_threaded_vector<winrt::IInspectable>();
		for (uint32_t i = 0; i < Size(); ++i)
		{
			snapshot.Append(GetAt(i));
		}
		return snapshot.GetView();
	}

	bool InterspersedObservableVector::IndexOf(winrt::IInspectable const& value, uint32_t& index) const
	{
		for (const auto& [key, val] : _interspersedObjects)
		{
			if (val == value)
			{
				index = key;
				return true;
			}
		}

		uint32_t innerIndex;
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

	void InterspersedObservableVector::InsertAt(uint32_t outerIndex, winrt::IInspectable const& value)
	{
		uint32_t innerIndex = outerIndex;
		for (const auto& kvp : _interspersedObjects)
		{
			if (kvp.first < outerIndex)
			{
				innerIndex--;
			}
		}

		if (innerIndex != outerIndex)
		{
			MoveKeysForward(outerIndex, 1);
			_isInsertingOriginal = true;
		}

		_itemsSource.InsertAt(innerIndex, value);
	}

	void InterspersedObservableVector::RemoveAt(uint32_t index)
	{
		if (auto it = _interspersedObjects.find(index); it != _interspersedObjects.end())
		{
			_interspersedObjects.erase(it);
			MoveKeysBackward(index, 1);

			RaiseVectorChanged(winrt::CollectionChange::ItemRemoved, index);
		}
		else
		{
			uint32_t innerIndex = ToInnerIndex(index);
			_itemsSource.RemoveAt(innerIndex);
		}
	}

	void InterspersedObservableVector::RemoveAtEnd()
	{
		if (!_interspersedObjects.empty())
		{
			auto it = std::prev(_interspersedObjects.end());
			uint32_t index = it->first;
			_interspersedObjects.erase(it);
			RaiseVectorChanged(winrt::CollectionChange::ItemRemoved, index);
		}
		else
		{
			_itemsSource.RemoveAtEnd();
		}
	}

	void InterspersedObservableVector::Append(winrt::IInspectable const& value)
	{
		uint32_t index = _itemsSource.Size();
		_itemsSource.Append(value);
		uint32_t outer = ToOuterIndex(index);
		RaiseVectorChanged(winrt::CollectionChange::ItemInserted, outer);
	}

	void InterspersedObservableVector::ReplaceAll([[maybe_unused]] winrt::array_view<const winrt::IInspectable> newItems)
	{
		throw winrt::hresult_not_implemented();
	}

	uint32_t InterspersedObservableVector::GetMany([[maybe_unused]] uint32_t startIndex, [[maybe_unused]] winrt::array_view<winrt::IInspectable> values) const
	{
		throw winrt::hresult_not_implemented();
	}

	void InterspersedObservableVector::Clear()
	{
		_interspersedObjects.clear();
		_itemsSource.Clear();
		RaiseVectorChanged(winrt::CollectionChange::Reset, 0);
	}

	winrt::IIterator<winrt::IInspectable> InterspersedObservableVector::First() const
	{
		auto snapshot = winrt::single_threaded_vector<winrt::IInspectable>();
		for (uint32_t i = 0; i < Size(); ++i)
		{
			snapshot.Append(GetAt(i));
		}
		return snapshot.First();
	}

	void InterspersedObservableVector::Insert(uint32_t index, winrt::IInspectable const& value)
	{
		MoveKeysForward(index, 1);
		_interspersedObjects[index] = value;
		RaiseVectorChanged(winrt::CollectionChange::ItemInserted, index);
	}

	bool InterspersedObservableVector::Contains(winrt::IInspectable const& value) const
	{
		for (const auto& [key, val] : _interspersedObjects)
		{
			if (val == value)
			{
				return true;
			}
		}
		uint32_t index;
		return _itemsSource.IndexOf(value, index);
	}

	winrt::IVector<winrt::IInspectable> InterspersedObservableVector::ItemsSource() const noexcept
	{
		return _itemsSource;
	}

	void InterspersedObservableVector::MoveKeysForward(uint32_t pivot, uint32_t amount)
	{
		std::map<uint32_t, winrt::IInspectable> updated;
		for (auto it = _interspersedObjects.rbegin(); it != _interspersedObjects.rend(); ++it)
		{
			uint32_t key = it->first;
			winrt::IInspectable val = it->second;
			if (key < pivot)
			{
				updated[key] = val;
			}
			else
			{
				updated[key + amount] = val;
			}
		}
		_interspersedObjects.swap(updated);
	}

	void InterspersedObservableVector::MoveKeysBackward(uint32_t pivot, uint32_t amount)
	{
		std::map<uint32_t, winrt::IInspectable> updated;
		for (const auto& [key, val] : _interspersedObjects)
		{
			if (key <= pivot)
			{
				updated[key] = val;
			}
			else
			{
				updated[key - amount] = val;
			}
		}
		_interspersedObjects.swap(updated);
	}

	uint32_t InterspersedObservableVector::ToInnerIndex(uint32_t outerIndex) const
	{
		uint32_t offset = 0;
		for (const auto& [key, val] : _interspersedObjects)
		{
			if (key <= outerIndex)
			{
				offset++;
			}
		}
		return outerIndex - offset;
	}

	uint32_t InterspersedObservableVector::ToOuterIndex(uint32_t innerIndex) const
	{
		uint32_t offset = 0;
		for (const auto& [key, val] : _interspersedObjects)
		{
			if (key <= innerIndex + offset)
			{
				offset++;
			}
		}
		return innerIndex + offset;
	}

	uint32_t InterspersedObservableVector::ToOuterIndexAfterRemoval(uint32_t innerIndexToProject) const
	{
		uint32_t projected = innerIndexToProject;
		for (const auto& [key, val] : _interspersedObjects)
		{
			if (projected >= key)
			{
				projected++;
			}
			else
			{
				break;
			}
		}
		return projected;
	}

	void InterspersedObservableVector::ReadjustKeys()
	{
		uint32_t count = _itemsSource.Size();
		uint32_t existing = 0;

		std::vector<uint32_t> keys;
		for (const auto& kvp : _interspersedObjects)
		{
			keys.push_back(kvp.first);
		}

		for (auto key : keys)
		{
			auto it = _interspersedObjects.find(key);
			if (it == _interspersedObjects.end())
				continue;

			if (key <= count)
			{
				existing++;
				continue;
			}

			uint32_t newKey = count + existing++;
			_interspersedObjects[newKey] = it->second;
			_interspersedObjects.erase(it);
		}
	}

	void InterspersedObservableVector::RaiseVectorChanged(winrt::CollectionChange change, uint32_t index)
	{
		_vectorChanged(*this, winrt::make<args>(change, index));
	}

	void InterspersedObservableVector::OnItemsSourceChanged(winrt::IObservableVector<winrt::IInspectable> const&, winrt::IVectorChangedEventArgs const& args)
	{
		auto change = args.CollectionChange();
		uint32_t index = args.Index();

		switch (change)
		{
		case winrt::CollectionChange::ItemInserted:
			if (!_isInsertingOriginal)
			{
				MoveKeysForward(index, 1);
			}
			_isInsertingOriginal = false;
			RaiseVectorChanged(change, ToOuterIndex(index));
			break;

		case winrt::CollectionChange::ItemRemoved:
		{
			uint32_t outerIndex = ToOuterIndexAfterRemoval(index);
			MoveKeysBackward(outerIndex, 1);
			RaiseVectorChanged(change, outerIndex);
		}
		break;

		case winrt::CollectionChange::Reset:
			ReadjustKeys();
			RaiseVectorChanged(change, 0);
			break;
		}
	}

	bool InterspersedObservableVector::Remove(winrt::IInspectable const& value)
	{
		for (auto it = _interspersedObjects.begin(); it != _interspersedObjects.end(); ++it)
		{
			if (it->second == value)
			{
				uint32_t index = it->first;
				_interspersedObjects.erase(it);
				MoveKeysBackward(index, 1);
				RaiseVectorChanged(winrt::CollectionChange::ItemRemoved, index);
				return true;
			}
		}

		uint32_t innerIndex;
		if (_itemsSource.IndexOf(value, innerIndex))
		{
			_itemsSource.RemoveAt(innerIndex);
			return true;
		}

		return false;
	}
}
