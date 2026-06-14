#pragma once
#include "../Extensions/AnimationExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#endif

import winrt.Microsoft.UI.Xaml.Media.Animation;

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
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
        static EasingFunctionBase ToEasingFunction(EasingType easingType, EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            switch (easingType)
            {
            case EasingType::Linear:
                return nullptr;

            case EasingType::Default:
                if (easingMode == EasingMode::EaseIn)
                {
                    ExponentialEase ease;
                    ease.Exponent(4.5);
                    ease.EasingMode(EasingMode::EaseIn);
                    return ease;
                }
                else if (easingMode == EasingMode::EaseOut)
                {
                    ExponentialEase ease;
                    ease.Exponent(7);
                    ease.EasingMode(EasingMode::EaseOut);
                    return ease;
                }
                else if (easingMode == EasingMode::EaseInOut)
                {
                    CircleEase ease;
                    ease.EasingMode(EasingMode::EaseInOut);
                    return ease;
                }

                throw winrt::hresult_invalid_argument(L"Invalid easing type");

            case EasingType::Cubic:
            {
                CubicEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Back:
            {
                BackEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Bounce:
            {
                BounceEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Elastic:
            {
                ElasticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Circle:
            {
                CircleEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Quadratic:
            {
                QuadraticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Quartic:
            {
                QuarticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Quintic:
            {
                QuinticEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            case EasingType::Sine:
            {
                SineEase ease;
                ease.EasingMode(easingMode);
                return ease;
            }

            default:
                throw winrt::hresult_invalid_argument(L"Invalid easing type");
            }
        }
    };
}
