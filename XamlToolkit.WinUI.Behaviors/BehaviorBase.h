#pragma once

#include "Behavior.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Windows::Foundation;
}

namespace winrt::XamlToolkit::WinUI::Behaviors
{
    /// <summary>
    /// Base class for behaviors that solves 2 problems:
    ///   1. Prevent duplicate initialization that can happen (prevent multiple OnAttached calls);
    ///   2. Whenever Initialize initially fails, this method will subscribe to FrameworkElement.SizeChanged to allow lazy initialization.
    /// </summary>
    /// <typeparam name="D">The derived type.</typeparam>
    /// <typeparam name="T">The type of the associated object.</typeparam>
    /// <remarks>
    /// For more info, see https://github.com/CommunityToolkit/WindowsCommunityToolkit/issues/1008.
    /// </remarks>
    template<typename D, typename T>
    struct BehaviorBase : Behavior
    {
        T AssociatedObject() const noexcept
        {
            return Behavior::AssociatedObject().try_as<T>();
        }

    protected:
        /// <summary>
        /// Gets a value indicating whether this behavior is attached.
        /// </summary>
        bool IsAttached() const noexcept { return _isAttached; }

        /// <summary>
        /// Called after the behavior is attached to the AssociatedObject.
        /// </summary>
        /// <remarks>
        /// Override this to hook up functionality to the AssociatedObject
        /// </remarks>
		virtual void OnAttached() override
        {
            auto object = AssociatedObject();
            if (object == nullptr)
            {
                auto actualType = winrt::get_class_name(object);
                std::wstring_view expectedType = winrt::name_of<winrt::FrameworkElement>();
                auto message = winrt::format(L"AssociatedObject is of type {0} but should be of type {1}.", actualType, expectedType);
                throw winrt::hresult_error(E_FAIL, message);
            }

            HandleAttach();

            if (auto frameworkElement = AssociatedObject().try_as<winrt::FrameworkElement>())
            {
                _loadedRevoker = frameworkElement.Loaded(winrt::auto_revoke, { this, &BehaviorBase::OnAssociatedObjectLoadedImpl });
                _unloadedRevoker = frameworkElement.Unloaded(winrt::auto_revoke, { this, &BehaviorBase::OnAssociatedObjectUnloadedImpl });
                _sizeChangedRevoker = frameworkElement.SizeChanged(winrt::auto_revoke, { this, &BehaviorBase::OnAssociatedObjectSizeChangedImpl });
            }
        }

        /// <summary>
        /// Called when the behavior is being detached from its AssociatedObject.
        /// </summary>
        /// <remarks>
        /// Override this to unhook functionality from the AssociatedObject
        /// </remarks>
        virtual void OnDetaching() override
        {
            _loadedRevoker.revoke();
            _unloadedRevoker.revoke();
            _sizeChangedRevoker.revoke();

            HandleDetach();
        }

        /// <summary>
        /// Called when the associated object has been loaded.
        /// </summary>
        virtual void OnAssociatedObjectLoaded() {}

        /// <summary>
        /// Called when the associated object has been unloaded.
        /// </summary>
        virtual void OnAssociatedObjectUnloaded() {}

        /// <summary>
        /// Initializes the behavior to the associated object.
        /// </summary>
        /// <returns>true if the initialization succeeded; otherwise false.</returns>
        virtual bool Initialize() { return true; }

        /// <summary>
        /// Uninitializes the behavior from the associated object.
        /// </summary>
        /// <returns>true if uninitialization succeeded; otherwise false.</returns>
        virtual bool Uninitialize() { return true; }

    private:
        void OnAssociatedObjectLoadedImpl([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
        {
            if (!_isAttached)
            {
                HandleAttach();
            }

            OnAssociatedObjectLoaded();
        }

        void OnAssociatedObjectUnloadedImpl([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
        {
            OnAssociatedObjectUnloaded();

            // Note: don't detach here, we'll let the behavior implementation take care of that
        }

        void OnAssociatedObjectSizeChangedImpl([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
        {
            if (!_isAttached)
            {
                HandleAttach();
            }
        }

        void HandleAttach()
        {
            if (_isAttaching || _isAttached)
            {
                return;
            }

            _isAttaching = true;

            bool attached = Initialize();
            if (attached)
            {
                _isAttached = true;
            }

            _isAttaching = false;
        }

        void HandleDetach()
        {
            if (!_isAttached)
            {
                return;
            }

            bool detached = Uninitialize();
            if (detached)
            {
                _isAttached = false;
            }
        }

        bool _isAttaching{ false };
        bool _isAttached{ false };

        winrt::FrameworkElement::Loaded_revoker _loadedRevoker;
        winrt::FrameworkElement::Unloaded_revoker _unloadedRevoker;
        winrt::FrameworkElement::SizeChanged_revoker _sizeChangedRevoker;
    };
}
