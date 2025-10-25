#pragma once

#include "Predicates.h"
#include <functional>
#include <generator>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>

namespace winrt::CommunityToolkit::WinUI
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Controls;

	class FrameworkElementEx
	{
	public:
		static FrameworkElement FindChild(FrameworkElement const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindChild<FrameworkElement, PredicateByName>(element, predicateByName);
		}

		template <typename T>
		static T FindChild(FrameworkElement const& element)
			requires winrt::derived_from<T, FrameworkElement>
		{
			PredicateByAny<T> predicateByAny;

			return FindChild<T, PredicateByAny<T>>(element, predicateByAny);
		}

		static FrameworkElement FindChild(FrameworkElement const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindChild<FrameworkElement, PredicateByType>(element, predicateByType);
		}

		template <typename T>
		static T FindChild(FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			PredicateByFunc<T> predicateByFunc(predicate);

			return FindChild<T, PredicateByFunc<T>>(element, predicateByFunc);
		}

		template <typename T, typename TState>
		static T FindChild(FrameworkElement const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindChild<T, PredicateByFunc<T, TState>>(element, predicateByFunc);
		}

		template <typename T, IPredicate<T> TPredicate>
		static T FindChild(FrameworkElement element, TPredicate predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			// Jump label to manually optimize the tail recursive paths for elements with a single
			// child by just overwriting the current element and jumping back to the start of the
			// method. This avoids a recursive call and one stack frame every time.
		Start:

			if (auto panel = element.try_as<Panel>())
			{
				for (const auto& child : panel.Children())
				{
					auto current = child.try_as<FrameworkElement>();
					if (!current)
					{
						continue;
					}

					if (auto result = child.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					T descendant = FindChild<T, TPredicate>(current, predicate);

					if (descendant != nullptr)
					{
						return descendant;
					}
				}
			}
			else if (auto itemsControl = element.try_as<ItemsControl>())
			{
				for (Windows::Foundation::IInspectable item : itemsControl.Items())
				{
					auto current = item.try_as<FrameworkElement>();
					if (!current)
					{
						continue;
					}

					if (auto result = item.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					T descendant = FindChild<T, TPredicate>(current, predicate);

					if (descendant != nullptr)
					{
						return descendant;
					}
				}
			}
			else if (auto contentControl = element.try_as<ContentControl>())
			{
				if (auto content = contentControl.Content().try_as<FrameworkElement>())
				{
					if (auto result = content.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = content;

					goto Start;
				}
			}
			else if (auto border = element.try_as<Border>())
			{
				if (auto child = border.Child().try_as<FrameworkElement>())
				{
					if (auto result = child.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = child;

					goto Start;
				}
			}
			else if (auto contentPresenter = element.try_as<ContentPresenter>())
			{
				// Sometimes ContentPresenter is used in control templates instead of ContentControl,
				// therefore we should still check if its Content is a matching FrameworkElement instance.
				// This also makes work for SwitchPresenter.
				if (auto content = contentPresenter.Content().try_as<FrameworkElement>())
				{
					if (auto result = content.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = content;

					goto Start;
				}
			}
			else if (auto viewbox = element.try_as<Viewbox>())
			{
				if (auto child = viewbox.Child().try_as<FrameworkElement>())
				{
					if (auto result = child.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = child;

					goto Start;
				}
			}
			else if (auto userControl = element.try_as<UserControl>())
			{
				// We put UserControl right before the slower reflection fallback path as
				// type is less likely to be used compared to the other ones above.
				if (auto content = userControl.Content().try_as<FrameworkElement>())
				{
					if (auto result = content.try_as<T>(); result && predicate(result))
					{
						return result;
					}

					element = content;

					goto Start;
				}
			}
			else if (auto containedControl = GetContentControl(element).try_as<FrameworkElement>())
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

		static FrameworkElement FindChildOrSelf(FrameworkElement const& element, std::wstring_view name)
		{
			if (name == element.Name())
			{
				return element;
			}

			return FindChild(element, name);
		}

		template<typename T>
		static T FindChildOrSelf(FrameworkElement const& element)
			requires winrt::derived_from<T, FrameworkElement>
		{
			if (auto result = element.try_as<T>())
			{
				return result;
			}

			return FindChild<T>(element);
		}

		static FrameworkElement FindChildOrSelf(FrameworkElement const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindChild(element, type);
		}

		template<typename T>
		static T FindChildOrSelf(FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindChild(element, predicate);
		}

		template<typename T, typename TState>
		static T FindChildOrSelf(FrameworkElement const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindChild(element, state, predicate);
		}

		static void FindChildren(FrameworkElement element, std::vector<FrameworkElement>& vec)
		{
		Start:

			if (auto panel = element.try_as<Panel>())
			{
				for (const auto& child : panel.Children())
				{
					auto current = child.try_as<FrameworkElement>();
					if (!current)
					{
						continue;
					}

					vec.emplace_back(current);

					std::vector<FrameworkElement> childrenOfChild;
					FindChildren(current, childrenOfChild);

					for (FrameworkElement childOfChild : childrenOfChild)
					{
						vec.emplace_back(childOfChild);
					}
				}
			}
			else if (auto itemsControl = element.try_as<ItemsControl>())
			{
				for (Windows::Foundation::IInspectable item : itemsControl.Items())
				{
					auto current = item.try_as<FrameworkElement>();
					if (!current)
					{
						continue;
					}

					vec.emplace_back(current);

					std::vector<FrameworkElement> childrenOfChild;
					FindChildren(current, childrenOfChild);

					for (const auto& childOfChild : childrenOfChild)
					{
						vec.emplace_back(childOfChild);
					}
				}
			}
			else if (auto contentControl = element.try_as<ContentControl>())
			{
				if (auto content = contentControl.Content().try_as<FrameworkElement>())
				{
					vec.emplace_back(content);

					element = content;

					goto Start;
				}
			}
			else if (auto border = element.try_as<Border>())
			{
				if (auto child = border.Child().try_as<FrameworkElement>())
				{
					vec.emplace_back(child);

					element = child;

					goto Start;
				}
			}
			else if (auto contentPresenter = element.try_as<ContentPresenter>())
			{
				if (auto content = contentPresenter.Content().try_as<FrameworkElement>())
				{
					vec.emplace_back(content);

					element = content;

					goto Start;
				}
			}
			else if (auto viewbox = element.try_as<Viewbox>())
			{
				if (auto child = viewbox.Child().try_as<FrameworkElement>())
				{
					vec.emplace_back(child);

					element = child;

					goto Start;
				}
			}
			else if (auto userControl = element.try_as<UserControl>())
			{
				if (auto content = userControl.Content().try_as<FrameworkElement>())
				{
					vec.emplace_back(content);

					element = content;

					goto Start;
				}
			}
			else if (auto containedControl = GetContentControl(element).try_as<FrameworkElement>())
			{
				vec.emplace_back(containedControl);

				element = containedControl;

				goto Start;
			}
		}

		static FrameworkElement FindParent(FrameworkElement const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindParent<FrameworkElement, PredicateByName>(element, predicateByName);
		}

		template<typename T>
		static T FindParent(FrameworkElement element)
			requires winrt::derived_from<T, FrameworkElement>
		{
			PredicateByAny<T> predicateByAny;

			return FindParent<T, PredicateByAny<T>>(element, predicateByAny);
		}

		static FrameworkElement FindParent(FrameworkElement const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			PredicateByType predicateByType(type);

			return FindParent<FrameworkElement, PredicateByType>(element, predicateByType);
		}

		template<typename T>
		static T FindParent(FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			PredicateByFunc<T> predicateByFunc(predicate);

			return FindParent<T, PredicateByFunc<T>>(element, predicateByFunc);
		}

		template<typename T, typename TState>
		static T FindParent(FrameworkElement const& element, TState state, std::function<bool(T, TState)> predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			PredicateByFunc<T, TState> predicateByFunc(state, predicate);

			return FindParent<T, PredicateByFunc<T, TState>>(element, predicateByFunc);
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindParent(FrameworkElement element, TPredicate predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			while (true)
			{
				auto parent = element.Parent().try_as<FrameworkElement>();

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

		static FrameworkElement FindParentOrSelf(FrameworkElement const& element, std::wstring_view name)
		{
			if (name == element.Name())
			{
				return element;
			}

			return FindParent(element, name);
		}

		template<typename T>
		static T FindParentOrSelf(FrameworkElement const& element)
			requires winrt::derived_from<T, FrameworkElement>
		{
			if (auto result = element.try_as<T>())
			{
				return result;
			}

			return FindParent<T>(element);
		}

		static FrameworkElement FindParentOrSelf(FrameworkElement const& element, winrt::Windows::UI::Xaml::Interop::TypeName type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindParent(element, type);
		}

		template<typename T>
		static T FindParentOrSelf(FrameworkElement const& element, const std::function<bool(T)>& predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindParent(element, predicate);
		}

		template<typename T, typename TState>
		static T FindParentOrSelf(FrameworkElement const& element, TState state, const std::function<bool(T, TState)>& predicate)
			requires winrt::derived_from<T, FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result, state))
			{
				return result;
			}

			return FindParent(element, state, predicate);
		}

		static std::generator<FrameworkElement> FindParents(FrameworkElement element)
		{
			while (true)
			{
				auto parent = element.Parent().try_as<FrameworkElement>();

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
		static UIElement GetContentControl(FrameworkElement const& element)
		{
			Application app = Application::Current();

			if (!app) return nullptr;

			winrt::Microsoft::UI::Xaml::Markup::IXamlMetadataProvider provider = app.try_as<
				winrt::Microsoft::UI::Xaml::Markup::IXamlMetadataProvider>();
			if (!provider) return nullptr;

			auto type = winrt::get_class_name(element);

			winrt::Microsoft::UI::Xaml::Markup::IXamlType xamlType = provider.GetXamlType(type);
			if (!xamlType) return nullptr;

			while (xamlType)
			{
				// We need to manually explore the custom attributes this way as the target one
				// is not returned by any of the other available GetCustomAttribute<T> APIs.
				winrt::Microsoft::UI::Xaml::Markup::IXamlMember member = xamlType.ContentProperty();

				if (member)
				{
					return member.GetValue(element).try_as<UIElement>();
				}

				xamlType = xamlType.BaseType();
			}

			return nullptr;
		}

		static Windows::Foundation::IInspectable FindResource(FrameworkElement const& element, Windows::Foundation::IInspectable resourceKey)
		{
			Windows::Foundation::IInspectable value{ nullptr };
			if (TryFindResource(element, resourceKey, value))
			{
				return value;
			}

			throw winrt::hresult_out_of_bounds(winrt::format(L"No resource was found with the key {}", winrt::get_class_name(resourceKey)));
		}

		static Windows::Foundation::IInspectable TryFindResource(FrameworkElement const& element, Windows::Foundation::IInspectable resourceKey)
		{
			FrameworkElement current = element;

			// Look in our dictionary and then walk-up parents. We use a do-while loop here
			// so that an implicit NRE will be thrown at the first iteration in case the
			// input element is null. This is consistent with the other extensions.
			do
			{
				if (auto value = current.Resources().TryLookup(resourceKey))
				{
					return value;
				}

				current = current.Parent().try_as<FrameworkElement>();
			} while (current);

			// Finally try application resources
			return Application::Current().Resources().TryLookup(resourceKey);
		}

		static bool TryFindResource(FrameworkElement const& element, Windows::Foundation::IInspectable const& resourceKey, Windows::Foundation::IInspectable& value)
		{
			return ((value = TryFindResource(element, resourceKey)) != nullptr);
		}
	};
}

