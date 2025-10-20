#include "pch.h"
#include "AttachedShadowElementContext.h"
#if __has_include("AttachedShadowElementContext.g.cpp")
#include "AttachedShadowElementContext.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
	AttachedShadowElementContext::AttachedShadowElementContext() : _isConnected(false), _element{ nullptr } {}

	AttachedShadowElementContext::AttachedShadowElementContext(CommunityToolkit::WinUI::AttachedShadowBase const& parent, FrameworkElement const& element)
		: _isConnected(false), _element{ element }
	{
		_isConnected = true;
		Parent = parent;

		if (!parent) throw winrt::hresult_invalid_argument(L"parent");
		if (!_element) throw winrt::hresult_invalid_argument(L"element");

		_loadedToken = element.Loaded({ get_weak(), &AttachedShadowElementContext::OnElementLoaded});
		_unloadedToken = element.Unloaded({ get_weak(), &AttachedShadowElementContext::OnElementUnloaded });
		Initialize();
	}


	void AttachedShadowElementContext::DisconnectFromElement()
	{
		if (_isConnected)
		{
			UnInitialize();
			if (auto element = Element()) {
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
			ElementVisual(ElementCompositionPreview::GetElementVisual(element));
			Compositor(ElementVisual().Compositor());
			Shadow(Compositor().CreateDropShadow());
			SpriteVisual(Compositor().CreateSpriteVisual());
			SpriteVisual().RelativeSizeAdjustment(winrt::Windows::Foundation::Numerics::float2::one());
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

			Parent().as<IAttachedShadowBaseOverrides>().OnElementContextUninitialized(*this);
			if (SpriteVisual())
			{
				SpriteVisual().Shadow(nullptr);
			}

			if (auto element = Element())
			{
				ElementCompositionPreview::SetElementChildVisual(element, nullptr);
				element.SizeChanged(_sizeChangedToken);
			}

			SpriteVisual(nullptr);
			Shadow(nullptr);
			ElementVisual(nullptr);
		}
	}

	void AttachedShadowElementContext::OnElementUnloaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		UnInitialize();
	}

	void AttachedShadowElementContext::OnElementLoaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		Initialize();
	}

	void AttachedShadowElementContext::OnElementSizeChanged([[maybe_unused]] IInspectable const& sender, SizeChangedEventArgs const& e)
	{
		Parent().as<IAttachedShadowBaseOverrides>().OnSizeChanged(*this, e.NewSize(), e.PreviousSize());
	}

	IInspectable AttachedShadowElementContext::AddResource(winrt::hstring const& key, IInspectable const& resource)
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

	IInspectable AttachedShadowElementContext::GetResource(std::wstring_view key)
	{
		if (auto iter = _resources.find(key); iter != _resources.end()) {
			return iter->second;
		}

		return { nullptr };
	}

	void AttachedShadowElementContext::ClearAndDisposeResources()
	{
		for (auto& [key, value] : _resources)
		{
			if (auto closeAble = value.try_as<Windows::Foundation::IClosable>())
				closeAble.Close();
		}

		_resources.clear();
	}

	void AttachedShadowElementContext::RemoveAndDisposeResource(std::wstring_view key)
	{
		if (auto iter = _resources.find(key); iter != _resources.end()) {
			_resources.erase(iter);
		}
	}
}
