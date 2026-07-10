#pragma once

#include "Predicates.h"

#ifdef __INTELLISENSE__
#include <vector>
#include <generator>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#else
import winrt.Windows.Foundation;
import winrt.Windows.UI.Xaml.Interop;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Controls;
import winrt.Microsoft.UI.Xaml.Markup;
import winrt.Microsoft.UI.Xaml.Media;
#endif

#ifdef FindResource
#undef FindResource
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Markup;
}

namespace winrt::XamlToolkit::WinUI
{
	class FrameworkElementEx
	{
	public:
		static winrt::FrameworkElement FindChild(winrt::FrameworkElement const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindChild<winrt::FrameworkElement, PredicateByName>(element, predicateByName);
		}

		template <typename T>
		static T FindChild(winrt::FrameworkElement const& element)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByAny<T> predicateByAny;

			return FindChild<T, PredicateByAny<T>>(element, predicateByAny);
		}

		static winrt::FrameworkElement FindChild(winrt::FrameworkElement const& element, winrt::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindChild<winrt::FrameworkElement, PredicateByType>(element, predicateByType);
		}

		template <typename T>
		static T FindChild(winrt::FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByFunc<T> predicateByFunc(predicate);

			return FindChild<T, PredicateByFunc<T>>(element, predicateByFunc);
		}

		template <typename T, typename TState>
		static T FindChild(winrt::FrameworkElement const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindChild<T, PredicateByFunc<T, TState>>(element, predicateByFunc);
		}

