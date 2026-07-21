#pragma once

#include "../Enums/Axis.h"
#include "../Enums/Side.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/XamlToolkit.WinUI.Animations.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <chrono>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <cwchar>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
	using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
	/// <summary>
	/// Common properties related to extensions.
	/// </summary>
	struct AnimationExtensions
	{
		/// <summary>
		/// Gets the default delay of animations.
		/// </summary>
		static constexpr winrt::TimeSpan DefaultDelay() { return {}; }

		/// <summary>
		/// Gets the default duration of animations.
		/// </summary>
		static constexpr winrt::TimeSpan DefaultDuration() { return std::chrono::milliseconds(400); }

		/// <summary>
		/// The default <see cref="EasingType"/> value used for animations.
		/// </summary>
		static constexpr EasingType DefaultEasingType() { return EasingType::Default; }
		/// <summary>
		/// The default <see cref="EasingMode"/> value used for animations.
		/// </summary>
		static constexpr winrt::EasingMode DefaultEasingMode() { return winrt::EasingMode::EaseInOut; }
		/// <summary>
		/// The default <see cref="AnimationDelayBehavior"/> value used for animations (only applies to composition animations).
		/// </summary>
		static constexpr AnimationDelayBehavior DefaultDelayBehavior() { return AnimationDelayBehavior::SetInitialValueBeforeDelay; }
		/// <summary>
		/// The reusable mapping of control points for easing curves for combinations of <see cref="EasingType"/> and <see cref="EasingMode"/> values.
		/// </summary>

		using EasingKey = std::tuple<enum EasingType, winrt::EasingMode>;
		using EasingValue = std::tuple<winrt::float2, winrt::float2>;

		struct EasingKeyHash
		{
			size_t operator()(EasingKey const& key) const noexcept
			{
				auto const& [type, mode] = key;
				return (static_cast<size_t>(type) << 8) ^ static_cast<size_t>(mode);
			}
		};

		static inline const std::unordered_map<EasingKey, EasingValue, EasingKeyHash> EasingMaps =
		{
			// The default/inout combination is missing, as in this case we just skip creating
			// an easing function entirely, and rely on the composition APIs using the implicit
			// easing automatically. This is a bit more efficient, and results in the same
			// visual behavior anyway, as that's the standard combination for animations.
			// Default
			{ {EasingType::Default, winrt::EasingMode::EaseOut}, {{0.1f, 0.9f}, {0.2f, 1.0f}} },
			{ {EasingType::Default, winrt::EasingMode::EaseIn},  {{0.7f, 0.0f}, {1.0f, 0.5f}} },

			// Cubic
			{ {EasingType::Cubic, winrt::EasingMode::EaseOut},   {{0.215f, 0.61f}, {0.355f, 1.f}} },
			{ {EasingType::Cubic, winrt::EasingMode::EaseIn},    {{0.55f, 0.055f}, {0.675f, 0.19f}} },
			{ {EasingType::Cubic, winrt::EasingMode::EaseInOut}, {{0.645f, 0.045f}, {0.355f, 1.f}} },

			// Back
			{ {EasingType::Back, winrt::EasingMode::EaseOut},    {{0.175f, 0.885f}, {0.32f, 1.275f}} },
			{ {EasingType::Back, winrt::EasingMode::EaseIn},     {{0.6f, -0.28f}, {0.735f, 0.045f}} },
			{ {EasingType::Back, winrt::EasingMode::EaseInOut},  {{0.68f, -0.55f}, {0.265f, 1.55f}} },

			// Bounce
			{ {EasingType::Bounce, winrt::EasingMode::EaseOut},  {{0.58f, 1.93f}, {0.08f, 0.36f}} },
			{ {EasingType::Bounce, winrt::EasingMode::EaseIn},   {{0.93f, 0.7f}, {0.4f, -0.93f}} },
			{ {EasingType::Bounce, winrt::EasingMode::EaseInOut},{{0.65f, -0.85f}, {0.35f, 1.85f}} },

			// Elastic
			{ {EasingType::Elastic, winrt::EasingMode::EaseOut}, {{0.37f, 2.68f}, {0.f, 0.22f}} },
			{ {EasingType::Elastic, winrt::EasingMode::EaseIn},  {{1.f, 0.78f}, {0.63f, -1.68f}} },
			{ {EasingType::Elastic, winrt::EasingMode::EaseInOut},{{0.9f, -1.2f}, {0.1f, 2.2f}} },

			// Circle
			{ {EasingType::Circle, winrt::EasingMode::EaseOut},  {{0.075f, 0.82f}, {0.165f, 1.f}} },
			{ {EasingType::Circle, winrt::EasingMode::EaseIn},   {{0.6f, 0.04f}, {0.98f, 0.335f}} },
			{ {EasingType::Circle, winrt::EasingMode::EaseInOut},{{0.785f, 0.135f}, {0.15f, 0.86f}} },

			// Quadratic
			{ {EasingType::Quadratic, winrt::EasingMode::EaseOut},{{0.25f, 0.46f}, {0.45f, 0.94f}} },
			{ {EasingType::Quadratic, winrt::EasingMode::EaseIn}, {{0.55f, 0.085f}, {0.68f, 0.53f}} },
			{ {EasingType::Quadratic, winrt::EasingMode::EaseInOut},{{0.445f, 0.03f}, {0.515f, 0.955f}} },

			// Quartic
			{ {EasingType::Quartic, winrt::EasingMode::EaseOut}, {{0.165f, 0.84f}, {0.44f, 1.f}} },
			{ {EasingType::Quartic, winrt::EasingMode::EaseIn},  {{0.895f, 0.03f}, {0.685f, 0.22f}} },
			{ {EasingType::Quartic, winrt::EasingMode::EaseInOut},{{0.77f, 0.f}, {0.175f, 1.f}} },

			// Quintic
			{ {EasingType::Quintic, winrt::EasingMode::EaseOut}, {{0.23f, 1.f}, {0.32f, 1.f}} },
			{ {EasingType::Quintic, winrt::EasingMode::EaseIn},  {{0.755f, 0.05f}, {0.855f, 0.06f}} },
			{ {EasingType::Quintic, winrt::EasingMode::EaseInOut},{{0.86f, 0.f}, {0.07f, 1.f}} },

			// Sine
			{ {EasingType::Sine, winrt::EasingMode::EaseOut},    {{0.39f, 0.575f}, {0.565f, 1.f}} },
			{ {EasingType::Sine, winrt::EasingMode::EaseIn},     {{0.47f, 0.f}, {0.745f, 0.715f}} },
			{ {EasingType::Sine, winrt::EasingMode::EaseInOut},  {{0.445f, 0.05f}, {0.55f, 0.95f}} },
		};

		/// <summary>
		/// Parses a string to a Vector2.
		/// </summary>
		static winrt::float2 ToVector2(winrt::hstring const& value)
		{
			if (value.empty())
				return winrt::float2{ 0, 0 };

			std::wstring_view str(value);

			float x = 0.0f;
			float y = 0.0f;

			auto comma = str.find(L',');

			if (comma != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma + 1, nullptr);
			}
			else
			{
				x = wcstof(str.data(), nullptr);
				y = x;
			}

			return winrt::float2{ x, y };
		}

		/// <summary>
		/// Parses a string to a Vector3.
		/// </summary>
		static winrt::float3 ToVector3(winrt::hstring const& value)
		{
			if (value.empty())
				return winrt::float3{ 0, 0, 0 };

			std::wstring_view str{ value };

			float x = 0, y = 0, z = 0;

			auto comma1 = str.find(L',');
			auto comma2 = comma1 != std::wstring_view::npos
				? str.find(L',', comma1 + 1)
				: std::wstring_view::npos;

			if (comma1 != std::wstring_view::npos && comma2 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
				z = wcstof(str.data() + comma2 + 1, nullptr);
			}
			else if (comma1 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
			}
			else
			{
				x = wcstof(str.data(), nullptr);
				y = x;
				z = x;
			}

			return winrt::float3{ x, y, z };
		}

		/// <summary>
		/// Parses a string to a Vector4.
		/// </summary>
		static winrt::float4 ToVector4(winrt::hstring const& value)
		{
			if (value.empty())
				return winrt::float4{ 0, 0, 0, 0 };

			std::wstring_view str{ value };

			float x = 0, y = 0, z = 0, w = 0;

			auto comma1 = str.find(L',');
			auto comma2 = comma1 != std::wstring_view::npos
				? str.find(L',', comma1 + 1)
				: std::wstring_view::npos;
			auto comma3 = comma2 != std::wstring_view::npos
				? str.find(L',', comma2 + 1)
				: std::wstring_view::npos;

			if (comma3 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
				z = wcstof(str.data() + comma2 + 1, nullptr);
				w = wcstof(str.data() + comma3 + 1, nullptr);
			}
			else if (comma2 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
				z = wcstof(str.data() + comma2 + 1, nullptr);
			}
			else if (comma1 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
			}
			else
			{
				x = wcstof(str.data(), nullptr);
				y = x;
				z = x;
				w = x;
			}

			return winrt::float4{ x, y, z, w };
		}

		/// <summary>
		/// Parses a string to a Quaternion.
		/// </summary>
		static winrt::quaternion ToQuaternion(winrt::hstring const& value)
		{
			if (value.empty())
				return winrt::quaternion{ 0, 0, 0, 1 };

			std::wstring_view str{ value };

			float x = 0, y = 0, z = 0, w = 1;

			auto comma1 = str.find(L',');
			auto comma2 = comma1 != std::wstring_view::npos
				? str.find(L',', comma1 + 1)
				: std::wstring_view::npos;
			auto comma3 = comma2 != std::wstring_view::npos
				? str.find(L',', comma2 + 1)
				: std::wstring_view::npos;

			if (comma3 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
				z = wcstof(str.data() + comma2 + 1, nullptr);
				w = wcstof(str.data() + comma3 + 1, nullptr);
			}
			else if (comma2 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
				z = wcstof(str.data() + comma2 + 1, nullptr);
			}
			else if (comma1 != std::wstring_view::npos)
			{
				x = wcstof(str.data(), nullptr);
				y = wcstof(str.data() + comma1 + 1, nullptr);
			}
			else
			{
				x = wcstof(str.data(), nullptr);
			}

			return winrt::quaternion{ x, y, z, w };
		}

		/// <summary>
		/// A static container for animatable properties. Composite properties in
		/// the nested classes are manually stored as constants to avoid having
		/// to perform std::wstring_view interpolation at runtime and allocating memory.
		/// </summary>
		struct Properties
		{
			/// <summary>
			/// Animatable properties for the composition layer.
			/// </summary>
		    struct Composition
			{
				/// <summary>
				/// Gets the path for a <see cref="Visual.AnchorPoint"/> axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring AnchorPoint(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"AnchorPoint.X";
					case Axis::Y:
						return L"AnchorPoint.Y";
					case Axis::Z:
						return L"AnchorPoint.Z";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the path for the <see cref="Visual"/> translation property.
				/// </summary>
				/// <returns>The animation property.</returns>
				static winrt::hstring Translation() { return L"Translation"; }

				/// <summary>
				/// Gets the path for a <see cref="Visual"/> translation axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Translation(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"Translation.X";
					case Axis::Y:
						return L"Translation.Y";
					case Axis::Z:
						return L"Translation.Z";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the "Translation.XY" constant.
				/// </summary>
				/// <returns>The animation property.</returns>
				static winrt::hstring TranslationXY() { return L"Translation.XY"; }

				/// <summary>
				/// Gets the path for a <see cref="Visual.Offset"/> axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Offset(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"Offset.X";
					case Axis::Y:
						return L"Offset.Y";
					case Axis::Z:
						return L"Offset.Z";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the "Offset.XY" constant.
				/// </summary>
				/// <returns>The animation property.</returns>
				static winrt::hstring OffsetXY() { return L"Offset.XY"; }

				/// <summary>
				/// Gets the path for a <see cref="Visual.Scale"/> axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Scale(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"Scale.X";
					case Axis::Y:
						return L"Scale.Y";
					case Axis::Z:
						return L"Scale.Z";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the "Scale.XY" constant.
				/// </summary>
				/// <returns>The animation property.</returns>
				static winrt::hstring ScaleXY() { return L"Scale.XY"; }

				/// <summary>
				/// Gets the path for a <see cref="Visual.CenterPoint"/> axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring CenterPoint(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"CenterPoint.X";
					case Axis::Y:
						return L"CenterPoint.Y";
					case Axis::Z:
						return L"CenterPoint.Z";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the "CenterPoint.XY" constant.
				/// </summary>
				/// <returns>The animation property.</returns>
				static winrt::hstring CenterPointXY() { return L"CenterPoint.XY"; }

				/// <summary>
				/// Gets the path for an <see cref="InsetClip"/> side.
				/// </summary>
				/// <param name="side">The target side.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Clip(Side side)
				{
					switch (side)
					{
					case Side::Top:
						return L"TopInset";
					case Side::Bottom:
						return L"BottomInset";
					case Side::Right:
						return L"RightInset";
					case Side::Left:
						return L"LeftInset";
					default:
						return ThrowArgumentException(L"Invalid clip side");
					}
				}

				/// <summary>
				/// Gets the "Size" constant.
				/// </summary>
				/// <returns>The animation property.</returns>
				static winrt::hstring Size() { return L"Size"; }

				/// <summary>
				/// Gets the path for a <see cref="Visual.Size"/> axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Size(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"Size.X";
					case Axis::Y:
						return L"Size.Y";
					case Axis::Z:
						return L"Size.Z";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}
			};

			/// <summary>
			/// Animatable properties for the XAML layer.
			/// </summary>
			struct Xaml
			{
				/// <summary>
				/// Gets the path for a <see cref="CompositeTransform"/> translation axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Translation(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"TranslateX";
					case Axis::Y:
						return L"TranslateY";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the path for a <see cref="CompositeTransform"/> scale axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Scale(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"ScaleX";
					case Axis::Y:
						return L"ScaleY";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the path for a <see cref="CompositeTransform"/> center point axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring CenterPoint(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"CenterX";
					case Axis::Y:
						return L"CenterY";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}

				/// <summary>
				/// Gets the path for a <see cref="FrameworkElement"/> size axis.
				/// </summary>
				/// <param name="axis">The target axis.</param>
				/// <returns>The animation property.</returns>
				static winrt::hstring Size(Axis axis)
				{
					switch (axis)
					{
					case Axis::X:
						return L"Width";
					case Axis::Y:
						return L"Height";
					default:
						return ThrowArgumentException(L"Invalid axis");
					}
				}
			};

			/// <summary>
			/// Throws a new <see cref="ArgumentException"/> with a given message.
			/// </summary>
		private:
			template<typename T = winrt::hstring>
			static inline T ThrowArgumentException(winrt::hstring const& message)
			{
				throw winrt::hresult_invalid_argument(message);
			}
		};
	};
}
