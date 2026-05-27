#pragma once

#include "CameraHelper.g.h"

#ifdef __INTELLISENSE__
#include <semaphore>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Media::Capture;
    using namespace Windows::Media::Capture::Frames;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct CameraHelper : CameraHelperT<CameraHelper>
    {
        CameraHelper() = default;

        MediaFrameSourceGroup FrameSourceGroup();

        void FrameSourceGroup(MediaFrameSourceGroup const& value);

        IVectorView<MediaFrameFormat> FrameFormatsAvailable();

        MediaFrameSource PreviewFrameSource();

        wil::untyped_event<winrt::XamlToolkit::WinUI::Helpers::FrameEventArgs> FrameArrived;

        IAsyncOperation<winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult> InitializeAndStartCaptureAsync();

        static IAsyncOperation<IVectorView<MediaFrameSourceGroup>> GetFrameSourceGroupsAsync();

        IAsyncAction CleanUpAsync();

        void Close();

    private:
        std::binary_semaphore _semaphoreSlim{ 1 };

        static inline IVectorView<MediaFrameSourceGroup> _frameSourceGroups{ nullptr };

        MediaCapture _mediaCapture{ nullptr };
        MediaFrameReader _frameReader{ nullptr };
        MediaFrameSourceGroup _group{ nullptr };
        MediaFrameSource _previewFrameSource{ nullptr };
        IVectorView<MediaFrameFormat> _frameFormatsAvailable{ nullptr };

        bool _groupChanged = false;
        bool _initialized = false;
        bool _disposed = false;

        MediaFrameReader::FrameArrived_revoker _frameArrivedRevoker;

        IAsyncOperation<winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult> InitializeMediaCaptureAsync();

        winrt::Windows::Foundation::IAsyncAction StopReaderAsync();

        void Reader_FrameArrived(MediaFrameReader const& sender, MediaFrameArrivedEventArgs const& args);
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct CameraHelper : CameraHelperT<CameraHelper, implementation::CameraHelper>
    {
    };
}
