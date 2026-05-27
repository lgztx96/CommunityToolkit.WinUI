#pragma once

#include "KeyFrame.h"
#include "../Interfaces/IKeyFrame{T}.h"
#include "../Builders/Interfaces/INormalizedKeyFrameAnimationBuilder{T}.h"
#include "../Extensions/AnimationExtensions.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// C++ counterpart for C# KeyFrame<TValue, TKeyFrame>.
    /// WinRT IDL doesn't support generics, so this is a C++-only layering helper.
    /// </summary>
    template<typename TValue, typename TKeyFrame>
    struct KeyFrameBase : KeyFrame, IKeyFrame<TKeyFrame>
    {
        KeyFrameBase() = default;

    public:
        /// <inheritdoc/>
        Animations::INormalizedKeyFrameAnimationBuilder<TKeyFrame>& AppendToBuilder(
            Animations::INormalizedKeyFrameAnimationBuilder<TKeyFrame>& builder) override
        {
            auto keyFrameEasingType = this->EasingType();
            auto keyFrameEasingMode = this->EasingMode();
            enum EasingType itemEasingType = keyFrameEasingType ? keyFrameEasingType.Value() : Animations::AnimationExtensions::DefaultEasingType();
            enum EasingMode itemEasingMode = keyFrameEasingMode ? keyFrameEasingMode.Value() : Animations::AnimationExtensions::DefaultEasingMode();

            if (auto expression = this->Expression(); !expression.empty())
            {
                return builder.ExpressionKeyFrame(this->Key(), expression, itemEasingType, itemEasingMode);
            }

            if (auto parsedValue = GetParsedValue())
            {
                return builder.KeyFrame(this->Key(), parsedValue.value(), itemEasingType, itemEasingMode);
            }

            return builder;
        }

    protected:
        /// <summary>
        /// Gets the parsed <typeparamref name="TKeyFrame"/> values for <see cref="Value"/>.
        /// </summary>
        /// <returns>The parsed keyframe values as <typeparamref name="TKeyFrame"/>.</returns>
        virtual std::optional<TKeyFrame> GetParsedValue() const = 0;
    };
}
