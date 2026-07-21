#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <memory>
#include <wil/resource.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An extension class for the <see cref="Storyboard"/> type.
    /// </summary>
    class StoryboardAnimations
    {
    public:
        /// <summary>
        /// Starts an animation and returns an async action that reports when it completes.
        /// </summary>
        /// <param name="storyboard">The target storyboard to start.</param>
        /// <returns>An <see cref="IAsyncAction"/> that completes when <paramref name="storyboard"/> completes.</returns>
        static winrt::Windows::Foundation::IAsyncAction BeginAsync(
            winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard const& storyboard)
        {
            wil::shared_event completionEvent(wil::EventOptions::ManualReset);
            winrt::event_token token = storyboard.Completed([=](auto&&...)
            {
                completionEvent.SetEvent();
            });

            winrt::apartment_context context;
            storyboard.Begin();
            co_await winrt::resume_on_signal(completionEvent.get());
            co_await context;
            storyboard.Completed(token);
        }
    };
}
