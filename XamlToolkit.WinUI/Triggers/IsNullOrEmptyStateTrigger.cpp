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

    void IsNullOrEmptyStateTrigger::OnValuePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        winrt::get_self<IsNullOrEmptyStateTrigger>(d.as<class_type>())->UpdateTrigger();
    }

    void IsNullOrEmptyStateTrigger::UpdateTrigger()
    {
        auto val = Value();

        SetActive(IsNullOrEmpty(val));

        if (val == nullptr)
        {
            return;
        }

        // Try to listen for various notification events
        // Starting with INotifyCollectionChanged
        if (auto valNotifyCollection = val.try_as<winrt::INotifyCollectionChanged>())
        {
            valNotifyCollection.CollectionChanged({ get_weak(), [this](auto&& source, auto&&) { SetActive(IsNullOrEmpty(source)); } });
            return;
        }

        // Not INotifyCollectionChanged, try IObservableVector
        if (auto valObservableVector = val.try_as<winrt::IObservableVector<winrt::IInspectable>>())
        {
            valObservableVector.VectorChanged({ get_weak(), [this](auto&& source, auto&&) { SetActive(IsNullOrEmpty(source)); } });
            return;
        }

        // Not INotifyCollectionChanged, try IObservableMap
        if (auto valObservableMap = val.try_as<winrt::IObservableMap<winrt::IInspectable, winrt::IInspectable>>())
        {
            valObservableMap.MapChanged({ get_weak(), [this](auto&& source, auto&&) { SetActive(IsNullOrEmpty(source)); } });
        }
    }

    bool IsNullOrEmptyStateTrigger::IsNullOrEmpty(winrt::IInspectable const& val)
    {
        if (val == nullptr)
        {
            return true;
        }

        // Object is not null, check for an empty string
        if (auto valString = val.try_as<winrt::hstring>(); valString)
        {
            return valString->empty();
        }

        // Object is not a string, check for an empty ICollection (faster)
        if (auto valCollection = val.try_as<winrt::IVector<winrt::IInspectable>>())
        {
            return valCollection.Size() == 0;
        }

        // Object is not an ICollection, check for an empty IEnumerable
        if (auto valEnumerable = val.try_as<winrt::IIterable<winrt::IInspectable>>())
        {
            return valEnumerable.begin() == valEnumerable.end();
        }

        // Not null and not a known type to test for emptiness
        return false;
    }
}
