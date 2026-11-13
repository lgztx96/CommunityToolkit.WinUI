#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsGroup.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	void SettingsGroup::OnItemsChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<winrt::XamlToolkit::WinUI::Controls::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseItemsChanged();
		}
	}

	void SettingsGroup::OnContentChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<winrt::XamlToolkit::WinUI::Controls::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseContentChanged();
		}
	}

	void SettingsGroup::OnHeaderIconChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<winrt::XamlToolkit::WinUI::Controls::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseHeaderIconChanged();
		}
	}

	void SettingsGroup::OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<winrt::XamlToolkit::WinUI::Controls::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseHeaderChanged();
		}
	}

	void SettingsGroup::OnDescriptionChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto settingsGroup = d.try_as<winrt::XamlToolkit::WinUI::Controls::implementation::SettingsGroup>())
		{
			settingsGroup->RaiseDescriptionChanged();
		}
	}

	void SettingsGroup::RaiseItemsChanged()
	{
		if (_itemsRepeater)
		{
			RootGridCornerRadius(GetRootGridCornerRadius());

			if (_rootGrid)
			{
				_rootGrid.CornerRadius(RootGridCornerRadius());

				if (Items().Size() > 0)
				{
					_rootGrid.BorderThickness(winrt::Thickness{ 1, 1, 1, 0 });
				}
				else
				{
					_rootGrid.BorderThickness(winrt::Thickness{ 1, 1, 1, 1 });
				}
			}

			auto size = Items().Size();
			for (int32_t i = 0; i < size; ++i)
			{
				auto item = Items().GetAt(i);

				if (auto control = item.try_as<winrt::Control>())
				{
					winrt::CornerRadius cornerRadius{ 0 };
					winrt::Thickness borderThickness{ 1, 1, 1, 0 };

					if (i == size - 1)
					{
						cornerRadius = winrt::CornerRadius{ 0, 0, 8, 8 };
						borderThickness = winrt::Thickness{ 1, 1, 1, 1 };
					}
					control.BorderThickness(borderThickness);
					control.CornerRadius(cornerRadius);
					control.Padding(winrt::Thickness{ 25, 0, 25, 0 });
					control.Margin(winrt::Thickness{ 0 });
				}
			}
			_itemsRepeater.ItemsSource(Items());
		}
	}

	void SettingsGroup::RaiseContentChanged()
	{
		if (_contentPresenter)
		{
			_contentPresenter.Visibility(Content() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::RaiseHeaderIconChanged()
	{
		if (auto headerIconPresenter = GetTemplateChild(PartHeaderIconPresenter).try_as<winrt::FrameworkElement>())
		{
			headerIconPresenter.Visibility(HeaderIcon() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::RaiseHeaderChanged()
	{
		if (_headerPresenter)
		{
			_headerPresenter.Visibility(Header() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::RaiseDescriptionChanged()
	{
		if (_descriptionPresenter)
		{
			_descriptionPresenter.Visibility(Description() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
		}
	}

	void SettingsGroup::Items_CollectionChanged(winrt::IInspectable const& s, winrt::IVectorChangedEventArgs const& e)
	{
		RaiseItemsChanged();
	}

	void SettingsGroup::SettingsGroup_IsEnabledChanged(winrt::IInspectable const& s, DependencyPropertyChangedEventArgs const& e)
	{
		winrt::VisualStateManager::GoToState(*this, IsEnabled() ? L"Normal" : L"Disabled", true);
	}
}
