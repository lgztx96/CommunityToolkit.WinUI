#pragma once

#include "../Enums/Axis.h"
#include "../Enums/Side.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#else
import winrt.Microsoft.UI.Xaml.Media.Animation;
#endif

namespace winrt
{
    using namespace Windows::UI;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
	using namespace Microsoft::UI::Xaml::Media::Animation;
	using XamlColorAnimation = Microsoft::UI::Xaml::Media::Animation::ColorAnimation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
	/// <summary>
	/// An extension <see langword="class"/> for the <see cref="DependencyObject"/> type.
	/// </summary>
	class DependencyObjectExtensions
	{
	public:
		/// <summary>
		/// Prepares a <see cref="DoubleAnimation"/> with the given info.
		/// </summary>
		/// <param name="target">The target <see cref="DependencyObject"/> to animate.</param>
		/// <param name="property">The property to animate inside the target <see cref="DependencyObject"/>.</param>
		/// <param name="to">The final property value.</param>
		/// <param name="from">The optional initial property value.</param>
		/// <param name="delay">The optional delay for the animation.</param>
		/// <param name="duration">The duration of the <see cref="DoubleAnimation"/>.</param>
		/// <param name="easing">The easing function to use inside the <see cref="DoubleAnimation"/>.</param>
		/// <param name="repeatBehavior">The repeat behavior for the animation (defaults to one iteration).</param>
		/// <param name="fillBehavior">The behavior to use when the animation reaches the end of its schedule.</param>
		/// <param name="autoReverse">Indicates whether the animation plays in reverse after each forward iteration.</param>
		/// <param name="enableDependecyAnimations">Indicates whether or not to apply this animation to elements that need the visual tree to be rearranged.</param>
		/// <returns>A <see cref="DoubleAnimation"/> instance with the specified parameters.</returns>
		static winrt::DoubleAnimation CreateDoubleAnimation(
			winrt::DependencyObject const& target,
			winrt::hstring const& property,
			double to,
			std::optional<double> from,
			std::optional<winrt::TimeSpan> delay,
			winrt::TimeSpan duration,
			winrt::EasingFunctionBase const& easing,
			std::optional<winrt::RepeatBehavior> repeatBehavior = std::nullopt,
			winrt::FillBehavior fillBehavior = winrt::FillBehavior::HoldEnd,
			bool autoReverse = false,
			bool enableDependecyAnimations = false)
		{
			DoubleAnimation animation;

			animation.To(to);

			if (from.has_value())
			{
				animation.From(from.value());
			}

			if (delay.has_value())
			{
				animation.BeginTime(delay.value());
			}

			animation.Duration(winrt::DurationHelper::FromTimeSpan(duration));

			if (easing)
			{
				animation.EasingFunction(easing);
			}

			animation.RepeatBehavior(repeatBehavior.value_or(winrt::RepeatBehavior{ 1 }));
			animation.FillBehavior(fillBehavior);
			animation.AutoReverse(autoReverse);
			animation.EnableDependentAnimation(enableDependecyAnimations);

			winrt::Storyboard::SetTarget(animation, target);
			winrt::Storyboard::SetTargetProperty(animation, property);

			return animation;
		}

