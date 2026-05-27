#pragma once

#include "KeyFrame.g.h"

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A base model representing a keyframe that can be used in XAML.
    /// </summary>
    struct KeyFrame : KeyFrameT<KeyFrame>
    {
    public:
        KeyFrame() = default;

        /// <summary>
        /// Gets or sets the key time for the current keyframe. This is a normalized
        /// value in the [0, 1] range, relative to the total animation duration.
        /// </summary>
        double Key() const
        {
            return winrt::unbox_value<double>(GetValue(KeyProperty));
        }
        void Key(double value)
        {
            SetValue(KeyProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional expression for the keyframe.
        /// </summary>
        winrt::hstring Expression() const
        {
            return winrt::unbox_value_or<winrt::hstring>(GetValue(ExpressionProperty), L"");
        }
        void Expression(winrt::hstring const& value)
        {
            SetValue(ExpressionProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function type for the keyframe.
        /// </summary>
        winrt::Windows::Foundation::IReference<enum EasingType> EasingType() const
        {
            return GetValue(EasingTypeProperty).try_as<winrt::Windows::Foundation::IReference<enum EasingType>>();
        }
        void EasingType(winrt::Windows::Foundation::IReference<enum EasingType> const& value)
        {
            SetValue(EasingTypeProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Gets or sets the optional easing function mode for the keyframe.
        /// </summary>
        winrt::Windows::Foundation::IReference<enum EasingMode> EasingMode() const
        {
            return GetValue(EasingModeProperty).try_as<winrt::Windows::Foundation::IReference<enum EasingMode>>();
        }
        void EasingMode(winrt::Windows::Foundation::IReference<enum EasingMode> const& value)
        {
            SetValue(EasingModeProperty, winrt::box_value(value));
        }

        /// <summary>
        /// Dependency property for Key.
        /// </summary>
        static const wil::single_threaded_property<DependencyProperty> KeyProperty;

        /// <summary>
        /// Dependency property for Expression.
        /// </summary>
        static const wil::single_threaded_property<DependencyProperty> ExpressionProperty;

        /// <summary>
        /// Dependency property for EasingType.
        /// </summary>
        static const wil::single_threaded_property<DependencyProperty> EasingTypeProperty;

        /// <summary>
        /// Dependency property for EasingMode.
        /// </summary>
        static const wil::single_threaded_property<DependencyProperty> EasingModeProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct KeyFrame : KeyFrameT<KeyFrame, implementation::KeyFrame>
    {
    };
}

