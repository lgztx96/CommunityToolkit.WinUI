#include "pch.h"
#include "winrt_module_imports.h"
#include "TransitionHelper.h"

namespace winrt
{
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	winrt::FrameworkElement TransitionHelper::Source() const
	{
		return _source.get();
	}

	void TransitionHelper::Source(winrt::FrameworkElement const& value)
	{
		if (_source.get() == value)
		{
			return;
		}

		bool needReset = IsAnimating() || IsTargetState();
		if (IsAnimating() && _source.get())
		{
			Stop();
			RestoreAnimatedElements(SourceAnimatedElements());
		}

		_currentAnimationGroupController.reset();
		_source = value;
		_sourceZIndex = value ? winrt::Canvas::GetZIndex(value) : -1;
		_sourceAnimatedElements.reset();
		_resolvedSourceAnimatedElements.reset();
		if (needReset)
		{
			Reset(true);
		}
	}

	winrt::FrameworkElement TransitionHelper::Target() const
	{
		return _target.get();
	}

	void TransitionHelper::Target(winrt::FrameworkElement const& value)
	{
		if (_target.get() == value)
		{
			return;
		}

		bool needReset = IsAnimating() || IsTargetState();
		if (IsAnimating() && _target.get())
		{
			Stop();
			RestoreAnimatedElements(TargetAnimatedElements());
		}

		_currentAnimationGroupController.reset();
		_target = value;
		_targetZIndex = value ? winrt::Canvas::GetZIndex(value) : -1;
		_targetAnimatedElements.reset();
		_resolvedTargetAnimatedElements.reset();

		if (needReset)
		{
			Reset(true);
		}
	}
}
