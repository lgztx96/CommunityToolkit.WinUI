#include "pch.h"
#include "winrt_module_imports.h"
#include "CameraPreview.h"
#if __has_include("CameraPreview.g.cpp")
#include "CameraPreview.g.cpp"
#endif
#include "PreviewFailedEventArgs.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	inline winrt::hstring ToString(CameraHelperResult value)
	{
		switch (value)
		{
		case CameraHelperResult::Success:
			return L"Success";
		case CameraHelperResult::CreateFrameReaderFailed:
			return L"CreateFrameReaderFailed";
		case CameraHelperResult::StartFrameReaderFailed:
			return L"StartFrameReaderFailed";
		case CameraHelperResult::NoFrameSourceGroupAvailable:
			return L"NoFrameSourceGroupAvailable";
		case CameraHelperResult::NoFrameSourceAvailable:
			return L"NoFrameSourceAvailable";
		case CameraHelperResult::CameraAccessDenied:
			return L"CameraAccessDenied";
		case CameraHelperResult::InitializationFailed_UnknownError:
			return L"InitializationFailed_UnknownError";
		case CameraHelperResult::NoCompatibleFrameFormatAvailable:
			return L"NoCompatibleFrameFormatAvailable";
		default:
			return L"Unknown";
		}
	}

	CameraPreview::CameraPreview()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void CameraPreview::IsFrameSourceGroupButtonVisibleChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto cameraPreview = winrt::get_self<CameraPreview>(control);

			if (cameraPreview->_frameSourceGroupButton != nullptr)
			{
				cameraPreview->SetFrameSourceGroupButtonVisibility();
			}
		}
	}

	IAsyncAction CameraPreview::StartAsync()
	{
		co_await StartAsync(winrt::XamlToolkit::WinUI::Helpers::CameraHelper());
	}

	IAsyncAction CameraPreview::StartAsync(winrt::XamlToolkit::WinUI::Helpers::CameraHelper cameraHelper)
	{
		if (cameraHelper == nullptr)
		{
			cameraHelper = winrt::XamlToolkit::WinUI::Helpers::CameraHelper();
		}

		_cameraHelper = cameraHelper;
		_frameSourceGroups = co_await CameraHelper.GetFrameSourceGroupsAsync();

		// UI controls exist and are initialized
		if (_mediaPlayerElementControl != nullptr)
		{
			co_await InitializeAsync();
		}
	}

	winrt::fire_and_forget CameraPreview::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		if (_frameSourceGroupButton != nullptr)
		{
			_frameSourceGroupButtonClickRevoker.revoke();
		}

		_mediaPlayerElementControl = GetTemplateChild(Preview_MediaPlayerElementControl).try_as<MediaPlayerElement>();
		_frameSourceGroupButton = GetTemplateChild(Preview_FrameSourceGroupButton).try_as<Button>();;

		if (_frameSourceGroupButton != nullptr)
		{
			_frameSourceGroupButtonClickRevoker =
				_frameSourceGroupButton.Click(winrt::auto_revoke, { this, &CameraPreview::FrameSourceGroupButton_ClickAsync });
			_frameSourceGroupButton.IsEnabled(false);
			_frameSourceGroupButton.Visibility(Visibility::Collapsed);
		}

		if (_cameraHelper != nullptr)
		{
			co_await InitializeAsync();
		}
	}

	IAsyncAction CameraPreview::InitializeAsync()
	{
		auto result = co_await _cameraHelper.InitializeAndStartCaptureAsync();
		if (result != CameraHelperResult::Success)
		{
			InvokePreviewFailed(ToString(result));
		}

		SetUIControls(result);
	}

	IAsyncAction CameraPreview::FrameSourceGroupButton_ClickAsync(IInspectable const& sender, RoutedEventArgs const& e)
	{
		auto oldGroup = _cameraHelper.FrameSourceGroup();
		int currentIndex = 0;

		for (uint32_t i = 0; i < _frameSourceGroups.Size(); ++i)
		{
			if (_frameSourceGroups.GetAt(i).Id() == oldGroup.Id())
			{
				currentIndex = i;
				break;
			}
		}

		uint32_t newIndex = (currentIndex < static_cast<int>(_frameSourceGroups.Size()) - 1)
			? currentIndex + 1
			: 0;

		auto group = _frameSourceGroups.GetAt(newIndex);
		_frameSourceGroupButton.IsEnabled(false);
		_cameraHelper.FrameSourceGroup(group);
		co_await InitializeAsync();
	}

	void CameraPreview::InvokePreviewFailed(winrt::hstring const& error)
	{
		auto args = winrt::make_self<PreviewFailedEventArgs>();
		args->Error = error;
		PreviewFailed.invoke(*this, *args);
	}

	void CameraPreview::SetMediaPlayerSource()
	{
		try
		{
			if (_cameraHelper == nullptr)
			{
				return;
			}

			if (auto frameSource = _cameraHelper.PreviewFrameSource())
			{
				if (_mediaPlayer == nullptr)
				{
					_mediaPlayer = MediaPlayer();
					_mediaPlayer.AutoPlay(true);
					_mediaPlayer.RealTimePlayback(true);
				}

				_mediaPlayer.Source(winrt::Windows::Media::Core::MediaSource::CreateFromMediaFrameSource(frameSource));
				_mediaPlayerElementControl.SetMediaPlayer(_mediaPlayer);
			}
		}
		catch (winrt::hresult_error const& ex)
		{
			InvokePreviewFailed(ex.message());
		}
	}

	void CameraPreview::SetUIControls(CameraHelperResult result)
	{
		auto success = result == CameraHelperResult::Success;
		if (success)
		{
			SetMediaPlayerSource();
		}
		else
		{
			_mediaPlayerElementControl.SetMediaPlayer(nullptr);
		}

		_frameSourceGroupButton.IsEnabled(IsFrameSourceGroupButtonAvailable());
		SetFrameSourceGroupButtonVisibility();
	}

	void CameraPreview::SetFrameSourceGroupButtonVisibility()
	{
		_frameSourceGroupButton.Visibility(IsFrameSourceGroupButtonAvailable() && IsFrameSourceGroupButtonVisible()
			? Visibility::Visible
			: Visibility::Collapsed);
	}

	void CameraPreview::Stop()
	{
		if (_mediaPlayerElementControl != nullptr)
		{
			_mediaPlayerElementControl.SetMediaPlayer(nullptr);
		}

		if (_mediaPlayer != nullptr)
		{
			_mediaPlayer.Close();
			_mediaPlayer = nullptr;
		}
	}
}
