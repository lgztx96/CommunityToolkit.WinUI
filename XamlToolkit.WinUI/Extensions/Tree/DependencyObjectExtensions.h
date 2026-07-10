#pragma once

#include "Predicates.h"

#ifdef __INTELLISENSE__
#include <functional>
#include <vector>
#include <generator>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#else
import winrt.Windows.Foundation;
import winrt.Windows.UI.Xaml.Interop;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Media;
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI
{
	class DependencyObjectEx 
	{
	public:
		template <typename T, IPredicate<T> TPredicate>
		static T FindDescendant(winrt::DependencyObject const& element, TPredicate& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			int childrenCount = winrt::VisualTreeHelper::GetChildrenCount(element);

			for (int i = 0; i < childrenCount; i++)
			{
				winrt::DependencyObject child = winrt::VisualTreeHelper::GetChild(element, i);

				if (auto result = child.try_as<T>(); result && predicate(result))
				{
					return result;
				}

				if (T descendant = FindDescendant<T, TPredicate>(child, predicate))
				{
					return descendant;
				}
			}

			return nullptr;
		}

		template <typename T, typename TState>
		static T FindDescendant(winrt::DependencyObject const& element, TState&& state, const std::function<bool(T, TState&&)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindDescendant<T>(element, predicateByFunc);
		}

		template <typename T>
		static T FindDescendant(winrt::DependencyObject const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			PredicateByFunc<T, void> predicateByFunc(predicate);
			return FindDescendant<T>(element, predicateByFunc);
		}

		static winrt::DependencyObject FindDescendant(winrt::DependencyObject const& element, winrt::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindDescendant<winrt::DependencyObject, PredicateByType>(element, predicateByType);
		}

		template <typename T>
		static T FindDescendant(winrt::DependencyObject const& element)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			PredicateByAny<T> predicateByAny;

			return FindDescendant<T>(element, predicateByAny);
		}

		static winrt::FrameworkElement FindDescendant(winrt::DependencyObject const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindDescendant<winrt::FrameworkElement, PredicateByName>(element, predicateByName);
		}

		static winrt::FrameworkElement FindDescendantOrSelf(winrt::DependencyObject const& element, std::wstring_view name)
		{
			if (auto result = element.try_as<winrt::FrameworkElement>(); result && result.Name() == name)
			{
				return result;
			}

			return FindDescendant(element, name);
		}

		template <typename T>
		static T FindDescendantOrSelf(winrt::DependencyObject const& element)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result)
			{
				return result;
			}

			return FindDescendant<T>(element);
		}

		static winrt::DependencyObject FindDescendantOrSelf(winrt::DependencyObject const& element, winrt::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindDescendant(element, type);
		}

		template <typename T>
		static T FindDescendantOrSelf(winrt::DependencyObject const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindDescendant(element, predicate);
		}

		template <typename T, typename TState>
		static T FindDescendantOrSelf(winrt::DependencyObject const& element, TState&& state, const std::function<bool(T, TState&&)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindDescendant(element, state, predicate);
		}

		static void FindDescendants(winrt::DependencyObject const& element, std::vector<winrt::DependencyObject>& result)
		{
			int childrenCount = winrt::VisualTreeHelper::GetChildrenCount(element);

			for (auto i = 0; i < childrenCount; i++)
			{
				winrt::DependencyObject child = winrt::VisualTreeHelper::GetChild(element, i);

				result.emplace_back(child);

				FindDescendants(child, result);
			}
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindAscendant(winrt::DependencyObject element, TPredicate& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			while (true)
			{
				winrt::DependencyObject parent = winrt::VisualTreeHelper::GetParent(element);

				if (parent == nullptr)
				{
					return nullptr;
				}

				if (auto result = parent.try_as<T>(); result && predicate(result))
				{
					return result;
				}

				element = parent;
			}
		}

		template<typename T, typename TState>
		static T FindAscendant(winrt::DependencyObject const& element, TState&& state, const std::function<bool(T, TState&&)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindAscendant<T>(element, predicateByFunc);
		}

		template<typename T>
		static T FindAscendant(winrt::DependencyObject const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			PredicateByFunc<T, void> predicateByFunc(predicate);

			return FindAscendant<T>(element, predicateByFunc);
		}

		static winrt::DependencyObject FindAscendant(winrt::DependencyObject const& element, winrt::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindAscendant<winrt::DependencyObject, PredicateByType>(element, predicateByType);
		}

		template <typename T>
		static T FindAscendant(winrt::DependencyObject const& element)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			PredicateByAny<T> predicateByAny;

			return FindAscendant<T, PredicateByAny<T>>(element, predicateByAny);
		}

		static winrt::FrameworkElement FindAscendant(winrt::DependencyObject const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindAscendant<winrt::FrameworkElement, PredicateByName>(element, predicateByName);
		}

		static winrt::FrameworkElement FindAscendantOrSelf(winrt::DependencyObject const& element, std::wstring_view name)
		{
			if (auto result = element.try_as<winrt::FrameworkElement>(); result && name == result.Name())
			{
				return result;
			}

			return FindAscendant(element, name);
		}

		template<typename T>
		static T FindAscendantOrSelf(winrt::DependencyObject const& element)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result)
			{
				return result;
			}

			return FindAscendant<T>(element);
		}

		static winrt::DependencyObject FindAscendantOrSelf(winrt::DependencyObject const& element, winrt::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindAscendant(element, type);
		}

		template<typename T>
		static T FindAscendantOrSelf(winrt::DependencyObject const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindAscendant(element, predicate);
		}

		template<typename T, typename TState>
		static T FindAscendantOrSelf(winrt::DependencyObject const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindAscendant(element, state, predicate);
		}

		static std::generator<winrt::DependencyObject> FindAscendants(winrt::DependencyObject element)
		{
			std::vector<winrt::DependencyObject> result;

			while (true)
			{
				winrt::DependencyObject parent = winrt::VisualTreeHelper::GetParent(element);

				if (parent == nullptr)
				{
					co_return;
				}

				co_yield parent;

				element = parent;
			}
		}
	};
}
