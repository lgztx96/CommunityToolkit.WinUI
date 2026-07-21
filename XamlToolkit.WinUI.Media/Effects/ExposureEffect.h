// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "ExposureEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	/// <summary>
	/// An exposure effect
	/// </summary>
	/// <remarks>This effect maps to the Win2D ExposureEffect effect</remarks>
	struct ExposureEffect : ExposureEffectT<ExposureEffect, implementation::PipelineEffect>
	{
		ExposureEffect() = default;

		/// <summary>
		/// Gets or sets the amount of exposure to apply to the background (defaults to 0, should be in the [-2, 2] range).
		/// </summary>
		double Amount() const noexcept
		{
			return _amount;
		}

		void Amount(double value)
		{
			_amount = std::clamp(value, -2.0, 2.0);
		}

		wil::single_threaded_rw_property<winrt::hstring> Id;

		/// <inheritdoc/>
		Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
		{
			if (IsAnimatable())
			{
				return builder.Exposure(static_cast<float>(Amount()), Id);
			}
			return builder.Exposure(static_cast<float>(Amount()));
		}

	private:
		double _amount{ 0.0 };
	};
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
	struct ExposureEffect : ExposureEffectT<ExposureEffect, implementation::ExposureEffect>
	{};
}