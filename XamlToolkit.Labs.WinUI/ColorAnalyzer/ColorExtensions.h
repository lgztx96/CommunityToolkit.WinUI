#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.h>
#include <cmath>
#include <concepts>
#include <ranges>
#include <span>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
	using namespace Windows::UI;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    template<typename R, typename V>
    concept input_range_of = std::ranges::input_range<R> && std::common_reference_with<std::ranges::range_value_t<R>, V>;

	struct ColorExtensions
	{
        static Color ToColor(float3 color);

        static float3 ToVector3(Color color);

        static double ContrastRatio(Color color1, Color color2);

        static double RelativeLuminance(Color color);

        static float FindColorfulness(Color color);

        static float FindColorfulness(std::span<Color> colors);

        private:
            template<typename R> requires std::ranges::forward_range<R> && input_range_of<R, float>
            static float FindStandardDeviation(R&& data, float& avg)
            {
                float sum = 0.0f;
                std::size_t count = 0;

                for (float x : data)
                {
                    sum += x;
                    ++count;
                }

                avg = sum / count;

                float sumOfSquares = 0.0f;

                for (float x : data)
                {
                    float d = x - avg;
                    sumOfSquares += d * d;
                }

                return std::sqrt(sumOfSquares / count);
            }
	};
}

