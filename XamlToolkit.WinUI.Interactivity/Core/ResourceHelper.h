#pragma once

#ifdef __INTELLISENSE__
#include <format>
#include <string_view>
#include <winrt/Windows.Foundation.h>
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    /// <summary>
    /// A helper class that provides access to localized resource strings.
    /// </summary>
    struct ResourceHelper
    {
        /// <summary>
        /// Gets the string resource with the specified name.
        /// </summary>
        /// <param name="resourceName">The name of the resource string to retrieve.</param>
        /// <returns>The localized string resource.</returns>
        static winrt::hstring GetString(std::wstring_view resourceName);

        static winrt::hstring CallMethodActionValidMethodNotFoundExceptionMessage();
        static winrt::hstring ChangePropertyActionCannotFindPropertyNameExceptionMessage();
        static winrt::hstring ChangePropertyActionCannotSetValueExceptionMessage();
        static winrt::hstring ChangePropertyActionPropertyIsReadOnlyExceptionMessage();
        static winrt::hstring GoToStateActionTargetHasNoStateGroups();
        static winrt::hstring CannotAttachBehaviorMultipleTimesExceptionMessage();
        static winrt::hstring CannotFindEventNameExceptionMessage();
        static winrt::hstring InvalidAssociatedObjectExceptionMessage();
        static winrt::hstring NonActionAddedToActionCollectionExceptionMessage();
        static winrt::hstring NonBehaviorAddedToBehaviorCollectionExceptionMessage();
        static winrt::hstring DuplicateBehaviorInCollectionExceptionMessage();
        static winrt::hstring InvalidLeftOperand();
        static winrt::hstring InvalidRightOperand();
        static winrt::hstring InvalidOperands();

        template<typename... Args>
        static winrt::hstring Format(std::wstring_view fmt, Args&&... args)
        {
            return winrt::hstring(std::vformat(fmt, std::make_wformat_args(args...)));
        }
    };
}
