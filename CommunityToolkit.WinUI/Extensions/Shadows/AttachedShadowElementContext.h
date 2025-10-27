#pragma once

#include "AttachedShadowElementContext.g.h"

#include <winrt/Microsoft.UI.Xaml.Hosting.h>

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include "TypedResourceKey.h"

namespace winrt::CommunityToolkit::WinUI::implementation
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
	using namespace Microsoft::UI::Xaml::Hosting;

	struct AttachedShadowElementContext : AttachedShadowElementContextT<AttachedShadowElementContext>
	{
		bool _isConnected;
		std::map<winrt::hstring, IInspectable, std::less<>> _resources;
		FrameworkElement::SizeChanged_revoker _sizeChangedRevoker;
		FrameworkElement::Loaded_revoker _loadedRevoker;
		FrameworkElement::Unloaded_revoker _unloadedRevoker;

		wil::single_threaded_property<CommunityToolkit::WinUI::AttachedShadowBase> Parent{ nullptr };

		winrt::weak_ref<FrameworkElement> _element;

		FrameworkElement Element() const noexcept { return _element.get(); }

		wil::single_threaded_rw_property<bool> IsInitialized{ false };
		wil::single_threaded_rw_property<Visual> ElementVisual{ nullptr };
		wil::single_threaded_rw_property<Compositor> Compositor{ nullptr };
		wil::single_threaded_rw_property<SpriteVisual> SpriteVisual{ nullptr };
		wil::single_threaded_rw_property<DropShadow> Shadow{ nullptr };

		AttachedShadowElementContext();

		AttachedShadowElementContext(CommunityToolkit::WinUI::AttachedShadowBase const& parent, FrameworkElement const& element);

		void DisconnectFromElement();

		void CreateResources();

		void Initialize(bool forceIfNotLoaded = false);

		void UnInitialize();

		void OnElementUnloaded(IInspectable const& sender, RoutedEventArgs const& e);

		void OnElementLoaded(IInspectable const& sender, RoutedEventArgs const& e);

		void OnElementSizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

		IInspectable AddResource(winrt::hstring const& key, IInspectable const& resource);

		IInspectable GetResource(std::wstring_view key);

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

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
	struct AttachedShadowElementContext : AttachedShadowElementContextT<AttachedShadowElementContext, implementation::AttachedShadowElementContext>
	{
	};
}
