// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once
#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#else
import winrt.Windows.Storage.Streams;
import winrt.Microsoft.UI.Xaml.Media.Imaging;
import winrt.XamlToolkit.WinUI.Helpers;
#endif

#include "ColorPickerRenderingHelpers.h"

namespace winrt
{
	using namespace Windows::Storage::Streams;
	using namespace Microsoft::UI::Xaml::Media::Imaging;
	using namespace XamlToolkit::WinUI::Helpers;
}

namespace winrt::XamlToolkit::WinUI::Controls
{
	using HsvColor = winrt::XamlToolkit::WinUI::HsvColor;
	using ColorHelper = winrt::XamlToolkit::WinUI::Helpers::ColorHelper;

	winrt::Color ColorPickerRenderingHelpers::CheckerBackgroundColor()
	{
		return winrt::Microsoft::UI::ColorHelper::FromArgb(0x19, 0x80, 0x80, 0x80);
	}

	winrt::IAsyncOperation<winrt::IVector<uint8_t>> ColorPickerRenderingHelpers::CreateChannelBitmapAsync(
		int width,
		int height,
		winrt::Orientation orientation,
		ColorRepresentation colorRepresentation,
		ColorChannel channel,
		HsvColor hsvColor,
		std::optional<winrt::Color> checkerColor,
		bool isAlphaMaxForced,
		bool isSaturationValueMaxForced)
	{
		if (width == 0 || height == 0)
		{
			co_return{};
		}

		co_await winrt::resume_background();

		auto baseHsvColor = hsvColor;

		std::uint32_t pixelDataIndex = 0;
		double channelStep;
		std::vector<std::uint8_t> bgraPixelData;
		winrt::IVector<std::uint8_t> bgraCheckeredPixelData{ nullptr };
		winrt::Color baseRgbColor{ winrt::Colors::White() };
		winrt::Color rgbColor{};
		int bgraPixelDataHeight;
		int bgraPixelDataWidth;

		// Allocate the buffer
		// BGRA formatted color channels 1 byte each (4 bytes in a pixel)
		bgraPixelData.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
		bgraPixelDataHeight = height * 4;
		bgraPixelDataWidth = width * 4;

		if (isAlphaMaxForced && channel != ColorChannel::Alpha)
		{
			baseHsvColor = { baseHsvColor.H, baseHsvColor.S, baseHsvColor.V, 1.0 };
		}

		if (colorRepresentation == ColorRepresentation::Rgba)
		{
			baseRgbColor = ColorHelper::FromHsv(
				baseHsvColor.H,
				baseHsvColor.S,
				baseHsvColor.V,
				baseHsvColor.A);
		}

		if (isSaturationValueMaxForced &&
			colorRepresentation == ColorRepresentation::Hsva &&
			channel != ColorChannel::Alpha)
		{
			switch (channel)
			{
			case ColorChannel::Channel1:
				baseHsvColor = HsvColor(baseHsvColor.H, 1.0, 1.0, baseHsvColor.A);
				break;
			case ColorChannel::Channel2:
				baseHsvColor = HsvColor(baseHsvColor.H, baseHsvColor.S, 1.0, baseHsvColor.A);
				break;
			case ColorChannel::Channel3:
				baseHsvColor = HsvColor(baseHsvColor.H, 1.0, baseHsvColor.V, baseHsvColor.A);
				break;
			}
		}

		if (checkerColor.has_value())
		{
			bgraCheckeredPixelData = co_await CreateCheckeredBitmapAsync(width, height, checkerColor.value());
		}

		auto GetColor = [&](double channelValue) -> Color
		{
			Color newRgbColor{ Colors::White() };

			switch (channel)
			{
				case ColorChannel::Channel1:
					if (colorRepresentation == ColorRepresentation::Hsva)
					{
						newRgbColor = ColorHelper::FromHsv(
							std::clamp(channelValue, 0.0, 360.0),
							baseHsvColor.S,
							baseHsvColor.V,
							baseHsvColor.A);
					}
					else
					{
						newRgbColor = winrt::Microsoft::UI::ColorHelper::FromArgb(
							baseRgbColor.A,
							static_cast<uint8_t>(std::clamp(channelValue, 0.0, 255.0)),
							baseRgbColor.G,
							baseRgbColor.B);
					}
					break;

				case ColorChannel::Channel2:
					if (colorRepresentation == ColorRepresentation::Hsva)
					{
						newRgbColor = ColorHelper::FromHsv(
							baseHsvColor.H,
							std::clamp(channelValue, 0.0, 1.0),
							baseHsvColor.V,
							baseHsvColor.A);
					}
					else
					{
						newRgbColor = winrt::Microsoft::UI::ColorHelper::FromArgb(
							baseRgbColor.A,
							baseRgbColor.R,
							static_cast<uint8_t>(std::clamp(channelValue, 0.0, 255.0)),
							baseRgbColor.B);
					}
					break;

				case ColorChannel::Channel3:
					if (colorRepresentation == ColorRepresentation::Hsva)
					{
						newRgbColor = ColorHelper::FromHsv(
							baseHsvColor.H,
							baseHsvColor.S,
							std::clamp(channelValue, 0.0, 1.0),
							baseHsvColor.A);
					}
					else
					{
						newRgbColor = winrt::Microsoft::UI::ColorHelper::FromArgb(
							baseRgbColor.A,
							baseRgbColor.R,
							baseRgbColor.G,
							static_cast<std::uint8_t>(std::clamp(channelValue, 0.0, 255.0)));
					}
					break;

				case ColorChannel::Alpha:
					if (colorRepresentation == ColorRepresentation::Hsva)
					{
						newRgbColor = ColorHelper::FromHsv(
							baseHsvColor.H,
							baseHsvColor.S,
							baseHsvColor.V,
							std::clamp(channelValue, 0.0, 1.0));
					}
					else
					{
						newRgbColor = winrt::Microsoft::UI::ColorHelper::FromArgb(
							static_cast<std::uint8_t>(std::clamp(channelValue, 0.0, 255.0)),
							baseRgbColor.R,
							baseRgbColor.G,
							baseRgbColor.B);
					}
					break;
			}

			return newRgbColor;
		};

		// Create the color channel gradient
		if (orientation == winrt::Orientation::Horizontal)
		{
			channelStep = (colorRepresentation == ColorRepresentation::Hsva)
				? ((channel == ColorChannel::Channel1) ? 360.0 / width : 1.0 / width)
				: 255.0 / width;

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					if (y == 0)
					{
						rgbColor = GetColor(x * channelStep);
						bgraPixelData[pixelDataIndex + 0] = static_cast<uint8_t>(rgbColor.B * rgbColor.A / 255);
						bgraPixelData[pixelDataIndex + 1] = static_cast<uint8_t>(rgbColor.G * rgbColor.A / 255);
						bgraPixelData[pixelDataIndex + 2] = static_cast<uint8_t>(rgbColor.R * rgbColor.A / 255);
						bgraPixelData[pixelDataIndex + 3] = rgbColor.A;
					}
					else
					{
						bgraPixelData[pixelDataIndex + 0] = bgraPixelData[pixelDataIndex + 0 - bgraPixelDataWidth];
						bgraPixelData[pixelDataIndex + 1] = bgraPixelData[pixelDataIndex + 1 - bgraPixelDataWidth];
						bgraPixelData[pixelDataIndex + 2] = bgraPixelData[pixelDataIndex + 2 - bgraPixelDataWidth];
						bgraPixelData[pixelDataIndex + 3] = bgraPixelData[pixelDataIndex + 3 - bgraPixelDataWidth];
					}
					pixelDataIndex += 4;
				}
			}
		}
		else
		{
			channelStep = (colorRepresentation == ColorRepresentation::Hsva)
				? ((channel == ColorChannel::Channel1) ? 360.0 / height : 1.0 / height)
				: 255.0 / height;

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					if (x == 0)
					{
						rgbColor = GetColor((height - 1 - y) * channelStep);
						bgraPixelData[pixelDataIndex + 0] = static_cast<std::uint8_t>(rgbColor.B * rgbColor.A / 255);
						bgraPixelData[pixelDataIndex + 1] = static_cast<std::uint8_t>(rgbColor.G * rgbColor.A / 255);
						bgraPixelData[pixelDataIndex + 2] = static_cast<std::uint8_t>(rgbColor.R * rgbColor.A / 255);
						bgraPixelData[pixelDataIndex + 3] = rgbColor.A;
					}
					else
					{
						bgraPixelData[pixelDataIndex + 0] = bgraPixelData[pixelDataIndex - 4];
						bgraPixelData[pixelDataIndex + 1] = bgraPixelData[pixelDataIndex - 3];
						bgraPixelData[pixelDataIndex + 2] = bgraPixelData[pixelDataIndex - 2];
						bgraPixelData[pixelDataIndex + 3] = bgraPixelData[pixelDataIndex - 1];
					}
					pixelDataIndex += 4;
				}
			}
		}

		// Composite checkered background if needed
		if (checkerColor.has_value() && !bgraCheckeredPixelData)
		{
			pixelDataIndex = 0;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					std::uint8_t rXa0 = bgraCheckeredPixelData.GetAt(pixelDataIndex + 2);
					std::uint8_t gXa0 = bgraCheckeredPixelData.GetAt(pixelDataIndex + 1);
					std::uint8_t bXa0 = bgraCheckeredPixelData.GetAt(pixelDataIndex + 0);
					std::uint8_t a0 = bgraCheckeredPixelData.GetAt(pixelDataIndex + 3);

					std::uint8_t rXa1 = bgraPixelData[pixelDataIndex + 2];
					std::uint8_t gXa1 = bgraPixelData[pixelDataIndex + 1];
					std::uint8_t bXa1 = bgraPixelData[pixelDataIndex + 0];
					std::uint8_t a1 = bgraPixelData[pixelDataIndex + 3];

					bgraPixelData[pixelDataIndex + 0] = static_cast<std::uint8_t>(((bXa0 * 255) - (bXa0 * a1) + (bXa1 * 255)) / 255);
					bgraPixelData[pixelDataIndex + 1] = static_cast<std::uint8_t>(((gXa0 * 255) - (gXa0 * a1) + (gXa1 * 255)) / 255);
					bgraPixelData[pixelDataIndex + 2] = static_cast<std::uint8_t>(((rXa0 * 255) - (rXa0 * a1) + (rXa1 * 255)) / 255);
					bgraPixelData[pixelDataIndex + 3] = static_cast<std::uint8_t>(((a0 * 255) - (a0 * a1) + (a1 * 255)) / 255);

					pixelDataIndex += 4;
				}
			}
		}

		co_return winrt::single_threaded_vector<std::uint8_t>(std::move(bgraPixelData));
	}

	winrt::IAsyncOperation<winrt::IVector<std::uint8_t>> ColorPickerRenderingHelpers::CreateCheckeredBitmapAsync(int width, int height, winrt::Color checkerColor)
	{
		const int checkerSize = 4;

		if (width == 0 || height == 0)
		{
			co_return{ nullptr };
		}

		co_await winrt::resume_background();

		std::vector<std::uint8_t> bgraPixelData(width * height * 4);
		size_t pixelDataIndex = 0;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				bool pixelShouldBeBlank = ((x / checkerSize) + (y / checkerSize)) % 2 == 0;

				if (pixelShouldBeBlank)
				{
					bgraPixelData[pixelDataIndex + 0] = 0;
					bgraPixelData[pixelDataIndex + 1] = 0;
					bgraPixelData[pixelDataIndex + 2] = 0;
					bgraPixelData[pixelDataIndex + 3] = 0;
				}
				else
				{
					bgraPixelData[pixelDataIndex + 0] = static_cast<std::uint8_t>(checkerColor.B * checkerColor.A / 255);
					bgraPixelData[pixelDataIndex + 1] = static_cast<std::uint8_t>(checkerColor.G * checkerColor.A / 255);
					bgraPixelData[pixelDataIndex + 2] = static_cast<std::uint8_t>(checkerColor.R * checkerColor.A / 255);
					bgraPixelData[pixelDataIndex + 3] = checkerColor.A;
				}

				pixelDataIndex += 4;
			}
		}

		co_return winrt::single_threaded_vector<std::uint8_t>(std::move(bgraPixelData));
	}

	winrt::ImageBrush ColorPickerRenderingHelpers::BitmapToBrush(winrt::IVector<std::uint8_t> const& bitmap, int width, int height)
	{
		winrt::WriteableBitmap writableBitmap(width, height);
		auto buffer = writableBitmap.PixelBuffer();
		std::copy(bitmap.begin(), bitmap.end(), buffer.data());
		winrt::ImageBrush brush;
		brush.ImageSource(writableBitmap);
		brush.Stretch(winrt::Stretch::None);
		return brush;
	}

	winrt::IAsyncAction ColorPickerRenderingHelpers::UpdateBorderBackgroundWithCheckerAsync(winrt::Border border, winrt::Color color)
	{
		if (border)
		{
			int width = static_cast<int>(border.ActualWidth());
			int height = static_cast<int>(border.ActualHeight());

			if (const auto& bitmap = co_await CreateCheckeredBitmapAsync(width, height, color))
			{
				border.Background(BitmapToBrush(bitmap, width, height));
			}
		}
	}
}
