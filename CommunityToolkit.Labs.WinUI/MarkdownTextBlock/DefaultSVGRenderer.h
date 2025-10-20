#pragma once
#include <winrt/Windows.Foundation.h>
#include <winrt/CommunityToolkit.Labs.WinUI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include "Extension.h"

namespace winrt::CommunityToolkit::Labs::WinUI
{
    struct DefaultSVGRenderer : winrt::implements<DefaultSVGRenderer, winrt::CommunityToolkit::Labs::WinUI::ISVGRenderer>
    {
        Windows::Foundation::IAsyncOperation<Microsoft::UI::Xaml::Controls::Image> SvgToImage(std::wstring_view svgString)
        {
            Microsoft::UI::Xaml::Media::Imaging::SvgImageSource svgImageSource;
            Microsoft::UI::Xaml::Controls::Image image;
            // Create a MemoryStream object and write the SVG string to it
            Windows::Storage::Streams::InMemoryRandomAccessStream memoryStream;

            auto outputStream = memoryStream.GetOutputStreamAt(0);
            auto streamWriter = Windows::Storage::Streams::DataWriter(outputStream);

            streamWriter.WriteString(svgString);
            co_await streamWriter.FlushAsync();

            // Rewind the MemoryStream
            memoryStream.Seek(0);

            // Load the SVG from the MemoryStream
            co_await svgImageSource.SetSourceAsync(memoryStream);

            // Set the Source property of the Image control to the SvgImageSource object
            image.Source(svgImageSource);
            auto size = Extensions::GetSvgSize(svgString);
            if (size.Width != 0)
            {
                image.Width(size.Width);
            }
            if (size.Height != 0)
            {
                image.Height(size.Height);
            }
            co_return image;
        }
    };
}
