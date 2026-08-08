#pragma once

#if defined(__INTELLISENSE__) || !defined(WINRT_IMPORT_MODULE)
#include <concepts>
#include <functional>
#include <string_view>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#else
import std;
import winrt.Windows.Foundation;
import winrt.Windows.UI.Xaml.Interop;
import winrt.Microsoft.UI.Xaml;
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::UI::Xaml::Interop;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt
{
    template <typename D, typename B>
    concept derived_from = std::is_base_of<winrt::impl::base_one<D, B>, D>::value || std::is_base_of<B, D>::value;

    template <typename D, typename IB>
    concept derived_from_interface = std::is_base_of<winrt::impl::require_one<D, IB>, D>::value;

    //https://github.com/microsoft/cppwinrt/issues/609
    template <typename D, typename B>
    concept is_derived_from = std::derived_from<D, winrt::impl::base_one<D, B>>;
}

namespace winrt::XamlToolkit::WinUI
{
    template <class F, typename Ret, class... Args> 
    concept Callable = std::invocable<F, Args...> && std::same_as<Ret, std::invoke_result_t<F, Args...>>;

    template <typename T, typename Arg>
    concept IPredicate = Callable<T, bool, Arg>;

    struct PredicateByName
    {
        std::wstring_view name;

        inline bool operator()(winrt::FrameworkElement const& element) const
        {
            return name == element.Name();
        }
    };

    struct PredicateByType
    {
        winrt::TypeName type;

        inline bool operator()(winrt::IInspectable const& element) const 
        {
            return winrt::get_class_name(element) == type.Name;
        }
    };

    template <typename T>
    struct PredicateByAny 
    {
        constexpr bool operator()([[maybe_unused]] const T& element) const noexcept 
        {
            return true;
        }
    };
}
