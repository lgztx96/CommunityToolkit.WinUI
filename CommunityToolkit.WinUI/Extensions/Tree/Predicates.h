#pragma once

#include <functional>
#include <string_view>
#include <winrt/Microsoft.UI.Xaml.h>

namespace winrt 
{
    template <typename D, typename B>
    concept derived_from = std::is_base_of<impl::base_one<D, B>, D>::value || std::is_base_of<B, D>::value;

    template <typename D, typename IB>
    concept derived_from_interface = std::is_base_of<impl::require_one<D, IB>, D>::value;

    //https://github.com/microsoft/cppwinrt/issues/609
    template <typename D, typename B>
    concept is_derived_from = std::derived_from<D, winrt::impl::base_one<D, B>>;
}

namespace winrt::CommunityToolkit::WinUI
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;

    template <class F, typename Ret, class... Args> 
    concept Callable = std::invocable<F, Args...> && std::same_as<Ret, std::invoke_result_t<F, Args...>>;

    template <typename T, typename Arg>
    concept IPredicate = Callable<T, bool, Arg>;

    struct PredicateByName
    {
        std::wstring_view name;

        inline bool operator()(FrameworkElement const& element) const {
            return name == element.Name();
        }
    };

    struct PredicateByType
    {
        winrt::Windows::UI::Xaml::Interop::TypeName type;

        inline bool operator()(Windows::Foundation::IInspectable const& element) const {
            return winrt::get_class_name(element) == type.Name;
        }
    };

    template <typename T>
    struct PredicateByAny {

        constexpr bool operator()([[maybe_unused]] const T& element) const noexcept {
            return true;
        }
    };

    template <typename T, typename TState>
    struct PredicateByFunc
    {
        /// <summary>
        /// The state to give as input to <see name="predicate"/>.
        /// </summary>
    private:
        TState& state;

        /// <summary>
        /// The predicatee to use to match items.
        /// </summary>
    private:
        const std::function<bool(T, TState&&)>& predicate;

        /// <summary>
        /// Initializes a new instance of the <see cref="PredicateByFunc{T, TState}"/> struct.
        /// </summary>
        /// <param name="state">The state to give as input to <paramref name="predicate"/>.</param>
        /// <param name="predicate">The predicatee to use to match items.</param>
    public:
        PredicateByFunc(TState state, std::function<bool(T, TState&&)>& predicate) : state(state), predicate(predicate) { }

        inline bool operator()(T element)
        {
            return predicate(element, state);
        }
    };
}