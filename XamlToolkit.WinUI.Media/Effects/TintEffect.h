// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "TintEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Windows.UI;
import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	/// <summary>
	/// A tint effect
	/// </summary>
	/// <remarks>This effect maps to the Win2D TintEffect effect</remarks>
	struct TintEffect : TintEffectT<TintEffect, implementation::PipelineEffect>
	{
		TintEffect() = default;

		/// <summary>
		/// Gets or sets the int color to use
		/// </summary>
		wil::single_threaded_rw_property<winrt::Windows::UI::Color> Color;

		wil::single_threaded_rw_property<winrt::hstring> Id;

		/// <inheritdoc/>
		Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
		{
			if (IsAnimatable())
			{
				return builder.Tint(Color(), Id);
			}
			return builder.Tint(Color());
		}
	};
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
	struct TintEffect : TintEffectT<TintEffect, implementation::TintEffect>
	{};
}