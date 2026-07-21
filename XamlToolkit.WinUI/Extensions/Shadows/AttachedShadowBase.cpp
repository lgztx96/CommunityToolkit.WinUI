#include "pch.h"
#include "winrt_module_imports.h"
#include "AttachedShadowBase.h"
#if __has_include("AttachedShadowBase.g.cpp")
#include "AttachedShadowBase.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	void AttachedShadowBase::OnDependencyPropertyChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (auto shadow = sender.try_as<class_type>())
		{
			auto ref = winrt::get_self<AttachedShadowBase>(shadow)->get_strong();
			ref->CallPropertyChangedForEachElement(args.Property(), args.OldValue(), args.NewValue());
		}
	}

	void AttachedShadowBase::ConnectElement(winrt::FrameworkElement const& element)
	{
		std::erase_if(ShadowElementContextTable, [](auto&& value)
		{
			if (!value.first.get())
			{
				value.second.DisconnectFromElement();
				return true;
			}

			return false;
		});

		if (ShadowElementContextTable.contains(element))
		{
			return;
		}

		auto context = winrt::make<winrt::XamlToolkit::WinUI::implementation::AttachedShadowElementContext>(*this, element);
		ShadowElementContextTable.emplace(element, context);
	}

	void AttachedShadowBase::DisconnectElement(winrt::FrameworkElement const& element)
	{
		std::erase_if(ShadowElementContextTable, [](auto&& value)
		{
			if (!value.first.get())
			{
				value.second.DisconnectFromElement();
				return true;
			}

			return false;
		});

		if (auto iter = ShadowElementContextTable.find(element); iter != ShadowElementContextTable.end())
		{
			ShadowElementContextTable.erase(iter);
		}
	}

	winrt::XamlToolkit::WinUI::AttachedShadowElementContext AttachedShadowBase::GetElementContext(winrt::FrameworkElement const& element)
	{
		std::erase_if(ShadowElementContextTable, [](auto&& value)
		{
			if (!value.first.get())
			{
				value.second.DisconnectFromElement();
				return true;
			}

			return false;
		});

		if (auto iter = ShadowElementContextTable.find(element); iter != ShadowElementContextTable.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	bool AttachedShadowBase::SupportsOnSizeChangedEvent() const noexcept { return false; }

	void AttachedShadowBase::OnSizeChanged(
		[[maybe_unused]] winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context,
		[[maybe_unused]] winrt::Size newSize,
		[[maybe_unused]] winrt::Size previousSize)
	{

	}

	void AttachedShadowBase::OnElementContextInitialized(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		overridable().OnPropertyChanged(context, OpacityProperty, winrt::box_value(Opacity()), winrt::box_value(Opacity()));
		overridable().OnPropertyChanged(context, BlurRadiusProperty, winrt::box_value(BlurRadius()), winrt::box_value(BlurRadius()));
		overridable().OnPropertyChanged(context, ColorProperty, winrt::box_value(Color()), winrt::box_value(Color()));
		overridable().OnPropertyChanged(context, OffsetProperty, winrt::box_value(Offset()), winrt::box_value(Offset()));
		UpdateShadowClip(context);
		UpdateShadowMask(context);
		overridable().SetElementChildVisual(context);
	}

	void AttachedShadowBase::OnElementContextUninitialized(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		winrt::ElementCompositionPreview::SetElementChildVisual(context.Element(), nullptr);
		context.ClearAndDisposeResources();
	}

	winrt::IVector<winrt::XamlToolkit::WinUI::AttachedShadowElementContext> AttachedShadowBase::EnumerateElementContexts()
	{
		std::vector<winrt::XamlToolkit::WinUI::AttachedShadowElementContext> vec;

		for (const auto& [elementRef, ctx] : ShadowElementContextTable)
		{
			if (auto element = elementRef.get())
			{
				vec.emplace_back(ctx);
			}
		}

		return winrt::single_threaded_vector<winrt::XamlToolkit::WinUI::AttachedShadowElementContext>(std::move(vec));
	}

	void AttachedShadowBase::SetElementChildVisual(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		ElementCompositionPreview::SetElementChildVisual(context.Element(), context.SpriteVisual());
	}

	void AttachedShadowBase::CallPropertyChangedForEachElement(winrt::DependencyProperty const& property, winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue)
	{
		for (const auto& [elementRef, ctx] : ShadowElementContextTable)
		{
			if (ctx.IsInitialized())
			{
				overridable().OnPropertyChanged(ctx, property, oldValue, newValue);
			}
		}
	}

	winrt::CompositionBrush AttachedShadowBase::GetShadowMask([[maybe_unused]] winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		return nullptr;
	}

	winrt::CompositionClip AttachedShadowBase::GetShadowClip([[maybe_unused]] winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		return nullptr;
	}

	void AttachedShadowBase::UpdateShadowMask(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		if (const auto shadow = context.Shadow(); shadow && context.IsInitialized())
		{
			shadow.Mask(overridable().GetShadowMask(context));
		}
	}

	void AttachedShadowBase::UpdateShadowClip(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		if (const auto spriteVisual = context.SpriteVisual(); spriteVisual && context.IsInitialized())
		{
			spriteVisual.Clip(overridable().GetShadowClip(context));
		}
	}

	void AttachedShadowBase::OnPropertyChanged(winrt::XamlToolkit::WinUI::AttachedShadowElementContext const& context, winrt::DependencyProperty const& property, [[maybe_unused]] winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue)
	{
		if (!context.IsInitialized() || context.Shadow() == nullptr)
		{
			return;
		}

		if (property == BlurRadiusProperty)
		{
			context.Shadow().BlurRadius(static_cast<float>(winrt::unbox_value<double>(newValue)));
		}
		else if (property == OpacityProperty)
		{
			context.Shadow().Opacity(static_cast<float>(winrt::unbox_value<double>(newValue)));
		}
		else if (property == ColorProperty)
		{
			context.Shadow().Color(winrt::unbox_value<winrt::Color>(newValue));
		}
		else if (property == OffsetProperty)
		{
			context.Shadow().Offset(ToVector3(winrt::unbox_value<winrt::hstring>(newValue)));
		}
	}
}
