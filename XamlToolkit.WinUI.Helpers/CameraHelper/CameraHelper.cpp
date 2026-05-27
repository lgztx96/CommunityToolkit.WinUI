#include "pch.h"
#include "winrt_module_imports.h"
#include "CameraHelper.h"
#if __has_include("CameraHelper.g.cpp")
#include "CameraHelper.g.cpp"
#endif
#include "FrameEventArgs.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Foundation::Numerics;
    using namespace Windows::Devices::Enumeration;
    using namespace Windows::Media::Capture;
    using namespace Windows::Media::Capture::Frames;
    using namespace Windows::Media::MediaProperties;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    IAsyncOperation<IVectorView<MediaFrameSourceGroup>> CameraHelper::GetFrameSourceGroupsAsync()
    {
        if (!_frameSourceGroups)
        {
            auto videoDevices = co_await DeviceInformation::FindAllAsync(DeviceClass::VideoCapture);
            auto groups = co_await MediaFrameSourceGroup::FindAllAsync();

            std::vector<MediaFrameSourceGroup> filtered;

            for (auto const& g : groups)
            {
                bool match = false;
                for (auto const& s : g.SourceInfos())
                {
                    if (s.SourceKind() == MediaFrameSourceKind::Color &&
                        (s.MediaStreamType() == MediaStreamType::VideoPreview ||
                            s.MediaStreamType() == MediaStreamType::VideoRecord))
                    {
                        for (auto const& vd : videoDevices)
                        {
                            if (vd.Id() == s.DeviceInformation().Id())
                            {
                                match = true;
                                break;
                            }
                        }
                    }
                }
                if (match)
                    filtered.push_back(g);
            }

            _frameSourceGroups = single_threaded_vector(std::move(filtered)).GetView();
        }

        co_return _frameSourceGroups;
    }

    MediaFrameSourceGroup CameraHelper::FrameSourceGroup() { return _group; }

    void CameraHelper::FrameSourceGroup(MediaFrameSourceGroup const& value)
    {
        _groupChanged = _group != value;
        _group = value;
    }

    Collections::IVectorView<MediaFrameFormat> CameraHelper::FrameFormatsAvailable()
    {
        return _frameFormatsAvailable;
    }

    MediaFrameSource CameraHelper::PreviewFrameSource() { return _previewFrameSource; }

    IAsyncOperation<CameraHelperResult> CameraHelper::InitializeAndStartCaptureAsync()
    {
        _semaphoreSlim.acquire();

        auto cleanup = wil::scope_exit([this]
            {
                _semaphoreSlim.release();
            });

        CameraHelperResult result = CameraHelperResult::Success;

        try
        {
            // if FrameSourceGroup hasn't changed from last initialization, just return back.
            if (_initialized && _group && !_groupChanged)
            {
                co_return CameraHelperResult::Success;
            }

            _groupChanged = false;

            co_await StopReaderAsync();

            if (_mediaCapture)
            {
                _mediaCapture.Close();
                _mediaCapture = nullptr;
            }

            if (_frameSourceGroups == nullptr)
            {
                _frameSourceGroups = co_await GetFrameSourceGroupsAsync();
            }

            if (_group == nullptr)
            {
                _group = _frameSourceGroups.Size() > 0 ? _frameSourceGroups.GetAt(0) : nullptr;
            }
            else
            {
                // Verify selected group is part of existing FrameSourceGroups
                for (auto const& group : _frameSourceGroups)
                {
                    if (group.Id() == _group.Id())
                    {
                        _group = group;
                        break;
                    }
                }
            }

            // If there is no camera source available, we can't proceed
            if (_group == nullptr)
            {
                co_return CameraHelperResult::NoFrameSourceGroupAvailable;
            }

            result = co_await InitializeMediaCaptureAsync();

            if (_previewFrameSource && _mediaCapture)
            {
                _frameReader = co_await _mediaCapture.CreateFrameReaderAsync(_previewFrameSource);
                if (_frameReader == nullptr)
                {
                    result = CameraHelperResult::CreateFrameReaderFailed;
                }
                else
                {
                    _frameReader.AcquisitionMode(MediaFrameReaderAcquisitionMode::Realtime);
                    _frameArrivedRevoker = _frameReader.FrameArrived(winrt::auto_revoke, { this, &CameraHelper::Reader_FrameArrived });
                    auto status = co_await _frameReader.StartAsync();
                    if (status != MediaFrameReaderStartStatus::Success)
                    {
                        result = CameraHelperResult::StartFrameReaderFailed;
                    }
                }
            }

            _initialized = (result == CameraHelperResult::Success);
        }
        catch (...)
        {
            result = CameraHelperResult::InitializationFailed_UnknownError;
        }

        if (result != CameraHelperResult::Success)
        {
            cleanup.reset();
            co_await CleanUpAsync();
        }

        co_return result;
    }

    IAsyncOperation<CameraHelperResult> CameraHelper::InitializeMediaCaptureAsync()
    {
        if (_mediaCapture == nullptr)
        {
            _mediaCapture = MediaCapture();
        }

        MediaCaptureInitializationSettings settings;
        settings.SourceGroup(_group);
        settings.MemoryPreference(MediaCaptureMemoryPreference::Cpu);
        settings.StreamingCaptureMode(StreamingCaptureMode::Video);

        CameraHelperResult result = CameraHelperResult::Success;

        try
        {
            co_await _mediaCapture.InitializeAsync(settings);

            for (auto const& kvp : _mediaCapture.FrameSources())
            {
                auto const& value = kvp.Value();
                auto const& info = value.Info();
                if (info.MediaStreamType() == MediaStreamType::VideoPreview &&
                    info.SourceKind() == MediaFrameSourceKind::Color)
                {
                    _previewFrameSource = value;
                    break;
                }
            }

            if (_previewFrameSource == nullptr)
            {
                for (auto const& kvp : _mediaCapture.FrameSources())
                {
                    auto const& value = kvp.Value();
                    auto const& info = value.Info();
                    if (info.MediaStreamType() == MediaStreamType::VideoRecord &&
                        info.SourceKind() == MediaFrameSourceKind::Color)
                    {
                        _previewFrameSource = value;
                        break;
                    }
                }
            }

            if (_previewFrameSource == nullptr)
            {
                co_return CameraHelperResult::NoFrameSourceAvailable;
            }

            std::vector<MediaFrameFormat> formats;

            auto compare_ignore_case = [](std::wstring_view lhs, std::wstring_view rhs)
            {
                return std::ranges::equal(lhs, rhs, [](wchar_t c1, wchar_t c2) { return std::tolower(c1) == std::tolower(c2); });
            };

            for (auto const& fmt : _previewFrameSource.SupportedFormats())
            {
                double fps = static_cast<double>(fmt.FrameRate().Numerator()) / fmt.FrameRate().Denominator();
                auto subtype = fmt.Subtype();
                if (fps >= 15 &&
                    (compare_ignore_case(subtype, MediaEncodingSubtypes::Nv12()) ||
                     compare_ignore_case(subtype, MediaEncodingSubtypes::Bgra8()) ||
                     compare_ignore_case(subtype, MediaEncodingSubtypes::Yuy2()) ||
                     compare_ignore_case(subtype, MediaEncodingSubtypes::Rgb32())))
                {
                    formats.emplace_back(fmt);
                }
            }

            if (formats.empty())
            {
                co_return CameraHelperResult::NoCompatibleFrameFormatAvailable;
            }

            std::sort(formats.begin(), formats.end(), [](auto const& a, auto const& b)
                { return a.VideoFormat().Width() * a.VideoFormat().Height() <
                b.VideoFormat().Width() * b.VideoFormat().Height(); });

            // Set the format with the highest resolution available by default
            _frameFormatsAvailable = winrt::single_threaded_vector(std::move(formats)).GetView();
            co_await _previewFrameSource.SetFormatAsync(_frameFormatsAvailable.GetAt(_frameFormatsAvailable.Size() - 1));
        }
        catch (winrt::hresult_error const& e)
        {
            result = e.code() == E_ACCESSDENIED ? CameraHelperResult::CameraAccessDenied : CameraHelperResult::InitializationFailed_UnknownError;
        }

        if (result != CameraHelperResult::Success)
        {
            co_await StopReaderAsync();

            if (_mediaCapture != nullptr)
            {
                _mediaCapture.Close();
                _mediaCapture = nullptr;
            }
        }

        co_return result;
    }

    IAsyncAction CameraHelper::StopReaderAsync()
    {
        if (_frameReader)
        {
            _frameArrivedRevoker.revoke();
            co_await _frameReader.StopAsync();
            _frameReader.Close();
            _frameReader = nullptr;
        }
    }

    void CameraHelper::Reader_FrameArrived(MediaFrameReader const& sender, MediaFrameArrivedEventArgs const&)
    {
        // TryAcquireLatestFrame will return the latest frame that has not yet been acquired.
        // This can return null if there is no such frame, or if the reader is not in the
        // "Started" state. The latter can occur if a FrameArrived event was in flight
        // when the reader was stopped.
        if (auto frame = sender.TryAcquireLatestFrame())
        {
            if (auto vmf = frame.VideoMediaFrame())
            {
                auto args = winrt::make_self<FrameEventArgs>();
                args->VideoFrame(vmf.GetVideoFrame());
                FrameArrived.invoke(*this, *args);
            }
        }
    }

    IAsyncAction CameraHelper::CleanUpAsync()
    {
        _semaphoreSlim.acquire();

        try
        {
            _initialized = false;
            co_await StopReaderAsync();

            if (_mediaCapture)
            {
                _mediaCapture.Close();
                _mediaCapture = nullptr;
            }
        }
        catch (...) {}

        _semaphoreSlim.release();
    }

    void CameraHelper::Close()
    {
        if (!_disposed)
        {
            CleanUpAsync().get();
            _disposed = true;
        }
    }
}
