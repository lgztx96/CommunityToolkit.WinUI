// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once
#include "RepeatOptionHelper.g.h"
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

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// Helper functions for RepeatOption.
    /// </summary>
    struct RepeatOptionHelper
    {
        /// <summary>
        /// Gets a RepeatOption value representing a single iteration.
        /// </summary>
        static RepeatOption Once() noexcept { return RepeatOption{ 1 }; }

        /// <summary>
        /// Gets a RepeatOption value indicating an animation that repeats forever.
        /// </summary>
        static RepeatOption Forever() noexcept { return RepeatOption{ -1 }; }

        /// <summary>
        /// Creates a RepeatOption value with the specified number of iterations.
        /// </summary>
        static RepeatOption Count(int count);

        /// <summary>
        /// Gets a RepeatBehavior value corresponding to the current RepeatOption value.
        /// </summary>
        static RepeatBehavior ToRepeatBehavior(RepeatOption const& option);

        /// <summary>
        /// Gets the AnimationIterationBehavior and count values matching the current RepeatOption value.
        /// </summary>
        static std::tuple<AnimationIterationBehavior, int> ToBehaviorAndCount(RepeatOption const& option);

        /// <summary>
        /// Parses a <see cref="RepeatOption"/> value from a <see cref="string"/>.
        /// The allowed values are either non-negative integers, or "Forever".
        /// </summary>
        /// <param name="text">The input text to parse.</param>
        /// <returns>The parsed <see cref="RepeatOption"/> value.</returns>
        static RepeatOption Parse(winrt::hstring const& text);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct RepeatOptionHelper : RepeatOptionHelperT<RepeatOptionHelper, implementation::RepeatOptionHelper>
    {
    };
}
