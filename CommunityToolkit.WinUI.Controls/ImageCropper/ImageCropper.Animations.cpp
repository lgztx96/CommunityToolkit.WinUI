#include "pch.h"
#include "ImageCropper.h"
#include <winrt/Microsoft.UI.Xaml.Hosting.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Hosting;
using namespace Windows::Foundation::Numerics;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void ImageCropper::AnimateUIElementOffset(Point to, TimeSpan duration, UIElement const& target)
	{
		auto targetVisual = ElementCompositionPreview::GetElementVisual(target);
		auto compositor = targetVisual.Compositor();
		auto linear = compositor.CreateLinearEasingFunction();
		auto offsetAnimation = compositor.CreateVector3KeyFrameAnimation();
		offsetAnimation.Duration(duration);
		offsetAnimation.Target(L"Offset");
		offsetAnimation.InsertKeyFrame(1.0f, float3(to.X, to.Y, 0), linear);
		targetVisual.StartAnimation(L"Offset", offsetAnimation);
	}

	void ImageCropper::AnimateUIElementScale(double to, TimeSpan duration, UIElement const& target)
	{
		auto targetVisual = ElementCompositionPreview::GetElementVisual(target);
		auto compositor = targetVisual.Compositor();
		auto linear = compositor.CreateLinearEasingFunction();
		auto scaleAnimation = compositor.CreateVector3KeyFrameAnimation();
		scaleAnimation.Duration(duration);
		scaleAnimation.Target(L"Scale");
		scaleAnimation.InsertKeyFrame(1.0f, float3(static_cast<float>(to)), linear);
		targetVisual.StartAnimation(L"Scale", scaleAnimation);
	}

	DoubleAnimation ImageCropper::CreateDoubleAnimation(double to, TimeSpan duration, DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation)
	{
		auto animation = DoubleAnimation();
		animation.To(to);
		animation.Duration({ duration, DurationType::TimeSpan });
		animation.EnableDependentAnimation(enableDependentAnimation);

		Storyboard::SetTarget(animation, target);
		Storyboard::SetTargetProperty(animation, propertyName);

		return animation;
	}

	PointAnimation ImageCropper::CreatePointAnimation(Point to, TimeSpan duration, DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation)
	{
		auto animation = PointAnimation();
		animation.To(to);
		animation.Duration({ duration, DurationType::TimeSpan });
		animation.EnableDependentAnimation(enableDependentAnimation);

		Storyboard::SetTarget(animation, target);
		Storyboard::SetTargetProperty(animation, propertyName);

		return animation;
	}

	ObjectAnimationUsingKeyFrames ImageCropper::CreateRectangleAnimation(Rect to, TimeSpan duration, RectangleGeometry rectangle, bool enableDependentAnimation)
	{
		auto animation = ObjectAnimationUsingKeyFrames();
		animation.Duration({ duration, DurationType::TimeSpan });
		animation.EnableDependentAnimation(enableDependentAnimation);

		auto frames = GetRectKeyframes(rectangle.Rect(), to, duration);
		for (auto& item : frames)
		{
			animation.KeyFrames().Append(item);
		}

		Storyboard::SetTarget(animation, rectangle);
		Storyboard::SetTargetProperty(animation, L"Rect");

		return animation;
	}

	std::vector<DiscreteObjectKeyFrame> ImageCropper::GetRectKeyframes(Rect from, Rect to, TimeSpan duration)
	{
		auto rectKeyframes = std::vector<DiscreteObjectKeyFrame>();
		auto step = 10ms;
		auto startPointFrom = Point(from.X, from.Y);
		auto endPointFrom = Point(from.X + from.Width, from.Y + from.Height);
		auto startPointTo = Point(to.X, to.Y);
		auto endPointTo = Point(to.X + to.Width, to.Y + to.Height);
		for (auto time = 0ms; time < duration; time += step)
		{
			auto progress = std::chrono::duration<float>(time)
				/ std::chrono::duration<float>(duration);

			Point startPoint;
			startPoint.X = startPointFrom.X + (progress * (startPointTo.X - startPointFrom.X));
			startPoint.Y = startPointFrom.Y + (progress * (startPointTo.Y - startPointFrom.Y));

			Point endPoint;
			endPoint.X = endPointFrom.X + (progress * (endPointTo.X - endPointFrom.X));
			endPoint.Y = endPointFrom.Y + (progress * (endPointTo.Y - endPointFrom.Y));

			DiscreteObjectKeyFrame frame;
			frame.KeyTime(KeyTime(time));
			frame.Value(winrt::box_value(ToRect(startPoint, endPoint)));
			rectKeyframes.emplace_back(frame);
		}

		DiscreteObjectKeyFrame frame;
		frame.KeyTime(KeyTime(duration));
		frame.Value(winrt::box_value(to));
		rectKeyframes.emplace_back(frame);
		return rectKeyframes;
	}
}