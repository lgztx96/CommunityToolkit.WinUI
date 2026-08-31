#include "pch.h"
#include "winrt_module_imports.h"
#include "IsNullOrEmptyStateTrigger.h"
#if __has_include("IsNullOrEmptyStateTrigger.g.cpp")
#include "IsNullOrEmptyStateTrigger.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	IsNullOrEmptyStateTrigger::IsNullOrEmptyStateTrigger() { UpdateTrigger(); }

	const wil::single_threaded_property<winrt::DependencyProperty> IsNullOrEmptyStateTrigger::ValueProperty =
		winrt::DependencyProperty::Register(
			L"Value",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &IsNullOrEmptyStateTrigger::OnValuePropertyChanged));

	void IsNullOrEmptyStateTrigger::OnValuePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto impl = winrt::get_self<IsNullOrEmptyStateTrigger>(d.as<class_type>())->get_strong();

		if (const auto oldValue = e.OldValue())
		{
			if (const auto oldNotifyCollection = oldValue.try_as<winrt::INotifyCollectionChanged>())
			{
				oldNotifyCollection.CollectionChanged(impl->_containerChangedToken);
			}
			else if (const auto oldObservableVector = oldValue.try_as<winrt::IObservableVector<winrt::IInspectable>>())
			{
				oldObservableVector.VectorChanged(impl->_containerChangedToken);
			}
			else if (const auto oldObservableMap = oldValue.try_as<winrt::IObservableMap<winrt::IInspectable, winrt::IInspectable>>())
			{
				oldObservableMap.MapChanged(impl->_containerChangedToken);
			}
			else if (const auto oldBindableObservableVector = oldValue.try_as<winrt::IBindableObservableVector>())
			{
				oldBindableObservableVector.VectorChanged(impl->_containerChangedToken);
			}

			impl->_containerChangedToken = { 0 };
		}

		impl->UpdateTrigger();
	}

	void IsNullOrEmptyStateTrigger::UpdateTrigger()
	{
		const auto val = Value();

		SetActive(IsNullOrEmpty(val));

		if (val == nullptr)
		{
			return;
		}

		// Try to listen for various notification events
		// Starting with INotifyCollectionChanged
		if (const auto valNotifyCollection = val.try_as<winrt::INotifyCollectionChanged>())
		{
			_containerChangedToken = valNotifyCollection.CollectionChanged({ get_weak(), [this](auto&& source, auto&&) { SetActive(IsNullOrEmpty(source)); } });
			return;
		}

		// Not INotifyCollectionChanged, try IObservableVector
		if (const auto valObservableVector = val.try_as<winrt::IObservableVector<winrt::IInspectable>>())
		{
			_containerChangedToken = valObservableVector.VectorChanged({ get_weak(), [this](auto&& source, auto&&) { SetActive(IsNullOrEmpty(source)); } });
			return;
		}

		// Not IObservableVector, try IObservableMap
		if (const auto valObservableMap = val.try_as<winrt::IObservableMap<winrt::IInspectable, winrt::IInspectable>>())
		{
			_containerChangedToken = valObservableMap.MapChanged({ get_weak(), [this](auto&& source, auto&&) { SetActive(IsNullOrEmpty(source)); } });
			return;
		}

		// Not IObservableMap, try IBindableObservableVector
		if (const auto valBindableObservableVector = val.try_as<winrt::IBindableObservableVector>())
		{
			_containerChangedToken = valBindableObservableVector.VectorChanged({ get_weak(), [this](auto&& source, auto&&) { SetActive(IsNullOrEmpty(source)); } });
		}
	}

	bool IsNullOrEmptyStateTrigger::IsNullOrEmpty(winrt::IInspectable const& val)
	{
		if (val == nullptr)
		{
			return true;
		}

		// Object is not null, check for an empty string
		if (const auto valString = val.try_as<winrt::hstring>())
		{
			return valString->empty();
		}

		// Object is not a hstring, check for an empty IVector (faster)
		if (const auto valVector = val.try_as<winrt::IVector<winrt::IInspectable>>())
		{
			return valVector.Size() == 0;
		}

		// Object is not an IVector, check for an empty IIterable
		if (const auto valIterable = val.try_as<winrt::IIterable<winrt::IInspectable>>())
		{
			return !valIterable.First().HasCurrent();
		}

		// Object is not an IIterable, check for an empty IBindableVector
		if (const auto valBindableVector = val.try_as<winrt::IBindableVector>())
		{
			return valBindableVector.Size() == 0;
		}

		// Object is not an IBindableVector, check for an empty IBindableVectorView
		if (const auto valBindableVectorView = val.try_as<winrt::IBindableVectorView>())
		{
			return valBindableVectorView.Size() == 0;
		}

		// Object is not an IBindableVectorView, check for an empty IBindableIterable
		if (const auto valBindableIterable = val.try_as<winrt::IBindableIterable>())
		{
			return !valBindableIterable.First().HasCurrent();
		}

		// Not null and not a known type to test for emptiness
		return false;
	}
}
