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
		return _source;
	}

	void TransitionHelper::Source(winrt::FrameworkElement const& value)
	{
		if (_source == value)
		{
			return;
		}

		bool needReset = IsAnimating() || IsTargetState();
		if (IsAnimating() && _source)
		{
			Stop();
			RestoreAnimatedElements(SourceAnimatedElements());
		}

		_currentAnimationGroupController.reset();
		_source = value;
		_sourceZIndex = value ? winrt::Canvas::GetZIndex(value) : -1;
		_sourceAnimatedElements.reset();
		if (needReset)
		{
			Reset(true);
		}
	}

	winrt::FrameworkElement TransitionHelper::Target() const
	{
		return _target;
	}

	void TransitionHelper::Target(winrt::FrameworkElement const& value)
	{
		if (_target == value)
		{
			return;
		}

		bool needReset = IsAnimating() || IsTargetState();
		if (IsAnimating() && _target)
		{
			Stop();
			RestoreAnimatedElements(TargetAnimatedElements());
		}

		_currentAnimationGroupController.reset();
		_target = value;
		_targetZIndex = value ? winrt::Canvas::GetZIndex(value) : -1;
		_targetAnimatedElements.reset();

		if (needReset)
		{
			Reset(true);
		}
	}
}
