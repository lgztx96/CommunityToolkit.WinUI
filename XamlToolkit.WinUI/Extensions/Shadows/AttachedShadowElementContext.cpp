#include "pch.h"
#include "winrt_module_imports.h"
#include "AttachedShadowElementContext.h"
#if __has_include("AttachedShadowElementContext.g.cpp")
#include "AttachedShadowElementContext.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	AttachedShadowElementContext::AttachedShadowElementContext() : _isConnected(false), _element{ nullptr } {}

	AttachedShadowElementContext::AttachedShadowElementContext(winrt::XamlToolkit::WinUI::AttachedShadowBase const& parent, winrt::FrameworkElement const& element)
		: _isConnected(false), _element{ nullptr }
	{
		if (!parent) throw winrt::hresult_invalid_argument(L"parent");
		if (!element) throw winrt::hresult_invalid_argument(L"element");

		Parent = parent;
		_element = element;

		_isConnected = true;

		_loadedToken = element.Loaded({ get_weak(), &AttachedShadowElementContext::OnElementLoaded });
		_unloadedToken = element.Unloaded({ get_weak(), &AttachedShadowElementContext::OnElementUnloaded });
		Initialize();
	}

	void AttachedShadowElementContext::DisconnectFromElement()
	{
		if (_isConnected)
		{
			UnInitialize();
			if (const auto element = Element())
			{
				element.Loaded(_loadedToken);
				element.Unloaded(_unloadedToken);
			}

			_isConnected = false;
		}
	}

	void AttachedShadowElementContext::CreateResources()
	{
		Initialize(true);
	}

	void AttachedShadowElementContext::Initialize(bool forceIfNotLoaded)
	{
		if (auto element = Element(); !IsInitialized() && _isConnected && (element.IsLoaded() || forceIfNotLoaded))
		{
			IsInitialized(true);
			ElementVisual(winrt::ElementCompositionPreview::GetElementVisual(element));
			Compositor(ElementVisual().Compositor());
			Shadow(Compositor().CreateDropShadow());
			SpriteVisual(Compositor().CreateSpriteVisual());
			SpriteVisual().RelativeSizeAdjustment(winrt::float2::one());
			SpriteVisual().Shadow(Shadow());
			if (Parent().as<IAttachedShadowBaseOverrides>().SupportsOnSizeChangedEvent())
			{
				_sizeChangedToken = element.SizeChanged({ get_weak(), &AttachedShadowElementContext::OnElementSizeChanged });
			}

			Parent().as<IAttachedShadowBaseOverrides>().OnElementContextInitialized(*this);
		}
	}

	void AttachedShadowElementContext::UnInitialize()
	{
		if (IsInitialized())
		{
			IsInitialized(false);

			if (const auto element = Element())
			{
				winrt::ElementCompositionPreview::SetElementChildVisual(element, nullptr);
				element.SizeChanged(_sizeChangedToken);
			}

			Parent().as<IAttachedShadowBaseOverrides>().OnElementContextUninitialized(*this);
			if (SpriteVisual())
			{
				SpriteVisual().Shadow(nullptr);
			}

			SpriteVisual(nullptr);
			Shadow(nullptr);
			ElementVisual(nullptr);
		}
	}

	void AttachedShadowElementContext::OnElementUnloaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		UnInitialize();
	}

	void AttachedShadowElementContext::OnElementLoaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		Initialize();
	}

	void AttachedShadowElementContext::OnElementSizeChanged([[maybe_unused]] winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e)
	{
		Parent().as<IAttachedShadowBaseOverrides>().OnSizeChanged(*this, e.NewSize(), e.PreviousSize());
	}

	winrt::IInspectable AttachedShadowElementContext::AddResource(winrt::hstring const& key, winrt::IInspectable const& resource)
	{
		_resources.insert_or_assign(key, resource);
		return resource;
	}

	winrt::IInspectable AttachedShadowElementContext::GetResource(std::wstring_view key)
	{
		if (auto iter = _resources.find(key); iter != _resources.end()) 
		{
			return iter->second;
		}

		return { nullptr };
	}

	void AttachedShadowElementContext::ClearAndDisposeResources()
	{
		for (const auto& [key, value] : _resources)
		{
			if (auto closable = value.try_as<winrt::IClosable>())
			{
				closable.Close();
			}
		}

		_resources.clear();
	}

	void AttachedShadowElementContext::RemoveAndDisposeResource(std::wstring_view key)
	{
		_resources.erase(key);
	}
}
