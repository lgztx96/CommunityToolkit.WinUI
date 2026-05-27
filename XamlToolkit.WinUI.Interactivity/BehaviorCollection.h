#pragma once

#include "BehaviorCollection.g.h"

#ifdef __INTELLISENSE__
#include <vector>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// Represents a collection of IBehaviors with a shared <see cref="AssociatedObject"/>.
    /// </summary>
    struct BehaviorCollection : BehaviorCollectionT<BehaviorCollection>
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BehaviorCollection"/> class.
        /// </summary>
        BehaviorCollection();

        /// <summary>
        /// Cleans up the <see cref="BehaviorCollection"/> and detaches all behaviors.
        /// </summary>
        ~BehaviorCollection();

        /// <summary>
        /// Gets the <see cref="DependencyObject"/> to which the <see cref="BehaviorCollection"/> is attached.
        /// </summary>
        winrt::DependencyObject AssociatedObject() const noexcept;

        /// <summary>
        /// Attaches the collection of behaviors to the specified <see cref="DependencyObject"/>.
        /// </summary>
        /// <param name="associatedObject">The <see cref="DependencyObject"/> to which to attach.</param>
        void Attach(winrt::DependencyObject const& associatedObject);

        /// <summary>
        /// Detaches the collection of behaviors from the <see cref="AssociatedObject"/>.
        /// </summary>
        void Detach();

    private:
        void OnVectorChanged(
            winrt::IObservableVector<winrt::DependencyObject> const& sender,
            winrt::IVectorChangedEventArgs const& eventArgs);

        winrt::XamlToolkit::WinUI::Interactivity::IBehavior VerifiedAttach(winrt::DependencyObject const& item);

#if _DEBUG
        void VerifyOldCollectionIntegrity();
#endif

        std::vector<winrt::XamlToolkit::WinUI::Interactivity::IBehavior> _oldCollection;
        winrt::weak_ref<winrt::DependencyObject> _associatedObject{ nullptr };
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct BehaviorCollection : BehaviorCollectionT<BehaviorCollection, implementation::BehaviorCollection>
    {
    };
}
