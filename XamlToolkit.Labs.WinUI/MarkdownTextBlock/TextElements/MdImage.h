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
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <cmath>
#include <limits>
#include <memory>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Media::Imaging;
	using namespace Windows::Storage::Streams;
	using namespace Windows::Web::Http;
}

#undef LoadImage

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
	class MdImage final : public IAddChild, public std::enable_shared_from_this<MdImage>
	{
	private:
		InlineUIContainer _container;
		Microsoft::UI::Xaml::Controls::Image _image;
		Windows::Foundation::Uri _uri;
		IImageProvider _imageProvider{ nullptr };
		ISVGRenderer _svgRenderer{ nullptr };
		MarkdownThemes _themes{ nullptr };
		double _precedentWidth;
		double _precedentHeight;
		bool _loaded = false;

	public:
		Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
		{
			return _container;
		}

		MdImage(Windows::Foundation::Uri const& uri, MarkdownConfig const& config)
			: _uri(uri), _precedentWidth(0), _precedentHeight(0)
		{
			_imageProvider = config.ImageProvider();
			_svgRenderer = config.SVGRenderer() ? config.SVGRenderer() : winrt::make<DefaultSVGRenderer>();
			_themes = config.Themes();

			Init();
			auto linkInline = uri.AbsoluteUri();
			auto size = Extensions::GetMarkdownImageSize(linkInline);
			if (size.Width != 0)
			{
				_precedentWidth = size.Width;
			}
			if (size.Height != 0)
			{
				_precedentHeight = size.Height;
			}
		}

		//MdImage(MarkdownConfig config)
		//{
		//    Windows::Foundation::Uri::Create(htmlNode.GetAttributeValue("src", "#"), UriKind::RelativeOrAbsolute, out _uri);

		//    _htmlNode = htmlNode;
		//    _imageProvider = config.ImageProvider();
		//    _svgRenderer = config.SVGRenderer() == nullptr ? DefaultSVGRenderer() : config.SVGRenderer();
		//    Init();
		//   /* int.TryParse(
		//        htmlNode.GetAttributeValue("width", "0"),
		//        NumberStyles.Integer,
		//        CultureInfo.InvariantCulture,
		//        out auto width
		//    );
		//    int.TryParse(
		//        htmlNode.GetAttributeValue("height", "0"),
		//        NumberStyles.Integer,
		//        CultureInfo.InvariantCulture,
		//        out auto height
		//    );
		//    if (width > 0)
		//    {
		//        _precedentWidth = width;
		//    }
		//    if (height > 0)
		//    {
		//        _precedentHeight = height;
		//    }*/
		//}

		void Init()
		{
			_image.Loaded({ this, &MdImage::LoadImage });
			_container.Child(_image);
		}

		winrt::Windows::Foundation::IAsyncAction LoadImage(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e)
		{
			if (_loaded) co_return;
			try
			{
				auto self = shared_from_this();
				// Track whether we have valid natural dimensions to constrain against
				bool hasNaturalWidth = false;
				bool hasNaturalHeight = false;

				if (_imageProvider != nullptr && _imageProvider.ShouldUseThisProvider(_uri.AbsoluteUri()))
				{
					_image = co_await _imageProvider.GetImage(_uri.AbsoluteUri());
					_container.Child(_image);

					// Capture natural dimensions as max constraints from the provider image
					// Then clear fixed Width/Height so images can shrink responsively
					auto imageWidth = _image.Width();
					auto imageHeight = _image.Height();
					if (imageWidth > 0 && !std::isnan(imageWidth) && !std::isinf(imageWidth))
					{
						_image.MaxWidth(imageWidth);
						_image.Width(std::numeric_limits<double>::quiet_NaN()); // Clear fixed width to allow shrinking
						hasNaturalWidth = true;
					}
					if (imageHeight > 0 && !std::isnan(imageHeight) && !std::isinf(imageHeight))
					{
						_image.MaxHeight(imageHeight);
						_image.Height(std::numeric_limits<double>::quiet_NaN()); // Clear fixed height to allow shrinking
						hasNaturalHeight = true;
					}

					_loaded = true;
				}
				else
				{
					Windows::Web::Http::HttpClient client;

					// Download data from URL
					HttpResponseMessage response = co_await client.GetAsync(_uri);

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
						if (const auto& resImage = co_await _svgRenderer.SvgToImage(svgString))
						{
							_image = resImage;
							_container.Child(_image);
						}
					}
					else
					{
						IBuffer data = co_await content.ReadAsBufferAsync();
						// Create a BitmapImage for other supported formats
						BitmapImage bitmap;
						InMemoryRandomAccessStream stream;

						// Write the data to the stream
						co_await stream.WriteAsync(data);
						stream.Seek(0);

						// Set the source of the BitmapImage
						co_await bitmap.SetSourceAsync(stream);

						_image.Source(bitmap);

						// Don't set fixed Width/Height - let layout system handle it
						// Store natural dimensions for MaxWidth/MaxHeight constraints
						double naturalWidth = bitmap.PixelWidth() == 0 ? bitmap.DecodePixelWidth() : bitmap.PixelWidth();
						double naturalHeight = bitmap.PixelHeight() == 0 ? bitmap.DecodePixelHeight() : bitmap.PixelHeight();

						// Use natural size as max constraint so image doesn't upscale
						if (naturalWidth > 0)
						{
							_image.MaxWidth(naturalWidth);
							hasNaturalWidth = true;
						}
						if (naturalHeight > 0)
						{
							_image.MaxHeight(naturalHeight);
							hasNaturalHeight = true;
						}
					}

					_loaded = true;
				}

				// Apply precedent (markdown-specified) dimensions if provided
				// Precedent always takes priority and sets a known dimension
				if (_precedentWidth != 0)
				{
					_image.MaxWidth(_precedentWidth);
					hasNaturalWidth = true;
				}
				if (_precedentHeight != 0)
				{
					_image.MaxHeight(_precedentHeight);
					hasNaturalHeight = true;
				}

				// Apply theme constraints - only if we have a known dimension to constrain
				// This prevents theme constraints from enlarging images with unknown natural size
				auto themeImageWidth = _themes.ImageMaxWidth();
				auto themeImageHeight = _themes.ImageMaxHeight();
				if (themeImageWidth > 0 && hasNaturalWidth && themeImageWidth < _image.MaxWidth())
				{
					_image.MaxWidth(themeImageWidth);
				}
				if (themeImageHeight > 0 && hasNaturalHeight && themeImageHeight < _image.MaxHeight())
				{
					_image.MaxHeight(themeImageHeight);
				}

				_image.Stretch(_themes.ImageStretch());
			}
			catch (const winrt::hresult_error&) {}
		}

		void SetToolTip(winrt::hstring const& tooltip)
		{
			Microsoft::UI::Xaml::Controls::ToolTipService::SetToolTip(_image, winrt::box_value(tooltip));
		}
	};
}

