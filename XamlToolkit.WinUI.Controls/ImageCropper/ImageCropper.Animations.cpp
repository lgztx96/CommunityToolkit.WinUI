#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <chrono>
#endif
#include "ImageCropper.h"

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Hosting;
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	void ImageCropper::AnimateUIElementOffset(winrt::Point to, winrt::TimeSpan duration, winrt::UIElement const& target)
	{
		auto targetVisual = winrt::ElementCompositionPreview::GetElementVisual(target);
		auto compositor = targetVisual.Compositor();
		auto linear = compositor.CreateLinearEasingFunction();
		auto offsetAnimation = compositor.CreateVector3KeyFrameAnimation();
		offsetAnimation.Duration(duration);
		offsetAnimation.Target(L"Offset");
		offsetAnimation.InsertKeyFrame(1.0f, winrt::float3(to.X, to.Y, 0), linear);
		targetVisual.StartAnimation(L"Offset", offsetAnimation);
	}

	void ImageCropper::AnimateUIElementScale(double to, winrt::TimeSpan duration, winrt::UIElement const& target)
	{
		auto targetVisual = winrt::ElementCompositionPreview::GetElementVisual(target);
		auto compositor = targetVisual.Compositor();
		auto linear = compositor.CreateLinearEasingFunction();
		auto scaleAnimation = compositor.CreateVector3KeyFrameAnimation();
		scaleAnimation.Duration(duration);
		scaleAnimation.Target(L"Scale");
		scaleAnimation.InsertKeyFrame(1.0f, winrt::float3(static_cast<float>(to)), linear);
		targetVisual.StartAnimation(L"Scale", scaleAnimation);
	}

	winrt::DoubleAnimation ImageCropper::CreateDoubleAnimation(double to, winrt::TimeSpan duration, winrt::DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation)
	{
		winrt::DoubleAnimation animation;
		animation.To(to);
		animation.Duration({ duration, winrt::DurationType::TimeSpan });
		animation.EnableDependentAnimation(enableDependentAnimation);

		winrt::Storyboard::SetTarget(animation, target);
		winrt::Storyboard::SetTargetProperty(animation, propertyName);

		return animation;
	}

	winrt::PointAnimation ImageCropper::CreatePointAnimation(winrt::Point to, winrt::TimeSpan duration, winrt::DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation)
	{
		winrt::PointAnimation animation;
		animation.To(to);
		animation.Duration({ duration, winrt::DurationType::TimeSpan });
		animation.EnableDependentAnimation(enableDependentAnimation);

		winrt::Storyboard::SetTarget(animation, target);
		winrt::Storyboard::SetTargetProperty(animation, propertyName);

		return animation;
	}

	winrt::ObjectAnimationUsingKeyFrames ImageCropper::CreateRectangleAnimation(winrt::Rect to, winrt::TimeSpan duration, winrt::RectangleGeometry rectangle, bool enableDependentAnimation)
	{
		winrt::ObjectAnimationUsingKeyFrames animation;
		animation.Duration({ duration, winrt::DurationType::TimeSpan });
		animation.EnableDependentAnimation(enableDependentAnimation);

		auto frames = GetRectKeyframes(rectangle.Rect(), to, duration);
		for (const auto& item : frames)
		{
			animation.KeyFrames().Append(item);
		}

		winrt::Storyboard::SetTarget(animation, rectangle);
		winrt::Storyboard::SetTargetProperty(animation, L"Rect");

		return animation;
	}

	std::vector<winrt::DiscreteObjectKeyFrame> ImageCropper::GetRectKeyframes(winrt::Rect from, winrt::Rect to, winrt::TimeSpan duration)
	{
		std::vector<winrt::DiscreteObjectKeyFrame> rectKeyframes;
		auto step = 10ms;
		auto startPointFrom = winrt::Point(from.X, from.Y);
		auto endPointFrom = winrt::Point(from.X + from.Width, from.Y + from.Height);
		auto startPointTo = winrt::Point(to.X, to.Y);
		auto endPointTo = winrt::Point(to.X + to.Width, to.Y + to.Height);
		for (auto time = 0ms; time < duration; time += step)
		{
			auto progress = std::chrono::duration<float>(time) / std::chrono::duration<float>(duration);

			winrt::Point startPoint;
			startPoint.X = startPointFrom.X + (progress * (startPointTo.X - startPointFrom.X));
			startPoint.Y = startPointFrom.Y + (progress * (startPointTo.Y - startPointFrom.Y));

			winrt::Point endPoint;
			endPoint.X = endPointFrom.X + (progress * (endPointTo.X - endPointFrom.X));
			endPoint.Y = endPointFrom.Y + (progress * (endPointTo.Y - endPointFrom.Y));

			winrt::DiscreteObjectKeyFrame frame;
			frame.KeyTime(winrt::KeyTime(time));
			frame.Value(winrt::box_value(ToRect(startPoint, endPoint)));
			rectKeyframes.emplace_back(frame);
		}

		winrt::DiscreteObjectKeyFrame frame;
		frame.KeyTime(winrt::KeyTime(duration));
		frame.Value(winrt::box_value(to));
		rectKeyframes.emplace_back(frame);
		return rectKeyframes;
	}
}
