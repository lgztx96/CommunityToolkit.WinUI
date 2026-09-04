#pragma once

#include "AttachedShadowElementContext.g.h"
#include "TypedResourceKey.h"

#ifdef __INTELLISENSE__
#include <map>
#include <optional>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
	using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct AttachedShadowElementContext : AttachedShadowElementContextT<AttachedShadowElementContext>
	{
		bool _isConnected;
		std::map<winrt::hstring, winrt::IInspectable, std::less<>> _resources;
		winrt::FrameworkElement::SizeChanged_revoker _sizeChangedRevoker;
		winrt::FrameworkElement::Loaded_revoker _loadedRevoker;
		winrt::FrameworkElement::Unloaded_revoker _unloadedRevoker;
		winrt::FrameworkElement::LayoutUpdated_revoker _layoutUpdatedRevoker;

		wil::single_threaded_rw_property<std::optional<uint64_t>> VisibilityToken;

		wil::single_threaded_property<winrt::XamlToolkit::WinUI::AttachedShadowBase> Parent{ nullptr };

		winrt::weak_ref<winrt::FrameworkElement> _element;

		winrt::FrameworkElement Element() const noexcept { return _element.get(); }

		wil::single_threaded_rw_property<bool> IsInitialized{ false };
		wil::single_threaded_rw_property<winrt::Visual> ElementVisual{ nullptr };
		wil::single_threaded_rw_property<winrt::Compositor> Compositor{ nullptr };
		wil::single_threaded_rw_property<winrt::SpriteVisual> SpriteVisual{ nullptr };
		wil::single_threaded_rw_property<winrt::DropShadow> Shadow{ nullptr };

		AttachedShadowElementContext();

		AttachedShadowElementContext(winrt::XamlToolkit::WinUI::AttachedShadowBase const& parent, winrt::FrameworkElement const& element);

		void DisconnectFromElement();

		void CreateResources();

		void Initialize(bool forceIfNotLoaded = false);

		void UnInitialize();

		void OnElementUnloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void OnElementLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void OnElementSizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		winrt::IInspectable AddResource(winrt::hstring const& key, winrt::IInspectable const& resource);

		winrt::IInspectable GetResource(std::wstring_view key);

		void ClearAndDisposeResources();

		void RemoveAndDisposeResource(std::wstring_view key);

		template <typename T>
		T AddResource(winrt::hstring key, T resource)
		{
			if (_resources.contains(key))
			{
				_resources[key] = resource;
			}
			else
			{
				_resources.emplace(key, resource);
			}

			return resource;
		}

		template <typename T>
		bool TryGetResource(std::wstring_view key, T&& resource)
		{
			if (auto iter = _resources.find(key); iter != _resources.end()) {
				if (iter->second) {
					resource = iter->second;
					return true;
				}
			}

			return false;
		}

		template <typename T>
		T GetResource(std::wstring_view key)
		{
			T resource{ nullptr };
			if (TryGetResource<T>(key, resource))
			{
				return resource;
			}

			return resource;
		}

		template <typename T>
		T RemoveResource(std::wstring_view key)
		{
			if (auto iter = _resources.find(key); iter != _resources.end())
			{
				_resources.erase(key);
				if (iter->second)
				{
					return iter->second.try_as<T>();
				}
			}

			return { nullptr };
		}

		template <typename T>
		T AddResource(TypedResourceKey<T> key, T resource)
		{
			return AddResource(key.Key(), resource);
		}

		template <typename T>
		bool TryGetResource(TypedResourceKey<T> key, T&& resource)
		{
			return TryGetResource(key.Key(), resource);
		}

		template <typename T>
		T GetResource(TypedResourceKey<T> key)
		{
			return GetResource(key.Key());
		}

		template <typename T>
		T RemoveResource(TypedResourceKey<T> key)
		{
			return RemoveResource(key.Key());
		}
	};
}
