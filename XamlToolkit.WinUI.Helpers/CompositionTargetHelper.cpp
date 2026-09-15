#include "pch.h"
#include "winrt_module_imports.h"
#include "CompositionTargetHelper.h"
#if __has_include("CompositionTargetHelper.g.cpp")
#include "CompositionTargetHelper.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_helpers.h>
#include <wil/resource.h>
#else
import winrt.Microsoft.UI.Dispatching;
#endif

namespace winrt 
{
    using namespace ::winrt::Microsoft::UI::Dispatching;
    using namespace ::winrt::Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    winrt::Windows::Foundation::IAsyncOperation<bool>
        CompositionTargetHelper::ExecuteAfterCompositionRenderingAsync(
            CompositionRenderingCallback callback)
    {
        if (!callback)
        {
            throw winrt::hresult_invalid_argument{ L"callback" };
        }

        auto completionEvent = wil::shared_event(wil::EventOptions::ManualReset);
        auto renderingEventToken = std::make_shared<winrt::event_token>();

        auto dispatcherQueue = winrt::DispatcherQueue::GetForCurrentThread();

        try
        {
            *renderingEventToken = winrt::CompositionTarget::Rendering(
                [completionEvent, renderingEventToken, callback](auto&&...)
                {
                    // Detach event or Rendering will keep calling us back.
                    winrt::CompositionTarget::Rendering(*renderingEventToken);

                    auto signalOnExit = wil::scope_exit([&] { completionEvent.SetEvent(); });

                    callback();
                });
        }
        catch (winrt::hresult_error const& e)
        {
            // DirectUI::CompositionTarget::add_Rendering can fail with RPC_E_WRONG_THREAD
            // if called while the Xaml Core is being shutdown...
            // Since the core is being shutdown, we no longer care about whatever work
            // we wanted to defer to CT.Rendering, so ignore this error.
            if (e.code() != RPC_E_WRONG_THREAD)
            {
                throw;
            }

            co_return false;
        }

        co_await winrt::resume_on_signal(completionEvent.get());

        co_await wil::resume_foreground(dispatcherQueue);

        co_return true;
    }
}
