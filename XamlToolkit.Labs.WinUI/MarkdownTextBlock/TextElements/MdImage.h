// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../DefaultSVGRenderer.h"
#include "../Extension.h"
#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Web.Http.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <cmath>
#include <limits>
#include <memory>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Media::Imaging;
	using namespace Windows::Storage::Streams;
	using namespace Windows::Web::Http;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
	class MdImage final : public IAddChild
	{
	private:
		// Loading needs nothing but these values (plus an Image, which the provider decides).
		// They are snapshotted while parsing, so the delegate that holds this state refers
		// back to neither MdImage nor MarkdownTextBlock: MdImage can be released as soon as
		// it has been added to the tree, and there is no reference cycle to break.
		struct LoadState
		{
			winrt::Uri Uri{ nullptr };
			IImageProvider ImageProvider{ nullptr };
			ISVGRenderer SvgRenderer{ nullptr };
			double PrecedentWidth{ 0 };
			double PrecedentHeight{ 0 };
			double ThemeImageMaxWidth{ 0 };
			double ThemeImageMaxHeight{ 0 };
			winrt::Stretch ImageStretch{ winrt::Stretch::Uniform };
			bool Loaded{ false };
		};

		winrt::InlineUIContainer _container;
		winrt::Image _image;

	public:
		winrt::TextElement TextElement() const override
		{
			return _container;
		}

		MdImage(winrt::Uri const& uri, MarkdownTextBlock const& control)
		{
			auto state = std::make_shared<LoadState>();
			state->Uri = uri;
			state->ImageProvider = control.ImageProvider();
			state->SvgRenderer = control.SVGRenderer() ? control.SVGRenderer() : winrt::make<DefaultSVGRenderer>();

			auto size = Extensions::GetMarkdownImageSize(uri.AbsoluteUri());
			if (size.Width != 0)
			{
				state->PrecedentWidth = size.Width;
			}
			if (size.Height != 0)
			{
				state->PrecedentHeight = size.Height;
			}

			// Theme values are read here, while the control is still around
			state->ThemeImageMaxWidth = control.ImageMaxWidth();
			state->ThemeImageMaxHeight = control.ImageMaxHeight();
			state->ImageStretch = control.ImageStretch();

			Init(state);
		}

		static winrt::Windows::Web::Http::HttpClient& SharedHttpClient()
		{
			static winrt::Windows::Web::Http::HttpClient client;
			return client;
		}

		void Init(std::shared_ptr<LoadState> const& state)
		{
			_image.Loaded([state](winrt::IInspectable const& sender, winrt::RoutedEventArgs const&)
			{
				LoadImageAsync(sender.as<winrt::Image>(), state);
			});

			_container.Child(_image);
		}

		static winrt::fire_and_forget LoadImageAsync(winrt::Image image, std::shared_ptr<LoadState> state)
		{
			if (state->Loaded) co_return;

			try
			{
				// Track whether we have valid natural dimensions to constrain against
				bool hasNaturalWidth = false;
				bool hasNaturalHeight = false;

				auto container = image.Parent().try_as<winrt::InlineUIContainer>();

				if (state->ImageProvider && state->ImageProvider.ShouldUseThisProvider(state->Uri.AbsoluteUri()))
				{
					image = co_await state->ImageProvider.GetImage(state->Uri.AbsoluteUri());
					if (container)
					{
						container.Child(image);
					}

					// Capture natural dimensions as max constraints from the provider image
					// Then clear fixed Width/Height so images can shrink responsively
					auto imageWidth = image.Width();
					auto imageHeight = image.Height();
					if (imageWidth > 0 && !std::isnan(imageWidth) && !std::isinf(imageWidth))
					{
						image.MaxWidth(imageWidth);
						image.Width(std::numeric_limits<double>::quiet_NaN()); // Clear fixed width to allow shrinking
						hasNaturalWidth = true;
					}
					if (imageHeight > 0 && !std::isnan(imageHeight) && !std::isinf(imageHeight))
					{
						image.MaxHeight(imageHeight);
						image.Height(std::numeric_limits<double>::quiet_NaN()); // Clear fixed height to allow shrinking
						hasNaturalHeight = true;
					}

					state->Loaded = true;
				}
				else
				{
					// Download data from URL
					winrt::HttpResponseMessage response = co_await SharedHttpClient().GetAsync(state->Uri);

					if (!response.IsSuccessStatusCode())
					{
						co_return;
					}

					// Get the Content-Type header
					auto content = response.Content();
					winrt::hstring contentType = content.Headers().ContentType().MediaType();

					if (contentType == L"image/svg+xml")
					{
						winrt::hstring svgString = co_await content.ReadAsStringAsync();
						if (const auto& resImage = co_await state->SvgRenderer.SvgToImage(svgString))
						{
							image = resImage;
							if (container)
							{
								container.Child(image);
							}
						}
					}
					else
					{
						winrt::IBuffer data = co_await content.ReadAsBufferAsync();
						// Create a BitmapImage for other supported formats
						winrt::BitmapImage bitmap;
						winrt::InMemoryRandomAccessStream stream;

						// Write the data to the stream
						co_await stream.WriteAsync(data);
						stream.Seek(0);

						// Set the source of the BitmapImage
						co_await bitmap.SetSourceAsync(stream);

						image.Source(bitmap);

						// Don't set fixed Width/Height - let layout system handle it
						// Store natural dimensions for MaxWidth/MaxHeight constraints
						double naturalWidth = bitmap.PixelWidth() == 0 ? bitmap.DecodePixelWidth() : bitmap.PixelWidth();
						double naturalHeight = bitmap.PixelHeight() == 0 ? bitmap.DecodePixelHeight() : bitmap.PixelHeight();

						// Use natural size as max constraint so image doesn't upscale
						if (naturalWidth > 0)
						{
							image.MaxWidth(naturalWidth);
							hasNaturalWidth = true;
						}
						if (naturalHeight > 0)
						{
							image.MaxHeight(naturalHeight);
							hasNaturalHeight = true;
						}
					}

					state->Loaded = true;
				}

				// Apply precedent (markdown-specified) dimensions if provided
				// Precedent always takes priority and sets a known dimension
				if (state->PrecedentWidth != 0)
				{
					image.MaxWidth(state->PrecedentWidth);
					hasNaturalWidth = true;
				}
				if (state->PrecedentHeight != 0)
				{
					image.MaxHeight(state->PrecedentHeight);
					hasNaturalHeight = true;
				}

				// Apply theme constraints - only if we have a known dimension to constrain
				// This prevents theme constraints from enlarging images with unknown natural size
				auto themeImageWidth = state->ThemeImageMaxWidth;
				auto themeImageHeight = state->ThemeImageMaxHeight;
				if (themeImageWidth > 0 && hasNaturalWidth && themeImageWidth < image.MaxWidth())
				{
					image.MaxWidth(themeImageWidth);
				}
				if (themeImageHeight > 0 && hasNaturalHeight && themeImageHeight < image.MaxHeight())
				{
					image.MaxHeight(themeImageHeight);
				}

				image.Stretch(state->ImageStretch);
			}
			catch (...) {}
		}

		void SetToolTip(winrt::hstring const& tooltip)
		{
			winrt::ToolTipService::SetToolTip(_image, winrt::box_value(tooltip));
		}
	};
}
