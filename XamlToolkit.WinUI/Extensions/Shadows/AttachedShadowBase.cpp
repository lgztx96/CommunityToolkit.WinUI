#include "pch.h"
#include "winrt_module_imports.h"
#include "AttachedShadowBase.h"
#if __has_include("AttachedShadowBase.g.cpp")
#include "AttachedShadowBase.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	void AttachedShadowBase::OnDependencyPropertyChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& args)
	{
		if (auto shadow = sender.try_as<class_type>())
		{
			auto ref = winrt::get_self<AttachedShadowBase>(shadow)->get_strong();
			ref->CallPropertyChangedForEachElement(args.Property(), args.OldValue(), args.NewValue());
		}
	}

	void AttachedShadowBase::ConnectElement(FrameworkElement const& element)
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

		auto context = winrt::make<XamlToolkit::WinUI::implementation::AttachedShadowElementContext>(*this, element);
		ShadowElementContextTable.emplace(element, context);
	}

	void AttachedShadowBase::DisconnectElement(FrameworkElement const& element)
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

	XamlToolkit::WinUI::AttachedShadowElementContext AttachedShadowBase::GetElementContext(FrameworkElement const& element)
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
		[[maybe_unused]] XamlToolkit::WinUI::AttachedShadowElementContext const& context,
		[[maybe_unused]] Windows::Foundation::Size newSize,
		[[maybe_unused]] Windows::Foundation::Size previousSize)
	{

	}

	void AttachedShadowBase::OnElementContextInitialized(XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		overridable().OnPropertyChanged(context, OpacityProperty, winrt::box_value(Opacity()), winrt::box_value(Opacity()));
		overridable().OnPropertyChanged(context, BlurRadiusProperty, winrt::box_value(BlurRadius()), winrt::box_value(BlurRadius()));
		overridable().OnPropertyChanged(context, ColorProperty, winrt::box_value(Color()), winrt::box_value(Color()));
		overridable().OnPropertyChanged(context, OffsetProperty, winrt::box_value(Offset()), winrt::box_value(Offset()));
		UpdateShadowClip(context);
		UpdateShadowMask(context);
		overridable().SetElementChildVisual(context);
	}

	void AttachedShadowBase::OnElementContextUninitialized(XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		ElementCompositionPreview::SetElementChildVisual(context.Element(), nullptr);
		context.ClearAndDisposeResources();
	}

	Windows::Foundation::Collections::IVector<XamlToolkit::WinUI::AttachedShadowElementContext> AttachedShadowBase::EnumerateElementContexts()
	{
		std::vector<XamlToolkit::WinUI::AttachedShadowElementContext> vec;

		for (const auto& [elementRef, ctx] : ShadowElementContextTable)
		{
			if (auto element = elementRef.get())
			{
				vec.emplace_back(ctx);
			}
		}

		return winrt::single_threaded_vector<XamlToolkit::WinUI::AttachedShadowElementContext>(std::move(vec));
	}

	void AttachedShadowBase::SetElementChildVisual(XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		ElementCompositionPreview::SetElementChildVisual(context.Element(), context.SpriteVisual());
	}

	void AttachedShadowBase::CallPropertyChangedForEachElement(DependencyProperty const& property, IInspectable const& oldValue, IInspectable const& newValue)
	{
		for (const auto& [elementRef, ctx] : ShadowElementContextTable)
		{
			if (ctx.IsInitialized())
			{
				overridable().OnPropertyChanged(ctx, property, oldValue, newValue);
			}
		}
	}

	CompositionBrush AttachedShadowBase::GetShadowMask([[maybe_unused]] XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		return nullptr;
	}

	CompositionClip AttachedShadowBase::GetShadowClip([[maybe_unused]] XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		return nullptr;
	}

	void AttachedShadowBase::UpdateShadowMask(XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		if (context.IsInitialized() && context.Shadow())
		{
			context.Shadow().Mask(overridable().GetShadowMask(context));
		}
	}

	void AttachedShadowBase::UpdateShadowClip(XamlToolkit::WinUI::AttachedShadowElementContext const& context)
	{
		if (context.IsInitialized() && context.SpriteVisual())
		{
			context.SpriteVisual().Clip(overridable().GetShadowClip(context));
		}
	}

	void AttachedShadowBase::OnPropertyChanged(XamlToolkit::WinUI::AttachedShadowElementContext const& context, DependencyProperty const& property, [[maybe_unused]] IInspectable const& oldValue, IInspectable const& newValue)
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
			context.Shadow().Color(winrt::unbox_value<Windows::UI::Color>(newValue));
		}
		else if (property == OffsetProperty)
		{
			context.Shadow().Offset(ToVector3(winrt::unbox_value<winrt::hstring>(newValue)));
		}
	}
}
