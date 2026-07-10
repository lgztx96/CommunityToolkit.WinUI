#pragma once
#include "../Extensions/AnimationExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#else
import winrt.Microsoft.UI.Xaml.Media.Animation;
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An extension <see langword="class"/> for the <see cref="EasingType"/> type.
    /// </summary>
    class EasingTypeExtensions
    {
    public:
        /// <summary>
        /// Gets an <see cref="EasingFunctionBase"/> instance corresponding to a given <see cref="EasingType"/> value.
        /// </summary>
        /// <param name="easingType">The desired easing function type.</param>
        /// <param name="easingMode">The desired easing mode.</param>
        /// <returns>An <see cref="EasingFunctionBase"/> instance corresponding to the input parameters.</returns>
        static winrt::EasingFunctionBase ToEasingFunction(EasingType easingType, winrt::EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            switch (easingType)
            {
            case EasingType::Linear:
                return nullptr;

            case EasingType::Default:
                if (easingMode == EasingMode::EaseIn)
                {
                    winrt::ExponentialEase ease;
                    ease.Exponent(4.5);
                    ease.EasingMode(EasingMode::EaseIn);
                    return ease;
                }
                else if (easingMode == EasingMode::EaseOut)
                {
                    winrt::ExponentialEase ease;
                    ease.Exponent(7);
                    ease.EasingMode(EasingMode::EaseOut);
                    return ease;
                }
                else if (easingMode == EasingMode::EaseInOut)
                {
                    winrt::CircleEase ease;
                    ease.EasingMode(EasingMode::EaseInOut);
                    return ease;
                }
                throw winrt::hresult_invalid_argument(L"Invalid easing type");

            case EasingType::Cubic:
            {
                winrt::CubicEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Back:
            {
                winrt::BackEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Bounce:
            {
                winrt::BounceEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Elastic:
            {
                winrt::ElasticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Circle:
            {
                winrt::CircleEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Quadratic:
            {
                winrt::QuadraticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Quartic:
            {
                winrt::QuarticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Quintic:
            {
                winrt::QuinticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            case EasingType::Sine:
            {
                winrt::SineEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }
            default:
                throw winrt::hresult_invalid_argument(L"Invalid easing type");
            }
        }
    };
}
