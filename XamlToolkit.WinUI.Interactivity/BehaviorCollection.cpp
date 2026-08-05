#include "pch.h"
#include "winrt_module_imports.h"
#include "BehaviorCollection.h"
#if __has_include("BehaviorCollection.g.cpp")
#include "BehaviorCollection.g.cpp"
#endif

#include "Core/ResourceHelper.h"

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
	BehaviorCollection::BehaviorCollection()
	{
		VectorChanged({ this, &BehaviorCollection::OnVectorChanged });
	}

	BehaviorCollection::~BehaviorCollection() { Detach(); }

	winrt::DependencyObject BehaviorCollection::AssociatedObject() const noexcept
	{
		if (auto strongRef = _associatedObject.get())
		{
			return strongRef;
		}

		return nullptr;
	}

	void BehaviorCollection::Attach(winrt::DependencyObject const& associatedObject)
	{
		if (associatedObject == AssociatedObject())
		{
			return;
		}

		if (winrt::Windows::ApplicationModel::DesignMode::DesignModeEnabled())
		{
			return;
		}

		if (_associatedObject)
		{
			throw winrt::hresult_error(E_FAIL, ResourceHelper::CannotAttachBehaviorMultipleTimesExceptionMessage());
		}

		WINRT_ASSERT(associatedObject);
		_associatedObject = associatedObject;

		for (winrt::DependencyObject const& item : *this)
		{
			VerifiedAttach(item);
		}
	}

	void BehaviorCollection::Detach()
	{
		for (const auto& item : _oldCollection)
		{
			if (item.AssociatedObject())
			{
				item.Detach();
			}
		}

		_oldCollection.clear();
		_associatedObject = nullptr;
	}

	void BehaviorCollection::OnVectorChanged(
		[[maybe_unused]] winrt::IObservableVector<winrt::DependencyObject> const& sender,
		winrt::IVectorChangedEventArgs const& eventArgs)
	{
		const auto collectionChange = eventArgs.CollectionChange();

		if (collectionChange == winrt::CollectionChange::Reset)
		{
			for (const auto& behavior : _oldCollection)
			{
				if (behavior.AssociatedObject())
				{
					behavior.Detach();
				}
			}

			_oldCollection.clear();
			_oldCollection.reserve(Size());

			for (winrt::DependencyObject const& newItem : *this)
			{
				_oldCollection.push_back(VerifiedAttach(newItem));
			}

#if _DEBUG
			VerifyOldCollectionIntegrity();
#endif
			return;
		}

		uint32_t const eventIndex = eventArgs.Index();
		winrt::DependencyObject changedItem{ nullptr };

		if (collectionChange != winrt::CollectionChange::ItemRemoved)
		{
			changedItem = GetAt(eventIndex);
		}

		switch (collectionChange)
		{
		case winrt::CollectionChange::ItemInserted:
			_oldCollection.insert(_oldCollection.begin() + static_cast<std::ptrdiff_t>(eventIndex), VerifiedAttach(changedItem));
			break;

		case winrt::CollectionChange::ItemChanged:
		{
			const auto& oldItem = _oldCollection[eventIndex];
			if (oldItem.AssociatedObject())
			{
				oldItem.Detach();
			}

			_oldCollection[eventIndex] = VerifiedAttach(changedItem);
			break;
		}

		case winrt::CollectionChange::ItemRemoved:
		{
			const auto& oldItem = _oldCollection[eventIndex];
			if (oldItem.AssociatedObject())
			{
				oldItem.Detach();
			}

			_oldCollection.erase(_oldCollection.begin() + static_cast<std::ptrdiff_t>(eventIndex));
			break;
		}

		default:
			WINRT_ASSERT(false);
			break;
		}

#if _DEBUG
		VerifyOldCollectionIntegrity();
#endif
	}

	winrt::XamlToolkit::WinUI::Interactivity::IBehavior BehaviorCollection::VerifiedAttach(winrt::DependencyObject const& item)
	{
		auto behavior = item.try_as<winrt::XamlToolkit::WinUI::Interactivity::IBehavior>();
		if (behavior == nullptr)
		{
			throw winrt::hresult_error(E_FAIL, ResourceHelper::NonBehaviorAddedToBehaviorCollectionExceptionMessage());
		}

		if (std::find(_oldCollection.begin(), _oldCollection.end(), behavior) != _oldCollection.end())
		{
			throw winrt::hresult_error(E_FAIL, ResourceHelper::DuplicateBehaviorInCollectionExceptionMessage());
		}

		if (auto strongRef = _associatedObject.get())
		{
			behavior.Attach(strongRef);
		}

		return behavior;
	}

#if _DEBUG
	void BehaviorCollection::VerifyOldCollectionIntegrity()
	{
		bool isValid = Size() == _oldCollection.size();

		if (isValid)
		{
			for (uint32_t i = 0; i < Size(); ++i)
			{
				if (GetAt(i) != _oldCollection[i].as<winrt::DependencyObject>())
				{
					isValid = false;
					break;
				}
			}
		}

		WINRT_ASSERT(isValid);
	}
#endif
}
