// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "SepiaEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import std;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	/// <summary>
	/// A sepia effect
	/// </summary>
	/// <remarks>This effect maps to the Win2D SepiaEffect effect</remarks>
	struct SepiaEffect : SepiaEffectT<SepiaEffect, implementation::PipelineEffect>
	{
		SepiaEffect() = default;

		/// <summary>
		/// Gets or sets the intensity of the effect (defaults to 0.5, should be in the [0, 1] range).
		/// </summary>
		double Intensity() const noexcept
		{
			return _intensity;
		}

		void Intensity(double value)
		{
			_intensity = std::clamp(value, 0.0, 1.0);
		}

		wil::single_threaded_rw_property<winrt::hstring> Id;

		Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
		{
			if (IsAnimatable())
			{
				return builder.Sepia(static_cast<float>(Intensity()), Id);
			}
			return builder.Sepia(static_cast<float>(Intensity()));
		}
	private:
		double _intensity{ 0.5 };
	};
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
	struct SepiaEffect : SepiaEffectT<SepiaEffect, implementation::SepiaEffect>
	{};
}