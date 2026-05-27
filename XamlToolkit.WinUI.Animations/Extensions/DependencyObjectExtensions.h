#pragma once
#include "../Options/RepeatOptionHelper.h"
#include "../Enums/Axis.h"
#include "../Enums/Side.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Xaml::Media;
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
		static DoubleAnimation CreateDoubleAnimation(
			winrt::Microsoft::UI::Xaml::DependencyObject const& target,
			winrt::hstring const& property,
			double to,
			std::optional<double> from,
			std::optional<winrt::Windows::Foundation::TimeSpan> delay,
			winrt::Windows::Foundation::TimeSpan duration,
			EasingFunctionBase const& easing,
			std::optional<RepeatBehavior> repeatBehavior = std::nullopt,
			FillBehavior fillBehavior = FillBehavior::HoldEnd,
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

			animation.Duration(winrt::Microsoft::UI::Xaml::DurationHelper::FromTimeSpan(duration));

			if (easing)
			{
				animation.EasingFunction(easing);
			}

			animation.RepeatBehavior(repeatBehavior.value_or(RepeatBehavior{ 1 }));
			animation.FillBehavior(fillBehavior);
			animation.AutoReverse(autoReverse);
			animation.EnableDependentAnimation(enableDependecyAnimations);

			Storyboard::SetTarget(animation, target);
			Storyboard::SetTargetProperty(animation, property);

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
		static PointAnimation CreatePointAnimation(
			winrt::Microsoft::UI::Xaml::DependencyObject const& target,
			winrt::hstring const& property,
			winrt::Windows::Foundation::Point to,
			std::optional<winrt::Windows::Foundation::Point> from,
			std::optional<Windows::Foundation::TimeSpan> delay,
			winrt::Windows::Foundation::TimeSpan duration,
			EasingFunctionBase const& easing,
			std::optional<RepeatBehavior> repeatBehavior = std::nullopt,
			FillBehavior fillBehavior = FillBehavior::HoldEnd,
			bool autoReverse = false,
			bool enableDependecyAnimations = false)
		{
			PointAnimation animation;

			animation.To(to);

			if (from.has_value())
			{
				animation.From(from.value());
			}

			if (delay.has_value())
			{
				animation.BeginTime(delay.value());
			}

			animation.Duration(winrt::Microsoft::UI::Xaml::DurationHelper::FromTimeSpan(duration));

			if (easing)
			{
				animation.EasingFunction(easing);
			}

			animation.RepeatBehavior(repeatBehavior.value_or(RepeatBehavior{ 1 }));
			animation.FillBehavior(fillBehavior);
			animation.AutoReverse(autoReverse);
			animation.EnableDependentAnimation(enableDependecyAnimations);

			Storyboard::SetTarget(animation, target);
			Storyboard::SetTargetProperty(animation, property);

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
		static winrt::Microsoft::UI::Xaml::Media::Animation::ColorAnimation CreateColorAnimation(
			winrt::Microsoft::UI::Xaml::DependencyObject const& target,
			winrt::hstring const& property,
			winrt::Windows::UI::Color to,
			std::optional<winrt::Windows::UI::Color> from,
			std::optional<winrt::Windows::Foundation::TimeSpan> delay,
			winrt::Windows::Foundation::TimeSpan duration,
			EasingFunctionBase const& easing,
			std::optional<RepeatBehavior> repeatBehavior = std::nullopt,
			FillBehavior fillBehavior = FillBehavior::HoldEnd,
			bool autoReverse = false)
		{
			winrt::Microsoft::UI::Xaml::Media::Animation::ColorAnimation animation;

			animation.To(to);

			if (from.has_value())
			{
				animation.From(from.value());
			}

			if (delay.has_value())
			{
				animation.BeginTime(delay.value());
			}

			animation.Duration(winrt::Microsoft::UI::Xaml::DurationHelper::FromTimeSpan(duration));

			if (easing)
			{
				animation.EasingFunction(easing);
			}

			animation.RepeatBehavior(repeatBehavior.value_or(RepeatBehavior{ 1 }));
			animation.FillBehavior(fillBehavior);
			animation.AutoReverse(autoReverse);

			Storyboard::SetTarget(animation, target);
			Storyboard::SetTargetProperty(animation, property);

			return animation;
		}
	};
}
