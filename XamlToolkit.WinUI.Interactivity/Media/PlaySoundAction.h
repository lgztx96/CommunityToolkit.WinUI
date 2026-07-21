#pragma once

#include "PlaySoundAction.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Dispatching;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Windows::Foundation;
    using namespace Windows::Media::Core;
    using namespace Windows::Media::Playback;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    /// <summary>
    /// An action that will play a sound to completion.
    /// </summary>
    /// <remarks>
    /// This action is intended for use with short sound effects that don't need to be stopped or controlled. If you are trying
    /// to create a music player or game, it may not meet your needs.
    /// </remarks>
    struct PlaySoundAction : PlaySoundActionT<PlaySoundAction>
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PlaySoundAction"/> class.
        /// </summary>
        PlaySoundAction();

        /// <summary>
        /// Identifies the <seealso cref="Source"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty;
        /// <summary>
        /// Identifies the <seealso cref="Volume"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> VolumeProperty;

        /// <summary>
        /// Gets or sets the location of the sound file. This is used to set the source property of a <see cref="MediaPlayerElement"/>. This is a dependency property.
        /// </summary>
        /// <remarks>
        /// The sound can be any file format supported by <see cref="MediaPlayerElement"/>. In the case of a video, it will play only the
        /// audio portion.
        /// </remarks>
        winrt::hstring Source() const;
        void Source(winrt::hstring const& value);

        /// <summary>
        /// Gets or set the volume of the sound. This is used to set the <see cref="MediaPlayer.Volume"/> property of the <see cref="MediaPlayerElement"/>. This is a dependency property.
        /// </summary>
        /// <remarks>
        /// By default this is set to 0.5.
        /// </remarks>
        double Volume() const;
        void Volume(double value);

        /// <summary>
        /// Executes the action.
        /// </summary>
        /// <param name="sender">The <see cref="object"/> that is passed to the action by the behavior. Generally this is <seealso cref="IBehavior.AssociatedObject"/> or a target object.</param>
        /// <param name="parameter">The value of this parameter is determined by the caller.</param>
        /// <returns>True if <see cref="MediaPlayerElement.Source"/> is set successfully; else false.</returns>
        winrt::IInspectable Execute(winrt::IInspectable const& sender, winrt::IInspectable const& parameter);

    private:
        void MediaPlayer_MediaFailed(winrt::MediaPlayer const& sender, winrt::MediaPlayerFailedEventArgs const& args);
        void MediaElement_MediaEnded(winrt::MediaPlayer const& sender, winrt::IInspectable const& args);
        void ClosePopup();

        winrt::DispatcherQueue _queue{ nullptr };
        winrt::Popup _popup{ nullptr };
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct PlaySoundAction : PlaySoundActionT<PlaySoundAction, implementation::PlaySoundAction>
    {
    };
}
