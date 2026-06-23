#pragma once

#include "CameraHelper.g.h"

#ifdef __INTELLISENSE__
#include <semaphore>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Media.MediaProperties.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <wil/resource.h>
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

        winrt::MediaFrameSourceGroup FrameSourceGroup();

        void FrameSourceGroup(winrt::MediaFrameSourceGroup const& value);

        winrt::IVectorView<winrt::MediaFrameFormat> FrameFormatsAvailable();

        winrt::MediaFrameSource PreviewFrameSource();

        wil::untyped_event<winrt::XamlToolkit::WinUI::Helpers::FrameEventArgs> FrameArrived;

        winrt::IAsyncOperation<winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult> InitializeAndStartCaptureAsync();

        static winrt::IAsyncOperation<winrt::IVectorView<winrt::MediaFrameSourceGroup>> GetFrameSourceGroupsAsync();

        winrt::IAsyncAction CleanUpAsync();

        void Close();

    private:
        std::binary_semaphore _semaphoreSlim{ 1 };

        static inline winrt::IVectorView<winrt::MediaFrameSourceGroup> _frameSourceGroups{ nullptr };

        winrt::MediaCapture _mediaCapture{ nullptr };
        winrt::MediaFrameReader _frameReader{ nullptr };
        winrt::MediaFrameSourceGroup _group{ nullptr };
        winrt::MediaFrameSource _previewFrameSource{ nullptr };
        winrt::IVectorView<winrt::MediaFrameFormat> _frameFormatsAvailable{ nullptr };

        bool _groupChanged = false;
        bool _initialized = false;
        bool _disposed = false;

        winrt::MediaFrameReader::FrameArrived_revoker _frameArrivedRevoker;

        winrt::IAsyncOperation<winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult> InitializeMediaCaptureAsync();

        winrt::IAsyncAction StopReaderAsync();

        void Reader_FrameArrived(winrt::MediaFrameReader const& sender, winrt::MediaFrameArrivedEventArgs const& args);
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct CameraHelper : CameraHelperT<CameraHelper, implementation::CameraHelper>
    {
    };
}
