#include "pch.h"
#include "winrt_module_imports.h"
#include "TransitionHelper.h"
#include <queue>

namespace winrt
{
	using namespace ::winrt::Microsoft::UI::Xaml::Media;
	using namespace ::winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	namespace
	{
		struct AnimatedElementComparer
		{
			bool operator()(winrt::DependencyObject const& x, winrt::DependencyObject const& y) const
			{
				if (!x || !y) return false;
				if (TransitionHelper::GetIsIndependent(x) || TransitionHelper::GetIsIndependent(y)) return false;
				auto xId = TransitionHelper::GetId(x);
				auto yId = TransitionHelper::GetId(y);
				return !xId.empty() && !yId.empty() && xId == yId;
			}
			size_t operator()(winrt::DependencyObject const&) const { return 0; }
		};

		bool IsNotVisible(winrt::DependencyObject const& element)
		{
			auto target = element.try_as<winrt::UIElement>();
			if (!target || target.Visibility() == winrt::Visibility::Collapsed || target.Opacity() < TransitionHelper::AlmostZero)
			{
				return true;
			}

			return false;
		}

		bool IsAnimatedElement(winrt::DependencyObject const& element)
		{
			return !TransitionHelper::GetId(element).empty()
				|| !TransitionHelper::GetCoordinatedTarget(element).empty()
				|| TransitionHelper::GetIsIndependent(element);
		}

		void FindDescendantsWithBFSAndPruneAndPredicate(
			winrt::DependencyObject const& element,
			std::vector<winrt::UIElement>& result)
		{
			std::queue<winrt::DependencyObject> searchQueue;
			auto childrenCount = winrt::VisualTreeHelper::GetChildrenCount(element);
			for (int i = 0; i < childrenCount; ++i)
			{
				auto child = winrt::VisualTreeHelper::GetChild(element, i);
				if (IsAnimatedElement(child)) result.push_back(child.as<winrt::UIElement>());
				else if (!IsNotVisible(child)) searchQueue.push(child);
			}
			while (!searchQueue.empty())
			{
				auto parent = searchQueue.front(); searchQueue.pop();
				auto count = winrt::VisualTreeHelper::GetChildrenCount(parent);
				for (int j = 0; j < count; ++j)
				{
					auto child = winrt::VisualTreeHelper::GetChild(parent, j);
					if (IsAnimatedElement(child)) result.push_back(child.as<winrt::UIElement>());
					else if (!IsNotVisible(child)) searchQueue.push(child);
				}
			}
		}
	}

	TransitionHelper::AnimatedElements<winrt::UIElement> TransitionHelper::GetAnimatedElements(winrt::DependencyObject const& parent)
	{
		AnimatedElements<winrt::UIElement> result;
		if (!parent) return result;

		std::vector<winrt::UIElement> allAnimatedElements;
		FindDescendantsWithBFSAndPruneAndPredicate(parent, allAnimatedElements);

		AnimatedElementComparer comparer;
		std::vector<winrt::UIElement> distinctElements;
		for (auto& element : allAnimatedElements)
		{
			bool isDuplicate = false;
			for (const auto& existing : distinctElements)
			{
				if (comparer(existing, element))
				{
					isDuplicate = true;
					break;
				}
			}
			if (!isDuplicate) { distinctElements.push_back(element); }
		}

		for (const auto& element : distinctElements)
		{
			if (auto id = GetId(element); !id.empty())
			{
				result.ConnectedElements.insert_or_assign(id, element);
			}
			else if (auto ct = GetCoordinatedTarget(element); !ct.empty())
			{
				result.CoordinatedElements[ct].push_back(element);
			}
			else
			{
				result.IndependentElements.push_back(element);
			}
		}

		return result;
	}

