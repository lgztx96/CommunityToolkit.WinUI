// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BlurEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	/// <summary>
	/// A gaussian blur effect
	/// </summary>
	/// <remarks>This effect maps to the Win2D GaussianBlurEffect effect</remarks>
	struct BlurEffect : BlurEffectT<BlurEffect, implementation::PipelineEffect>
	{
		BlurEffect() = default;

		/// <summary>
		/// Gets or sets the input to merge with the current instance (defaults to a <see cref="BackdropSourceExtension"/> with <see cref="Windows.UI.Xaml.Media.AcrylicBackgroundSource.Backdrop"/> source).
		/// </summary>
		wil::single_threaded_rw_property<Pipelines::PipelineBuilder> Source{ nullptr };

		/// <summary>
		/// Gets or sets the blur amount for the effect (must be a positive value)
		/// </summary>
		wil::single_threaded_rw_property<double> Amount{ 0.0 };

		wil::single_threaded_rw_property<winrt::hstring> Id;

		Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
		{
			if (IsAnimatable())
			{
				return builder.Blur(static_cast<float>(Amount()), Id);
			}
			return builder.Blur(static_cast<float>(Amount()));
		}
	};
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
	struct BlurEffect : BlurEffectT<BlurEffect, implementation::BlurEffect>
	{};
}