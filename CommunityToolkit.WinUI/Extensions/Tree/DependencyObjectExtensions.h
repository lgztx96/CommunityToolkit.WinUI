#pragma once

#include "Predicates.h"
#include <functional>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <generator>

namespace winrt::CommunityToolkit::WinUI
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;

	class DependencyObjectEx {
	public:
		template <typename T, IPredicate<T> TPredicate>
		static T FindDescendant(DependencyObject const& element, TPredicate& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			int childrenCount = VisualTreeHelper::GetChildrenCount(element);

			for (int i = 0; i < childrenCount; i++)
			{
				DependencyObject child = VisualTreeHelper::GetChild(element, i);

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
		static T FindDescendant(DependencyObject const& element, TState&& state, const std::function<bool(T, TState&&)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindDescendant<T>(element, predicateByFunc);
		}

		template <typename T>
		static T FindDescendant(DependencyObject const& element, std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			return FindDescendant(element, predicate);
		}

		static DependencyObject FindDescendant(DependencyObject const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindDescendant<DependencyObject, PredicateByType>(element, predicateByType);
		}

		template <typename T>
		static T FindDescendant(DependencyObject const& element)
			requires winrt::derived_from<T, DependencyObject>
		{
			PredicateByAny<T> predicateByAny;

			return FindDescendant<T>(element, predicateByAny);
		}

		static FrameworkElement FindDescendant(DependencyObject const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindDescendant<FrameworkElement, PredicateByName>(element, predicateByName);
		}

		static FrameworkElement FindDescendantOrSelf(DependencyObject const& element, std::wstring_view name)
		{
			if (auto result = element.try_as<FrameworkElement>(); result && result.Name() == name)
			{
				return result;
			}

			return FindDescendant(element, name);
		}

		template <typename T>
		static T FindDescendantOrSelf(DependencyObject const& element)
			requires winrt::derived_from<T, DependencyObject>
		{
			if (auto result = element.try_as<T>(); result)
			{
				return result;
			}

			return FindDescendant<T>(element);
		}

		static DependencyObject FindDescendantOrSelf(DependencyObject const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindDescendant(element, type);
		}

		template <typename T>
		static T FindDescendantOrSelf(DependencyObject const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindDescendant(element, predicate);
		}

		template <typename T, typename TState>
		static T FindDescendantOrSelf(DependencyObject const& element, TState&& state, const std::function<bool(T, TState&&)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindDescendant(element, state, predicate);
		}

		static void FindDescendants(DependencyObject const& element, std::vector<DependencyObject>& result)
		{
			int childrenCount = VisualTreeHelper::GetChildrenCount(element);

			for (auto i = 0; i < childrenCount; i++)
			{
				DependencyObject child = VisualTreeHelper::GetChild(element, i);

				result.push_back(child);

				std::vector<DependencyObject> childrenOfChild;

				FindDescendants(child, childrenOfChild);

				result.append_range(childrenOfChild);
			}
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindAscendant(DependencyObject element, TPredicate& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			while (true)
			{
				DependencyObject parent = VisualTreeHelper::GetParent(element);

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
		static T FindAscendant(DependencyObject const& element, TState&& state, const std::function<bool(T, TState&&)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindAscendant<T>(element, predicateByFunc);
		}

		template<typename T>
		T FindAscendant(DependencyObject const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			return FindAscendant<T>(element, predicate);
		}

		static DependencyObject FindAscendant(DependencyObject const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindAscendant<DependencyObject, PredicateByType>(element, predicateByType);
		}

		template <typename T>
		static T FindAscendant(DependencyObject const& element)
			requires winrt::derived_from<T, DependencyObject>
		{
			PredicateByAny<T> predicateByAny;

			return FindAscendant<T, PredicateByAny<T>>(element, predicateByAny);
		}

		static FrameworkElement FindAscendant(DependencyObject const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindAscendant<FrameworkElement, PredicateByName>(element, predicateByName);
		}

		static FrameworkElement FindAscendantOrSelf(DependencyObject const& element, std::wstring_view name)
		{
			if (auto result = element.try_as<FrameworkElement>(); result && name == result.Name())
			{
				return result;
			}

			return FindAscendant(element, name);
		}

		template<typename T>
		static T FindAscendantOrSelf(DependencyObject const& element)
			requires winrt::derived_from<T, DependencyObject>
		{
			if (auto result = element.try_as<T>(); result)
			{
				return result;
			}

			return FindAscendant<T>(element);
		}

		static DependencyObject FindAscendantOrSelf(DependencyObject const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindAscendant(element, type);
		}

		template<typename T>
		static T FindAscendantOrSelf(DependencyObject const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindAscendant(element, predicate);
		}

		template<typename T, typename TState>
		static T FindAscendantOrSelf(DependencyObject const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindAscendant(element, state, predicate);
		}

		static std::generator<DependencyObject> FindAscendants(DependencyObject element)
		{
			std::vector<DependencyObject> result;

			while (true)
			{
				DependencyObject parent = VisualTreeHelper::GetParent(element);

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
