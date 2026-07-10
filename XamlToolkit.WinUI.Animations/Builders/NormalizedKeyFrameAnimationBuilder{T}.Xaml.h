#pragma once

#include "NormalizedKeyFrameAnimationBuilder{T}.h"
#include "TimedKeyFrameAnimationBuilder{T}.Xaml.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// A custom <see cref="NormalizedKeyFrameAnimationBuilder{T}"/> class targeting the XAML layer.
    /// </summary>
    template<typename T>
    class NormalizedKeyFrameAnimationBuilderXaml : public NormalizedKeyFrameAnimationBuilder<T>, public IXamlAnimationFactory
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="NormalizedKeyFrameAnimationBuilderXaml"/> class.
        /// </summary>
        NormalizedKeyFrameAnimationBuilderXaml(
            winrt::hstring const& property,
            std::optional<winrt::TimeSpan> delay,
            winrt::TimeSpan duration,
            RepeatOption repeat)
            : NormalizedKeyFrameAnimationBuilder<T>(property, delay, duration, repeat)
        {
        }

        /// <inheritdoc/>
        INormalizedKeyFrameAnimationBuilder<T>& ExpressionKeyFrame(
            [[maybe_unused]] double progress,
            [[maybe_unused]] winrt::hstring const& expression,
            [[maybe_unused]] EasingType easingType = AnimationExtensions::DefaultEasingType(),
            [[maybe_unused]] winrt::EasingMode easingMode = AnimationExtensions::DefaultEasingMode()) override
        {
            throw winrt::hresult_error(E_FAIL, L"Expression keyframes can only be used on the composition layer");
        }

        /// <inheritdoc/>
        Timeline GetAnimation(winrt::DependencyObject const& targetHint) override
        {
            // Use the TimedKeyFrameAnimationBuilder implementation for XAML
            return BuildXamlKeyFrameAnimation<T, typename NormalizedKeyFrameAnimationBuilder<T>::KeyFrameInfo>(
                targetHint,
                this->property,
                this->delay,
                this->duration,
                this->repeat,
                this->keyFrames);
        }
    };
}