		/// <summary>
		/// Prepares a <see cref="PointAnimation"/> with the given info.
		/// </summary>
		/// <param name="target">The target <see cref="DependencyObject"/> to animate.</param>
		/// <param name="property">The property to animate inside the target <see cref="DependencyObject"/>.</param>
		/// <param name="to">The final property value.</param>
		/// <param name="from">The optional initial property value.</param>
		/// <param name="delay">The optional delay for the animation.</param>
		/// <param name="duration">The duration of the <see cref="PointAnimation"/>.</param>
		/// <param name="easing">The easing function to use inside the <see cref="PointAnimation"/>.</param>
		/// <param name="repeatBehavior">The repeat behavior for the animation (defaults to one iteration).</param>
		/// <param name="fillBehavior">The behavior to use when the animation reaches the end of its schedule.</param>
		/// <param name="autoReverse">Indicates whether the animation plays in reverse after each forward iteration.</param>
		/// <param name="enableDependecyAnimations">Indicates whether or not to apply this animation to elements that need the visual tree to be rearranged.</param>
		/// <returns>A <see cref="PointAnimation"/> instance with the specified parameters.</returns>
		static winrt::PointAnimation CreatePointAnimation(
			winrt::DependencyObject const& target,
			winrt::hstring const& property,
			winrt::Point to,
			std::optional<winrt::Point> from,
			std::optional<winrt::TimeSpan> delay,
			winrt::TimeSpan duration,
			winrt::EasingFunctionBase const& easing,
			std::optional<winrt::RepeatBehavior> repeatBehavior = std::nullopt,
			winrt::FillBehavior fillBehavior = winrt::FillBehavior::HoldEnd,
			bool autoReverse = false,
			bool enableDependecyAnimations = false)
		{
			winrt::PointAnimation animation;

			animation.To(to);

			if (from.has_value())
			{
				animation.From(from.value());
			}

			if (delay.has_value())
			{
				animation.BeginTime(delay.value());
			}

			animation.Duration(winrt::DurationHelper::FromTimeSpan(duration));

			if (easing)
			{
				animation.EasingFunction(easing);
			}

			animation.RepeatBehavior(repeatBehavior.value_or(winrt::RepeatBehavior{ 1 }));
			animation.FillBehavior(fillBehavior);
			animation.AutoReverse(autoReverse);
			animation.EnableDependentAnimation(enableDependecyAnimations);

			winrt::Storyboard::SetTarget(animation, target);
			winrt::Storyboard::SetTargetProperty(animation, property);

			return animation;
		}

		/// <summary>
		/// Prepares a <see cref="XamlColorAnimation"/> with the given info.
		/// </summary>
		/// <param name="target">The target <see cref="DependencyObject"/> to animate.</param>
		/// <param name="property">The property to animate inside the target <see cref="DependencyObject"/>.</param>
		/// <param name="to">The final property value.</param>
		/// <param name="from">The optional initial property value.</param>
		/// <param name="delay">The optional delay for the animation.</param>
		/// <param name="duration">The duration of the <see cref="XamlColorAnimation"/>.</param>
		/// <param name="easing">The easing function to use inside the <see cref="XamlColorAnimation"/>.</param>
		/// <param name="repeatBehavior">The repeat behavior for the animation (defaults to one iteration).</param>
		/// <param name="fillBehavior">The behavior to use when the animation reaches the end of its schedule.</param>
		/// <param name="autoReverse">Indicates whether the animation plays in reverse after each forward iteration.</param>
		/// <returns>A <see cref="XamlColorAnimation"/> instance with the specified parameters.</returns>
		static winrt::XamlColorAnimation CreateColorAnimation(
			winrt::DependencyObject const& target,
			winrt::hstring const& property,
			winrt::Color to,
			std::optional<winrt::Color> from,
			std::optional<winrt::TimeSpan> delay,
			winrt::TimeSpan duration,
			winrt::EasingFunctionBase const& easing,
			std::optional<winrt::RepeatBehavior> repeatBehavior = std::nullopt,
			winrt::FillBehavior fillBehavior = winrt::FillBehavior::HoldEnd,
			bool autoReverse = false)
		{
			winrt::XamlColorAnimation animation;

			animation.To(to);

			if (from.has_value())
			{
				animation.From(from.value());
			}

			if (delay.has_value())
			{
				animation.BeginTime(delay.value());
			}

			animation.Duration(winrt::DurationHelper::FromTimeSpan(duration));

			if (easing)
			{
				animation.EasingFunction(easing);
			}

			animation.RepeatBehavior(repeatBehavior.value_or(winrt::RepeatBehavior{ 1 }));
			animation.FillBehavior(fillBehavior);
			animation.AutoReverse(autoReverse);

			winrt::Storyboard::SetTarget(animation, target);
			winrt::Storyboard::SetTargetProperty(animation, property);

			return animation;
		}
	};
}
