#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <string>
#endif
#include "PlaySoundAction.h"
#if __has_include("PlaySoundAction.g.cpp")
#include "PlaySoundAction.g.cpp"
#endif

namespace
{
	constexpr std::wstring_view MsAppxPrefix = L"ms-appx:///";

	winrt::Windows::Foundation::Uri TryCreateUri(winrt::hstring const& source)
	{
		try
		{
			return winrt::Windows::Foundation::Uri(source);
		}
		catch (winrt::hresult_error const&)
		{
		}

		// Impose ms-appx:// scheme if user has specified a relative URI
		std::wstring absoluteSource{ MsAppxPrefix };
		absoluteSource.append(source.c_str());

		try
		{
			return winrt::Windows::Foundation::Uri(absoluteSource);
		}
		catch (winrt::hresult_error const&)
		{
		}

		return nullptr;
	}
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
	PlaySoundAction::PlaySoundAction()
		: _queue(winrt::DispatcherQueue::GetForCurrentThread())
	{}

	const wil::single_threaded_property<winrt::DependencyProperty> PlaySoundAction::SourceProperty = winrt::DependencyProperty::Register(
		L"Source",
		winrt::xaml_typename<winrt::hstring>(),
		winrt::xaml_typename<class_type>(),
		winrt::PropertyMetadata(winrt::box_value(L"")));

	const wil::single_threaded_property<winrt::DependencyProperty> PlaySoundAction::VolumeProperty = winrt::DependencyProperty::Register(
		L"Volume",
		winrt::xaml_typename<double>(),
		winrt::xaml_typename<class_type>(),
		winrt::PropertyMetadata(winrt::box_value(0.5)));

	winrt::hstring PlaySoundAction::Source() const
	{
		auto value = GetValue(SourceProperty());
		return winrt::unbox_value_or<winrt::hstring>(value, L"");
	}

	void PlaySoundAction::Source(winrt::hstring const& value)
	{
		SetValue(SourceProperty(), winrt::box_value(value));
	}

	double PlaySoundAction::Volume() const
	{
		auto value = GetValue(VolumeProperty());
		return winrt::unbox_value_or<double>(value, 0.5);
	}

	void PlaySoundAction::Volume(double value)
	{
		SetValue(VolumeProperty(), winrt::box_value(value));
	}

	winrt::IInspectable PlaySoundAction::Execute(winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& parameter)
	{
		winrt::hstring source = Source();
		if (source.empty())
		{
			return winrt::box_value(false);
		}

		const auto sourceUri = TryCreateUri(source);
		if (sourceUri == nullptr)
		{
			return winrt::box_value(false);
		}

		_popup = winrt::Popup();
		if (const auto element = sender.try_as<winrt::UIElement>())
		{
			if (const auto xamlRoot = element.XamlRoot())
			{
				_popup.XamlRoot(xamlRoot);
			}
		}

		winrt::MediaPlayerElement mediaElement;
		_popup.Child(mediaElement);

		// It is legal (although not advisable) to provide a video file. By setting visibility to collapsed, only the sound track should play.
		mediaElement.Visibility(winrt::Visibility::Collapsed);
		mediaElement.Source(winrt::MediaSource::CreateFromUri(sourceUri));
		mediaElement.AutoPlay(true);

		auto mediaPlayer = mediaElement.MediaPlayer();
		mediaPlayer.Volume(Volume());
		mediaPlayer.MediaEnded({ get_weak(), &PlaySoundAction::MediaElement_MediaEnded });
		mediaPlayer.MediaFailed({ get_weak(), &PlaySoundAction::MediaPlayer_MediaFailed });

		_popup.IsOpen(true);
		return winrt::box_value(true);
	}

	void PlaySoundAction::MediaPlayer_MediaFailed([[maybe_unused]] winrt::MediaPlayer const& sender, [[maybe_unused]] winrt::MediaPlayerFailedEventArgs const& args)
	{
		// TODO: We should probably have some system/properties to report/bubble errors here
		ClosePopup();
	}

	void PlaySoundAction::MediaElement_MediaEnded([[maybe_unused]] winrt::MediaPlayer const& sender, [[maybe_unused]] winrt::IInspectable const& args)
	{
		ClosePopup();
	}

	void PlaySoundAction::ClosePopup()
	{
		auto weak = get_weak();
		auto closePopupImpl = [weak]()
		{
			if (auto strong = weak.get()) 
			{
				if (strong->_popup)
				{
					strong->_popup.IsOpen(false);
					strong->_popup.Child(nullptr);
					strong->_popup = nullptr;
				}
			}
		};

		if (_queue.HasThreadAccess())
		{
			closePopupImpl();
		}
		else
		{
			// In WinUI3 the Media events are called on a background thread, so ensure we're on the UI thread to modify our popup container.
			_queue.TryEnqueue(winrt::DispatcherQueuePriority::Normal, closePopupImpl);
		}
	}
}