	void TransitionHelper::ToggleVisualState(winrt::UIElement const& element, VisualStateToggleMethod method, bool visible)
	{
		if (!element) return;

		if (method == VisualStateToggleMethod::ByVisibility)
		{
			element.Visibility(visible ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
		else
		{
			winrt::ElementCompositionPreview::GetElementVisual(element).IsVisible(visible);
		}

		element.IsHitTestVisible(visible);
	}

	void TransitionHelper::RestoreAnimatedElements(AnimatedElements<winrt::UIElement> const& elements)
	{
		for (const auto& [id, element] : elements.ConnectedElements)
		{
			RestoreElement(element);
		}

		for (const auto& element : elements.IndependentElements)
		{
			RestoreElement(element);
		}

		for (const auto& [id, coords] : elements.CoordinatedElements)
		{
			for (const auto& element : coords)
			{
				RestoreElement(element);
			}
		}
	}

	void TransitionHelper::RestoreElement(winrt::UIElement const& element)
	{
		winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
		auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
		visual.StopAnimation(L"Opacity");
		visual.StopAnimation(L"Translation.XY");
		visual.StopAnimation(L"Scale.XY");
		if (auto clip = visual.Clip().try_as<winrt::InsetClip>())
		{
			clip.StopAnimation(L"LeftInset"); clip.StopAnimation(L"TopInset");
			clip.StopAnimation(L"RightInset"); clip.StopAnimation(L"BottomInset");
		}
		visual.Opacity(1);
		visual.Scale({ 1, 1, 1 });
		visual.Clip(nullptr);
		visual.Properties().InsertVector3(L"Translation", {});
	}

	void TransitionHelper::IsNotNullAndIsInVisualTree(winrt::FrameworkElement const& target, std::wstring_view name)
	{
		if (!target)
		{
			throw winrt::hresult_invalid_argument(winrt::format(L"{} must not be null.", name));
		}

		if (!winrt::VisualTreeHelper::GetParent(target))
		{
			throw winrt::hresult_invalid_argument(winrt::format(L"The {} element is not in the visual tree.", name));
		}
	}

	winrt::IAsyncAction TransitionHelper::UpdateControlLayout(winrt::FrameworkElement const& target)
	{
		wil::shared_event evt(wil::EventOptions::ManualReset);
		winrt::event_token tok = target.LayoutUpdated([&](auto&&, auto&&) { target.LayoutUpdated(tok); evt.SetEvent(); });
		target.UpdateLayout();
		co_await winrt::resume_on_signal(evt.get());
	}

	winrt::float2 TransitionHelper::GetInverseScale(winrt::float2 const& scale)
	{
		return winrt::float2{ 1.f / std::max<float>(scale.x, 0.0001f), 1.f / std::max<float>(scale.y, 0.0001f) };
	}

	winrt::float4 TransitionHelper::GetFixedThickness(float left, float top, float right, float bottom, float defaultValue)
	{
		return winrt::float4
		{
			left < AlmostZero ? defaultValue : left,
			top < AlmostZero ? defaultValue : top,
			right < AlmostZero ? defaultValue : right,
			bottom < AlmostZero ? defaultValue : bottom
		};
	}

	winrt::Rect TransitionHelper::GetTransformedBounds(
		winrt::float2 initialLocation, winrt::float2 initialSize,
		winrt::float2 centerPoint, winrt::float2 targetScale)
	{
		auto tl = winrt::float2
		{
			initialLocation.x + (1.f - targetScale.x) * centerPoint.x,
			initialLocation.y + (1.f - targetScale.y) * centerPoint.y
		};
		return winrt::Rect{ tl.x, tl.y, initialSize.x * targetScale.x, initialSize.y * targetScale.y };
	}

	std::optional<winrt::float4> TransitionHelper::GetTargetClip(
		winrt::float2 const& initialLocation, winrt::float2 const& initialSize,
		winrt::float2 const& centerPoint, winrt::float2 const& targetScale,
		winrt::float2 const& translation, winrt::Rect const& targetParentBounds)
	{
		auto transformedBounds = GetTransformedBounds(initialLocation + translation, initialSize, centerPoint, targetScale);

		auto transformedRight = winrt::RectHelper::GetRight(transformedBounds);
		auto transformedBottom = winrt::RectHelper::GetBottom(transformedBounds);
		auto targetRight = winrt::RectHelper::GetRight(targetParentBounds);
		auto targetBottom = winrt::RectHelper::GetBottom(targetParentBounds);

		if (winrt::RectHelper::Contains(targetParentBounds, { transformedBounds.X, transformedBounds.Y })
			&& winrt::RectHelper::Contains(targetParentBounds, { transformedRight, transformedBottom }))
		{
			return std::nullopt;
		}

		auto inverseScale = GetInverseScale(targetScale);

		return GetFixedThickness(
			(targetParentBounds.X - transformedBounds.X) * inverseScale.x,
			(targetParentBounds.Y - transformedBounds.Y) * inverseScale.y,
			(transformedRight - targetRight) * inverseScale.x,
			(transformedBottom - targetBottom) * inverseScale.y);
	}

	TransitionHelper::EasingFunctionFactory TransitionHelper::GetEasingFunctionFactory(
		winrt::XamlToolkit::WinUI::Animations::EasingType type, winrt::EasingMode mode, bool inverse)
	{
		return EasingFunctionFactory{ type, mode, inverse };
	}

	float TransitionHelper::GetOpacityTransitionStartKey(float normalizedKey, float halfTransitionNormalizedDuration)
	{
		return std::clamp(normalizedKey - halfTransitionNormalizedDuration, 0.f, 1.f);
	}

	float TransitionHelper::GetOpacityTransitionEndKey(float normalizedKey, float halfTransitionNormalizedDuration)
	{
		return std::clamp(normalizedKey + halfTransitionNormalizedDuration, 0.f, 1.f);
	}
}