		template <typename T, IPredicate<T> TPredicate>
		static T FindChild(winrt::FrameworkElement element, TPredicate predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			// Jump label to manually optimize the tail recursive paths for elements with a single
			// child by just overwriting the current element and jumping back to the start of the
			// method. This avoids a recursive call and one stack frame every time.
		Start:

			if (auto panel = element.try_as<Panel>())
			{
				for (const auto& child : panel.Children())
				{
					auto current = child.try_as<winrt::FrameworkElement>();
					if (!current)
					{
						continue;
					}

					if (auto result = child.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					T descendant = FindChild<T, TPredicate>(current, predicate);

					if (descendant)
					{
						return descendant;
					}
				}
			}
			else if (auto itemsControl = element.try_as<winrt::ItemsControl>())
			{
				for (const auto& item : itemsControl.Items())
				{
					auto current = item.try_as<winrt::FrameworkElement>();
					if (!current)
					{
						continue;
					}

					if (auto result = item.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					T descendant = FindChild<T, TPredicate>(current, predicate);

					if (descendant)
					{
						return descendant;
					}
				}
			}
			else if (auto contentControl = element.try_as<winrt::ContentControl>())
			{
				if (auto content = contentControl.Content().try_as<winrt::FrameworkElement>())
				{
					if (auto result = content.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = content;

					goto Start;
				}
			}
			else if (auto border = element.try_as<winrt::Border>())
			{
				if (auto child = border.Child().try_as<winrt::FrameworkElement>())
				{
					if (auto result = child.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = child;

					goto Start;
				}
			}
			else if (auto contentPresenter = element.try_as<winrt::ContentPresenter>())
			{
				// Sometimes ContentPresenter is used in control templates instead of ContentControl,
				// therefore we should still check if its Content is a matching FrameworkElement instance.
				// This also makes work for SwitchPresenter.
				if (auto content = contentPresenter.Content().try_as<winrt::FrameworkElement>())
				{
					if (auto result = content.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = content;

					goto Start;
				}
			}
			else if (auto viewbox = element.try_as<winrt::Viewbox>())
			{
				if (auto child = viewbox.Child().try_as<winrt::FrameworkElement>())
				{
					if (auto result = child.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = child;

					goto Start;
				}
			}
			else if (auto userControl = element.try_as<winrt::UserControl>())
			{
				// We put UserControl right before the slower reflection fallback path as
				// type is less likely to be used compared to the other ones above.
				if (auto content = userControl.Content().try_as<winrt::FrameworkElement>())
				{
					if (auto result = content.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = content;

					goto Start;
				}
			}
			else if (auto containedControl = GetContentControl(element).try_as<winrt::FrameworkElement>())
			{
				if (auto result = containedControl.try_as<T>(); result && predicate(result))
				{
					return result;
				}

				element = containedControl;

				goto Start;
			}

			return nullptr;
		}

		static winrt::FrameworkElement FindChildOrSelf(winrt::FrameworkElement const& element, std::wstring_view name)
		{
			if (name == element.Name())
			{
				return element;
			}

			return FindChild(element, name);
		}

		template<typename T>
		static T FindChildOrSelf(winrt::FrameworkElement const& element)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>())
			{
				return result;
			}

			return FindChild<T>(element);
		}

		static winrt::FrameworkElement FindChildOrSelf(winrt::FrameworkElement const& element, winrt::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindChild(element, type);
		}

		template<typename T>
		static T FindChildOrSelf(winrt::FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindChild(element, predicate);
		}

		template<typename T, typename TState>
		static T FindChildOrSelf(winrt::FrameworkElement const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindChild(element, state, predicate);
		}

		static void FindChildren(winrt::FrameworkElement element, std::vector<winrt::FrameworkElement>& vec)
		{
		Start:

			if (auto panel = element.try_as<Panel>())
			{
				for (const auto& child : panel.Children())
				{
					auto current = child.try_as<winrt::FrameworkElement>();
					if (!current)
					{
						continue;
					}

					vec.emplace_back(current);

					std::vector<winrt::FrameworkElement> childrenOfChild;
					FindChildren(current, childrenOfChild);

					for (const auto& childOfChild : childrenOfChild)
					{
						vec.emplace_back(childOfChild);
					}
				}
			}
			else if (auto itemsControl = element.try_as<winrt::ItemsControl>())
			{
				for (const auto& item : itemsControl.Items())
				{
					auto current = item.try_as<winrt::FrameworkElement>();
					if (!current)
					{
						continue;
					}

					vec.emplace_back(current);

					std::vector<winrt::FrameworkElement> childrenOfChild;
					FindChildren(current, childrenOfChild);

					for (const auto& childOfChild : childrenOfChild)
					{
						vec.emplace_back(childOfChild);
					}
				}
			}
			else if (auto contentControl = element.try_as<winrt::ContentControl>())
			{
				if (auto content = contentControl.Content().try_as<winrt::FrameworkElement>())
				{
					vec.emplace_back(content);

					element = content;

					goto Start;
				}
			}
			else if (auto border = element.try_as<winrt::Border>())
			{
				if (auto child = border.Child().try_as<winrt::FrameworkElement>())
				{
					vec.emplace_back(child);

					element = child;

					goto Start;
				}
			}
			else if (auto contentPresenter = element.try_as<winrt::ContentPresenter>())
			{
				if (auto content = contentPresenter.Content().try_as<winrt::FrameworkElement>())
				{
					vec.emplace_back(content);

					element = content;

					goto Start;
				}
			}
			else if (auto viewbox = element.try_as<winrt::Viewbox>())
			{
				if (auto child = viewbox.Child().try_as<winrt::FrameworkElement>())
				{
					vec.emplace_back(child);

					element = child;

					goto Start;
				}
			}
			else if (auto userControl = element.try_as<winrt::UserControl>())
			{
				if (auto content = userControl.Content().try_as<winrt::FrameworkElement>())
				{
					vec.emplace_back(content);

					element = content;

					goto Start;
				}
			}
			else if (auto containedControl = GetContentControl(element).try_as<winrt::FrameworkElement>())
			{
				vec.emplace_back(containedControl);

				element = containedControl;

				goto Start;
			}
		}

		static winrt::FrameworkElement FindParent(winrt::FrameworkElement const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindParent<winrt::FrameworkElement, PredicateByName>(element, predicateByName);
		}

		template<typename T>
		static T FindParent(winrt::FrameworkElement element)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByAny<T> predicateByAny;

			return FindParent<T, PredicateByAny<T>>(element, predicateByAny);
		}

		static winrt::FrameworkElement FindParent(winrt::FrameworkElement const& element, winrt::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindParent<winrt::FrameworkElement, PredicateByType>(element, predicateByType);
		}

		template<typename T>
		static T FindParent(winrt::FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByFunc<T> predicateByFunc(predicate);

			return FindParent<T, PredicateByFunc<T>>(element, predicateByFunc);
		}

		template<typename T, typename TState>
		static T FindParent(winrt::FrameworkElement const& element, TState state, std::function<bool(T, TState)> predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindParent<T, PredicateByFunc<T, TState>>(element, predicateByFunc);
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindParent(winrt::FrameworkElement element, TPredicate predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			while (true)
			{
				auto parent = element.Parent().try_as<winrt::FrameworkElement>();

				if (!parent)
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

		static winrt::FrameworkElement FindParentOrSelf(winrt::FrameworkElement const& element, std::wstring_view name)
		{
			if (name == element.Name())
			{
				return element;
			}

			return FindParent(element, name);
		}

		template<typename T>
		static T FindParentOrSelf(winrt::FrameworkElement const& element)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>())
			{
				return result;
			}

			return FindParent<T>(element);
		}

		static winrt::FrameworkElement FindParentOrSelf(winrt::FrameworkElement const& element, winrt::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindParent(element, type);
		}

		template<typename T>
		static T FindParentOrSelf(winrt::FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindParent(element, predicate);
		}

		template<typename T, typename TState>
		static T FindParentOrSelf(winrt::FrameworkElement const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindParent(element, state, predicate);
		}

		static std::generator<winrt::FrameworkElement> FindParents(winrt::FrameworkElement element)
		{
			while (true)
			{
				auto parent = element.Parent().try_as<winrt::FrameworkElement>();

				if (parent == nullptr)
				{
					co_return;
				}

				co_yield parent;

				element = parent;
			}
		}

		/// <summary>
		/// Gets the content property of element as defined by <see cref="ContentPropertyAttribute"/>, if available.
		/// </summary>
		/// <param name="element">The parent element.</param>
		/// <returns>The retrieved content control, or <see langword="null"/> if not available.</returns>
		static winrt::UIElement GetContentControl(winrt::FrameworkElement const& element)
		{
			winrt::Application app = Application::Current();

			if (!app)
			{
				return nullptr;
			}

			winrt::IXamlMetadataProvider provider = app.try_as<winrt::IXamlMetadataProvider>();
			if (!provider)
			{
				return nullptr;
			}

			auto type = winrt::get_class_name(element);

			winrt::IXamlType xamlType = provider.GetXamlType(type);
			if (!xamlType) 
			{
				return nullptr;
			}

			while (xamlType)
			{
				// We need to manually explore the custom attributes this way as the target one
				// is not returned by any of the other available GetCustomAttribute<T> APIs.
				if (winrt::IXamlMember member = xamlType.ContentProperty())
				{
					return member.GetValue(element).try_as<winrt::UIElement>();
				}

				xamlType = xamlType.BaseType();
			}

			return nullptr;
		}

		static winrt::IInspectable FindResource(winrt::FrameworkElement const& element, winrt::IInspectable resourceKey)
		{
			winrt::IInspectable value{ nullptr };
			if (TryFindResource(element, resourceKey, value))
			{
				return value;
			}

			throw winrt::hresult_out_of_bounds(winrt::format(L"No resource was found with the key {}", winrt::get_class_name(resourceKey)));
		}

		static winrt::IInspectable TryFindResource(winrt::FrameworkElement const& element, winrt::IInspectable resourceKey)
		{
			winrt::FrameworkElement current = element;

			// Look in our dictionary and then walk-up parents. We use a do-while loop here
			// so that an implicit NRE will be thrown at the first iteration in case the
			// input element is null. This is consistent with the other extensions.
			do
			{
				if (auto value = current.Resources().TryLookup(resourceKey))
				{
					return value;
				}

				current = current.Parent().try_as<winrt::FrameworkElement>();
			} while (current);

			// Finally try application resources
			return winrt::Application::Current().Resources().TryLookup(resourceKey);
		}

		static bool TryFindResource(winrt::FrameworkElement const& element, winrt::IInspectable const& resourceKey, winrt::IInspectable& value)
		{
			return static_cast<bool>((value = TryFindResource(element, resourceKey)));
		}
	};
}

