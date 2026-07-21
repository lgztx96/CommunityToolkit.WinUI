// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "CrossFadeEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Windows.Foundation.Collections;
import winrt.Microsoft.UI.Composition;
import winrt.XamlToolkit.WinUI.Media;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt
{
	using namespace winrt::Windows::Foundation::Collections;
	using namespace winrt::Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	/// <summary>
	/// A blend effect that merges the current builder with an input one
	/// </summary>
	/// <remarks>This effect maps to the Win2D CrossFadeEffect effect</remarks>
	struct CrossFadeEffect : CrossFadeEffectT<CrossFadeEffect, implementation::PipelineEffect>
	{
		CrossFadeEffect() = default;

		/// <summary>
		/// Gets or sets the input to merge with the current instance (defaults to a <see cref="BackdropSourceExtension"/> with <see cref="Windows.UI.Xaml.Media.AcrylicBackgroundSource.Backdrop"/> source).
		/// </summary>
		wil::single_threaded_rw_property<Pipelines::PipelineBuilder> Source{ nullptr };

		/// <summary>
		/// Gets or sets the effects to apply to the input to merge with the current instance
		/// </summary>
		wil::single_threaded_rw_property<winrt::IVector<IPipelineEffect>> Effects = winrt::single_threaded_vector<IPipelineEffect>();

		/// <summary>
		/// Gets or sets the The cross fade factor to blend the input effects (default to 0.5, should be in the [0, 1] range)
		/// </summary>
		double Factor() const { return _factor; }
		void Factor(double value) { _factor = std::clamp(value, 0.0, 1.0); }

		wil::single_threaded_rw_property<winrt::hstring> Id;

		/// <inheritdoc/>
		Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
		{
			// Get or create source pipeline
			auto sourcePipeline = Source();
			if (!sourcePipeline)
			{
				sourcePipeline = Pipelines::PipelineBuilder::FromBackdrop();
			}

			// Apply effects
			for (const auto& effect : Effects())
			{
				sourcePipeline = effect.AppendToBuilder(sourcePipeline);
			}

			if (IsAnimatable())
			{
				return builder.CrossFade(sourcePipeline, static_cast<float>(Factor()), Id);
			}

			return builder.CrossFade(sourcePipeline, static_cast<float>(Factor()));
		}

		void NotifyCompositionBrushInUse(winrt::CompositionBrush const& brush) override
		{
			PipelineEffect::NotifyCompositionBrushInUse(brush);

			for (const auto& effect : Effects())
			{
				effect.NotifyCompositionBrushInUse(brush);
			}
		}

	private:
		double _factor{ 0.5 };
	};
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
	struct CrossFadeEffect : CrossFadeEffectT<CrossFadeEffect, implementation::CrossFadeEffect>
	{};
}