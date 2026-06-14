// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "HueRotationEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	/// <summary>
	/// A hue rotation effect
	/// </summary>
	/// <remarks>This effect maps to the Win2D HueRotationEffect effect</remarks>
	struct HueRotationEffect : HueRotationEffectT<HueRotationEffect, implementation::PipelineEffect>
	{
		HueRotationEffect() = default;

		/// <summary>
		/// Gets or sets the angle to rotate the hue, in radians
		/// </summary>
		wil::single_threaded_rw_property<double> Angle { 0.0 };

		wil::single_threaded_rw_property<winrt::hstring> Id;

		Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
		{
			if (IsAnimatable())
			{
				return builder.HueRotation(static_cast<float>(Angle()), Id);
			}
			return builder.HueRotation(static_cast<float>(Angle()));
		}
	};
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
	struct HueRotationEffect : HueRotationEffectT<HueRotationEffect, implementation::HueRotationEffect>
	{};
}