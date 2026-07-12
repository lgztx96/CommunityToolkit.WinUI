#include "pch.h"
#include "winrt_module_imports.h"
#include "CameraPreview.h"
#if __has_include("CameraPreview.g.cpp")
#include "CameraPreview.g.cpp"
#endif
#include "PreviewFailedEventArgs.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	static winrt::hstring ToString(CameraHelperResult value)
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

	void CameraPreview::IsFrameSourceGroupButtonVisibleChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto cameraPreview = winrt::get_self<CameraPreview>(control);

			if (cameraPreview->_frameSourceGroupButton)
			{
				cameraPreview->SetFrameSourceGroupButtonVisibility();
			}
		}
	}

	winrt::IAsyncAction CameraPreview::StartAsync()
	{
		co_await StartAsync(winrt::CameraHelper());
	}

	winrt::IAsyncAction CameraPreview::StartAsync(winrt::CameraHelper const& cameraHelper)
	{
		auto strongThis = get_strong();

		CameraHelper = cameraHelper ? cameraHelper : winrt::CameraHelper();
		_frameSourceGroups = co_await CameraHelper.GetFrameSourceGroupsAsync();

		// UI controls exist and are initialized
		if (_mediaPlayerElementControl)
		{
			co_await InitializeAsync();
		}
	}

	winrt::fire_and_forget CameraPreview::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		if (_frameSourceGroupButton)
		{
			_frameSourceGroupButtonClickRevoker.revoke();
		}

		_mediaPlayerElementControl = GetTemplateChild(Preview_MediaPlayerElementControl).try_as<winrt::MediaPlayerElement>();
		_frameSourceGroupButton = GetTemplateChild(Preview_FrameSourceGroupButton).try_as<winrt::Button>();

		if (_frameSourceGroupButton)
		{
			_frameSourceGroupButtonClickRevoker =
				_frameSourceGroupButton.Click(winrt::auto_revoke, { this, &CameraPreview::FrameSourceGroupButton_ClickAsync });
			_frameSourceGroupButton.IsEnabled(false);
			_frameSourceGroupButton.Visibility(winrt::Visibility::Collapsed);
		}

		if (CameraHelper)
		{
			co_await InitializeAsync();
		}
	}

	winrt::IAsyncAction CameraPreview::InitializeAsync()
	{
		auto result = co_await CameraHelper.InitializeAndStartCaptureAsync();
		if (result != CameraHelperResult::Success)
		{
			InvokePreviewFailed(ToString(result));
		}

		SetUIControls(result);
	}

	winrt::IAsyncAction CameraPreview::FrameSourceGroupButton_ClickAsync(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e)
	{
		const auto oldGroup = CameraHelper.FrameSourceGroup();
		int currentIndex = 0;

		for (uint32_t i = 0; i < _frameSourceGroups.Size(); ++i)
		{
			if (_frameSourceGroups.GetAt(i).Id() == oldGroup.Id())
			{
				currentIndex = i;
				break;
			}
		}

		uint32_t newIndex = (currentIndex < static_cast<int>(_frameSourceGroups.Size()) - 1) ? currentIndex + 1 : 0;
		const auto group = _frameSourceGroups.GetAt(newIndex);
		_frameSourceGroupButton.IsEnabled(false);
		CameraHelper.FrameSourceGroup(group);
		co_await InitializeAsync();
	}

	void CameraPreview::InvokePreviewFailed(winrt::hstring const& error)
	{
		auto args = winrt::make_self<PreviewFailedEventArgs>(error);
		PreviewFailed.invoke(*this, *args);
	}

	void CameraPreview::SetMediaPlayerSource()
	{
		try
		{
			if (CameraHelper == nullptr)
			{
				return;
			}

			if (const auto frameSource = CameraHelper.PreviewFrameSource())
			{
				if (_mediaPlayer == nullptr)
				{
					_mediaPlayer = winrt::MediaPlayer();
					_mediaPlayer.AutoPlay(true);
					_mediaPlayer.RealTimePlayback(true);
				}

				_mediaPlayer.Source(winrt::MediaSource::CreateFromMediaFrameSource(frameSource));
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
		if (result == CameraHelperResult::Success)
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
		_frameSourceGroupButton.Visibility(
			IsFrameSourceGroupButtonAvailable() && IsFrameSourceGroupButtonVisible()
			? winrt::Visibility::Visible
			: winrt::Visibility::Collapsed);
	}

	void CameraPreview::Stop()
	{
		if (_mediaPlayerElementControl)
		{
			_mediaPlayerElementControl.SetMediaPlayer(nullptr);
		}

		if (_mediaPlayer)
		{
			_mediaPlayer.Close();
			_mediaPlayer = nullptr;
		}
	}
}
