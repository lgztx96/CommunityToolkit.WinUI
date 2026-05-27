#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Behaviors
{
    /// <summary>
    /// A base class for behaviors, implementing the basic plumbing of IBehavior
    /// </summary>
    struct Behavior
    {
        /// <summary>
        /// Gets the <see cref="DependencyObject"/> to which the behavior is attached.
        /// </summary>
        winrt::DependencyObject AssociatedObject() const noexcept
        {
            return _associatedObject;
        }

        /// <summary>
        /// Attaches the behavior to the specified <see cref="DependencyObject"/>.
        /// </summary>
        /// <param name="associatedObject">The <see cref="DependencyObject"/> to which to attach.</param>
        /// <exception cref="global::System.ArgumentNullException"><paramref name="associatedObject"/> is null.</exception>
        void Attach(winrt::DependencyObject const& associatedObject)
        {
            if (associatedObject == _associatedObject)
            {
                return;
            }

            if (_associatedObject != nullptr)
            {
                throw winrt::hresult_error(E_FAIL, L"Cannot attach behavior multiple times.");
            }

            if (!associatedObject)
            {
                throw winrt::hresult_invalid_argument(L"associatedObject");
            }

            _associatedObject = associatedObject;

            OnAttached();
        }

        /// <summary>
        /// Detaches the behaviors from the <see cref="Microsoft.Xaml.Interactivity.Behavior.AssociatedObject"/>.
        /// </summary>
        void Detach()
        {
            OnDetaching();
            _associatedObject = nullptr;
        }

        /// <summary>
        /// Called after the behavior is attached to the <see cref="Microsoft.Xaml.Interactivity.Behavior.AssociatedObject"/>.
        /// </summary>
        /// <remarks>
        /// Override this to hook up functionality to the <see cref="Microsoft.Xaml.Interactivity.Behavior.AssociatedObject"/>
        /// </remarks>
        virtual void OnAttached() = 0;

        /// <summary>
        /// Called when the behavior is being detached from its <see cref="Microsoft.Xaml.Interactivity.Behavior.AssociatedObject"/>.
        /// </summary>
        /// <remarks>
        /// Override this to unhook functionality from the <see cref="Microsoft.Xaml.Interactivity.Behavior.AssociatedObject"/>
        /// </remarks>
        virtual void OnDetaching() = 0;

    private:

        winrt::DependencyObject _associatedObject{ nullptr };
    };

}
