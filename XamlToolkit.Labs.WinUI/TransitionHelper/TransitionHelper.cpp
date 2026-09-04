#include "pch.h"
#include "winrt_module_imports.h"
#include "TransitionHelper.h"
#include "TransitionConfig.h"
#if __has_include("TransitionHelper.g.cpp")
#include "TransitionHelper.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	winrt::XamlToolkit::Labs::WinUI::TransitionConfig TransitionHelper::DefaultConfig() const
	{
		auto config = winrt::make<implementation::TransitionConfig>();
		config.EasingMode(DefaultEasingMode());
		config.EasingType(DefaultEasingType());
		config.OpacityTransitionProgressKey(DefaultIndependentTranslation());
		return config;
	}

	winrt::IAsyncAction TransitionHelper::StartAsync()
	{
		return AnimateControlsAsync(false, false);
	}

	winrt::IAsyncAction TransitionHelper::StartAsync(bool forceUpdateAnimatedElements)
	{
		return AnimateControlsAsync(false, forceUpdateAnimatedElements);
	}

	winrt::IAsyncAction TransitionHelper::ReverseAsync()
	{
		return AnimateControlsAsync(true, false);
	}

	winrt::IAsyncAction TransitionHelper::ReverseAsync(bool forceUpdateAnimatedElements)
	{
		return AnimateControlsAsync(true, forceUpdateAnimatedElements);
	}

	void TransitionHelper::Stop()
	{
		if (!IsAnimating())
		{
			return;
		}

		if (_currentAnimationCancellationTokenSource.has_value())
		{
			_currentAnimationCancellationTokenSource->cancel();
			_currentAnimationCancellationTokenSource.reset();
		}
	}

	void TransitionHelper::Reset(bool toInitialState)
	{
		Stop();
		_currentAnimationGroupController.reset();
		RestoreState(!toInitialState);
	}
}
