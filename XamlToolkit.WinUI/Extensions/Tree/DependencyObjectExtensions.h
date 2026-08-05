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
		static T FindDescendant(winrt::DependencyObject const& element, TPredicate&& predicate)
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

				if (T descendant = FindDescendant<T>(child, predicate))
				{
					return descendant;
				}
			}

			return nullptr;
		}

		static winrt::DependencyObject FindDescendant(winrt::DependencyObject const& element, winrt::TypeName const& type)
		{
			PredicateByType predicateByType(type);

			return FindDescendant<winrt::DependencyObject>(element, predicateByType);
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

			return FindDescendant<winrt::FrameworkElement>(element, predicateByName);
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

		static winrt::DependencyObject FindDescendantOrSelf(winrt::DependencyObject const& element, winrt::TypeName const& type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindDescendant(element, type);
		}

		template <typename T, IPredicate<T> TPredicate>
		static T FindDescendantOrSelf(winrt::DependencyObject const& element, TPredicate&& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindDescendant<T>(element, predicate);
		}

		static std::generator<winrt::DependencyObject> FindDescendants(winrt::DependencyObject const& element)
		{
			int childrenCount = winrt::VisualTreeHelper::GetChildrenCount(element);

			for (int i = 0; i < childrenCount; i++)
			{
				auto child = winrt::VisualTreeHelper::GetChild(element, i);

				co_yield child;

				for (const auto& descendant : FindDescendants(child))
				{
					co_yield descendant;
				}
			}
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindAscendant(winrt::DependencyObject element, TPredicate&& predicate)
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

		static winrt::DependencyObject FindAscendant(winrt::DependencyObject const& element, winrt::TypeName const& type)
		{
			PredicateByType predicateByType(type);

			return FindAscendant<winrt::DependencyObject>(element, predicateByType);
		}

		template <typename T>
		static T FindAscendant(winrt::DependencyObject const& element)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			PredicateByAny<T> predicateByAny;

			return FindAscendant<T>(element, predicateByAny);
		}

		static winrt::FrameworkElement FindAscendant(winrt::DependencyObject const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindAscendant<winrt::FrameworkElement>(element, predicateByName);
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
			if (auto result = element.try_as<T>())
			{
				return result;
			}

			return FindAscendant<T>(element);
		}

		static winrt::DependencyObject FindAscendantOrSelf(winrt::DependencyObject const& element, winrt::TypeName const& type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindAscendant(element, type);
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindAscendantOrSelf(winrt::DependencyObject const& element, TPredicate&& predicate)
			requires winrt::derived_from<T, winrt::DependencyObject>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindAscendant<T>(element, predicate);
		}

		static std::generator<winrt::DependencyObject> FindAscendants(winrt::DependencyObject element)
		{
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
