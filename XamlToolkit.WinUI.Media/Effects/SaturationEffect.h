// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "SaturationEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import std;
import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	/// <summary>
	/// A saturation effect
	/// </summary>
	/// <remarks>This effect maps to the Win2D SaturationEffect effect</remarks>
	struct SaturationEffect : SaturationEffectT<SaturationEffect, implementation::PipelineEffect>
	{
		SaturationEffect() = default;

		/// <summary>
		/// Gets or sets the saturation amount to apply to the background (defaults to 1, should be in the [0, 1] range).
		/// </summary>
		double Value() const noexcept { return _value; }

		void Value(double value)
		{
			_value = std::clamp(value, 0.0, 1.0);
		}

		wil::single_threaded_rw_property<winrt::hstring> Id;

		Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
		{
			if (IsAnimatable())
			{
				return builder.Saturation(static_cast<float>(Value()), Id);
			}
			return builder.Saturation(static_cast<float>(Value()));
		}

	private:
		double _value{ 1 };
	};
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
	struct SaturationEffect : SaturationEffectT<SaturationEffect, implementation::SaturationEffect>
	{};
}