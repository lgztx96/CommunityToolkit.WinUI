#include "pch.h"
#include "winrt_module_imports.h"
#include "VisualAttach.h"
#if __has_include("VisualAttach.g.cpp")
#include "VisualAttach.g.cpp"
#endif

#include "string_utils.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	void VisualAttach::OnNormalizedCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto element = d.try_as<FrameworkElement>())
		{
			if (auto newValue = e.NewValue().try_as<hstring>())
			{
				auto center = winrt::string_to_float2(newValue.value());
				auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);

				static constexpr auto expression = L"Vector2(this.Target.Size.X * X, this.Target.Size.Y * Y)";

				auto animation = visual.Compositor().CreateExpressionAnimation(expression);
				animation.SetScalarParameter(L"X", center.x);
				animation.SetScalarParameter(L"Y", center.y);

				visual.StopAnimation(L"CenterPoint.XY");
				visual.StartAnimation(L"CenterPoint.XY", animation);
			}
		}
	}

	void VisualAttach::OnIsBindCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (e.NewValue() != e.OldValue())
		{
			if (auto element = d.try_as<UIElement>())
			{
				auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);

				if (unbox_value<bool>(e.NewValue()))
				{
					static constexpr auto expression = L"Vector3(this.Target.Size.X / 2, this.Target.Size.Y / 2, 0f)";

					auto animation = visual.Compositor().CreateExpressionAnimation(expression);
					visual.StartAnimation(L"CenterPoint", animation);
				}
				else
				{
					visual.StopAnimation(L"CenterPoint");
				}
			}
		}
	}

	void VisualAttach::OnScaleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto string = e.NewValue().try_as<hstring>())
		{
			SetScale(d, string.value_or(L"1"));
		}
	}

	bool VisualAttach::GetIsBindCenterPoint(winrt::DependencyObject const& obj) noexcept
	{
		return winrt::unbox_value<bool>(obj.GetValue(IsBindCenterPointProperty));
	}

	void VisualAttach::SetIsBindCenterPoint(winrt::DependencyObject const& obj, bool value)
	{
		obj.SetValue(IsBindCenterPointProperty, winrt::box_value(value));
	}

	hstring VisualAttach::GetNormalizedCenterPoint(winrt::DependencyObject const& obj) noexcept
	{
		return winrt::unbox_value<hstring>(obj.GetValue(NormalizedCenterPointProperty));
	}

	void VisualAttach::SetNormalizedCenterPoint(winrt::DependencyObject const& obj, winrt::hstring const& value)
	{
		obj.SetValue(NormalizedCenterPointProperty, winrt::box_value(value));
	}

	hstring VisualAttach::GetScale(winrt::DependencyObject const& obj) noexcept
	{
		if (auto element = obj.try_as<UIElement>())
		{
			return GetScaleForElement(element);
		}
		return winrt::unbox_value<hstring>(obj.GetValue(ScaleProperty));
	}

	void VisualAttach::SetScale(winrt::DependencyObject const& obj, winrt::hstring const& value)
	{
		if (auto element = obj.try_as<UIElement>())
		{
			SetScaleForElement(value, element);
		}
		obj.SetValue(ScaleProperty, winrt::box_value(value));
	}

	hstring VisualAttach::GetScaleForElement(winrt::UIElement const& element) noexcept
	{
		auto visual = winrt::ElementCompositionPreview::GetElementChildVisual(element);
		return winrt::float3_to_string(visual.Scale());
	}

	void VisualAttach::SetScaleForElement(winrt::hstring const& value, winrt::UIElement const& element)
	{
		auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
		visual.Scale(winrt::string_to_float3(value));
	}
}
