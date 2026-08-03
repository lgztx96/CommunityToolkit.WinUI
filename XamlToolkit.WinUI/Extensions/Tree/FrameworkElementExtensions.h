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

			return FindChild<winrt::FrameworkElement>(element, predicateByName);
		}

		template <typename T>
		static T FindChild(winrt::FrameworkElement const& element)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByAny<T> predicateByAny;

			return FindChild<T>(element, predicateByAny);
		}

		static winrt::FrameworkElement FindChild(winrt::FrameworkElement const& element, winrt::TypeName const& type)
		{
			PredicateByType predicateByType(type);

			return FindChild<winrt::FrameworkElement>(element, predicateByType);
		}

		template <typename T, IPredicate<T> TPredicate>
		static T FindChild(winrt::FrameworkElement element, TPredicate&& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			// Jump label to manually optimize the tail recursive paths for elements with a single
			// child by just overwriting the current element and jumping back to the start of the
			// method. This avoids a recursive call and one stack frame every time.
		Start:

			if (auto panel = element.try_as<winrt::Panel>())
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

					T descendant = FindChild<T>(current, predicate);

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

					T descendant = FindChild<T>(current, predicate);

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

		static winrt::FrameworkElement FindChildOrSelf(winrt::FrameworkElement const& element, winrt::TypeName const& type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindChild(element, type);
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindChildOrSelf(winrt::FrameworkElement const& element, TPredicate&& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindChild<T>(element, predicate);
		}

		std::generator<winrt::FrameworkElement> FindChildren(winrt::FrameworkElement const& element)
		{
			if (auto panel = element.try_as<winrt::Panel>())
			{
				for (const auto& child : panel.Children())
				{
					if (auto current = child.try_as<winrt::FrameworkElement>())
					{
						co_yield current;

						for (const auto& childOfChild : FindChildren(current))
						{
							co_yield childOfChild;
						}
					}
				}
			}
			else if (auto itemsControl = element.try_as<winrt::ItemsControl>())
			{
				for (const auto& item : itemsControl.Items())
				{
					if (auto current = item.try_as<winrt::FrameworkElement>())
					{
						co_yield current;

						for (const auto& childOfChild : FindChildren(current))
						{
							co_yield childOfChild;
						}
					}
				}
			}
			else if (auto contentControl = element.try_as<winrt::ContentControl>())
			{
				if (auto content = contentControl.Content().try_as<winrt::FrameworkElement>())
				{
					co_yield content;

					for (const auto& child : FindChildren(content))
					{
						co_yield child;
					}
				}
			}
			else if (auto border = element.try_as<winrt::Border>())
			{
				if (auto child = border.Child().try_as<winrt::FrameworkElement>())
				{
					co_yield child;

					for (const auto& childOfChild : FindChildren(child))
					{
						co_yield childOfChild;
					}
				}
			}
			else if (auto contentPresenter = element.try_as<winrt::ContentPresenter>())
			{
				if (auto content = contentPresenter.Content().try_as<winrt::FrameworkElement>())
				{
					co_yield content;

					for (const auto& childOfChild : FindChildren(content))
					{
						co_yield childOfChild;
					}
				}
			}
			else if (auto viewbox = element.try_as<winrt::Viewbox>())
			{
				if (auto child = viewbox.Child().try_as<winrt::FrameworkElement>())
				{
					co_yield child;

					for (const auto& childOfChild : FindChildren(child))
					{
						co_yield childOfChild;
					}
				}
			}
			else if (auto userControl = element.try_as<winrt::UserControl>())
			{
				if (auto content = userControl.Content().try_as<winrt::FrameworkElement>())
				{
					co_yield content;

					for (const auto& childOfChild : FindChildren(content))
					{
						co_yield childOfChild;
					}
				}
			}
			else if (auto containedControl = GetContentControl(element).try_as<winrt::FrameworkElement>())
			{
				co_yield containedControl;

				for (const auto& childOfChild : FindChildren(containedControl))
				{
					co_yield childOfChild;
				}
			}
		}

		static winrt::FrameworkElement FindParent(winrt::FrameworkElement const& element, std::wstring_view name)
		{
			PredicateByName predicateByName(name);

			return FindParent<winrt::FrameworkElement>(element, predicateByName);
		}

		template<typename T>
		static T FindParent(winrt::FrameworkElement element)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			PredicateByAny<T> predicateByAny;

			return FindParent<T>(element, predicateByAny);
		}

		static winrt::FrameworkElement FindParent(winrt::FrameworkElement const& element, winrt::TypeName const& type)
		{
			PredicateByType predicateByType(type);

			return FindParent<winrt::FrameworkElement>(element, predicateByType);
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindParent(winrt::FrameworkElement element, TPredicate&& predicate)
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

		static winrt::FrameworkElement FindParentOrSelf(winrt::FrameworkElement const& element, winrt::TypeName const& type)
		{
			if (winrt::get_class_name(element) == type.Name)
			{
				return element;
			}

			return FindParent(element, type);
		}

		template<typename T, IPredicate<T> TPredicate>
		static T FindParentOrSelf(winrt::FrameworkElement const& element, TPredicate&& predicate)
			requires winrt::derived_from<T, winrt::FrameworkElement>
		{
			if (auto result = element.try_as<T>(); result && predicate(result))
			{
				return result;
			}

			return FindParent<T>(element, predicate);
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
			auto app = winrt::Application::Current();
			if (!app)
			{
				return nullptr;
			}

			auto provider = app.try_as<winrt::IXamlMetadataProvider>();
			if (!provider)
			{
				return nullptr;
			}

			auto xamlType = provider.GetXamlType(winrt::get_class_name(element));

			while (xamlType)
			{
				if (auto member = xamlType.ContentProperty())
				{
					return member.GetValue(element).try_as<winrt::UIElement>();
				}

				xamlType = xamlType.BaseType();
			}

			return nullptr;
		}

		static winrt::IInspectable FindResource(winrt::FrameworkElement const& element, winrt::IInspectable const& resourceKey)
		{
			winrt::IInspectable value{ nullptr };
			if (TryFindResource(element, resourceKey, value))
			{
				return value;
			}

			throw winrt::hresult_out_of_bounds(winrt::format(L"No resource was found with the key {}", winrt::get_class_name(resourceKey)));
		}

		static winrt::IInspectable TryFindResource(winrt::FrameworkElement const& element, winrt::IInspectable const& resourceKey)
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
