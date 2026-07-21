#pragma once

#include "TransitionConfig.g.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct TransitionConfig : TransitionConfigT<TransitionConfig>
	{
		TransitionConfig() = default;

		winrt::hstring Id() const
		{
			return m_id;
		}
		void Id(winrt::hstring const& value)
		{
			m_id = value;
		}
		ScaleMode ScaleMode() const
		{
			return m_scaleMode;
		}
		void ScaleMode(enum ScaleMode value)
		{
			m_scaleMode = value;
		}
		winrt::XamlToolkit::Labs::WinUI::IScalingCalculator CustomScalingCalculator() const
		{
			return m_calculator;
		}
		void CustomScalingCalculator(winrt::XamlToolkit::Labs::WinUI::IScalingCalculator const& value)
		{
			m_calculator = value;
		}
		bool EnableClipAnimation() const
		{
			return m_enableClip;
		}
		void EnableClipAnimation(bool value)
		{
			m_enableClip = value;
		}
		winrt::Windows::Foundation::Point NormalizedCenterPoint() const
		{
			return m_center;
		}
		void NormalizedCenterPoint(winrt::Windows::Foundation::Point value)
		{
			m_center = value;
		}
		winrt::Windows::Foundation::IReference<winrt::XamlToolkit::WinUI::Animations::EasingType> EasingType() const
		{
			return m_easingType;
		}
		void EasingType(winrt::Windows::Foundation::IReference<winrt::XamlToolkit::WinUI::Animations::EasingType> const& value)
		{
			m_easingType = value;
		}
		winrt::Windows::Foundation::IReference<winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode> EasingMode() const
		{
			return m_easingMode;
		}
		void EasingMode(winrt::Windows::Foundation::IReference<winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode> const& value)
		{
			m_easingMode = value;
		}
		winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::Point> OpacityTransitionProgressKey() const
		{
			return m_opacityKey;
		}
		void OpacityTransitionProgressKey(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::Point> const& value)
		{
			m_opacityKey = value;
		}

	private:
		winrt::hstring m_id;
		enum ScaleMode m_scaleMode
		{
			ScaleMode::None
		};
		winrt::XamlToolkit::Labs::WinUI::IScalingCalculator m_calculator{ nullptr };
		bool m_enableClip{};
		winrt::Windows::Foundation::Point m_center{};
		winrt::Windows::Foundation::IReference<winrt::XamlToolkit::WinUI::Animations::EasingType> m_easingType{ nullptr };
		winrt::Windows::Foundation::IReference<winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode> m_easingMode{ nullptr };
		winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::Point> m_opacityKey{ nullptr };
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct TransitionConfig : TransitionConfigT<TransitionConfig, implementation::TransitionConfig>
	{
	};
}
