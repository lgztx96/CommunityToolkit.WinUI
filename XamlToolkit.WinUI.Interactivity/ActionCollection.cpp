#include "pch.h"
#include "winrt_module_imports.h"
#include "ActionCollection.h"
#if __has_include("ActionCollection.g.cpp")
#include "ActionCollection.g.cpp"
#endif
#include "Core/ResourceHelper.h"

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    ActionCollection::ActionCollection()
    {
        VectorChanged({ this, &ActionCollection::OnVectorChanged });
    }

    void ActionCollection::OnVectorChanged(
       [[maybe_unused]] winrt::IObservableVector<winrt::DependencyObject> const& sender, 
        winrt::IVectorChangedEventArgs const& eventArgs)
    {
        winrt::CollectionChange collectionChange = eventArgs.CollectionChange();

        if (collectionChange == winrt::CollectionChange::Reset)
        {
            for (const auto& item : *this)
            {
                ActionCollection::VerifyType(item);
            }
        }
        else if (collectionChange == winrt::CollectionChange::ItemInserted || collectionChange == winrt::CollectionChange::ItemChanged)
        {
            auto changedItem = GetAt(eventArgs.Index());
            ActionCollection::VerifyType(changedItem);
        }
    }

    void ActionCollection::VerifyType(winrt::Microsoft::UI::Xaml::DependencyObject const& item)
    {
        if (!item.try_as<IAction>())
        {
            throw winrt::hresult_invalid_argument(ResourceHelper::NonActionAddedToActionCollectionExceptionMessage());
        }
    }
}
