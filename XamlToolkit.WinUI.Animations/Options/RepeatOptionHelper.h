// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <tuple>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Media::Animation;
	using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// Helper functions for RepeatOption.
    /// </summary>
    struct RepeatOptionHelper
    {
        /// <summary>
        /// Gets a RepeatOption value representing a single iteration.
        /// </summary>
        static RepeatOption Once() { return RepeatOption{ 1 }; }

        /// <summary>
        /// Gets a RepeatOption value indicating an animation that repeats forever.
        /// </summary>
        static RepeatOption Forever() { return RepeatOption{ -1 }; }

        /// <summary>
        /// Creates a RepeatOption value with the specified number of iterations.
        /// </summary>
        static RepeatOption Count(int count)
        {
            if (count < 0)
            {
                throw winrt::hresult_invalid_argument(L"The parameter \"count\" must be greater than or equal to 0.");
            }
            return RepeatOption{ count };
        }

        /// <summary>
        /// Gets a RepeatBehavior value corresponding to the current RepeatOption value.
        /// </summary>
        static RepeatBehavior ToRepeatBehavior(RepeatOption const& option)
        {
            if (option.Value < 0)
            {
                return RepeatBehaviorHelper::Forever();
            }
            return RepeatBehaviorHelper::FromCount(option.Value);
        }

        /// <summary>
        /// Gets the AnimationIterationBehavior and count values matching the current RepeatOption value.
        /// </summary>
        static std::tuple<AnimationIterationBehavior, int> ToBehaviorAndCount(RepeatOption const& option)
        {
            if (option.Value < 0)
            {
                return std::make_tuple(AnimationIterationBehavior::Forever, 1);
            }
            return std::make_tuple(AnimationIterationBehavior::Count, option.Value);
        }
    };
}
