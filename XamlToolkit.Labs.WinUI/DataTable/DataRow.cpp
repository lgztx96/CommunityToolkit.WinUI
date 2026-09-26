#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <ranges>
#endif
#include "DataRow.h"
#include "DataColumn.h"
#if __has_include("DataRow.g.cpp")
#include "DataRow.g.cpp"
#endif
#include "../../XamlToolkit.WinUI/common.h"
#include "DataTable.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	DataRow::DataRow()
	{
		Unloaded({ this, &DataRow::DataRow_Unloaded });
	}

	void DataRow::DataRow_Unloaded([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		// Remove our references on unloaded
		if (_parentTable)
		{
			winrt::get_self<implementation::DataTable>(_parentTable)->Rows().erase(*this); // Notify table that we may have changed size
			_parentTable = nullptr;
		}

		_parentPanel = nullptr;
	}

	Panel DataRow::InitializeParentHeaderConnection()
	{
		Panel panel = nullptr;
		// TreeView's items are hosted by TreeViewList. The TreeView control itself
		// is therefore not guaranteed to be a visual ancestor of the DataRow.
		// TreeViewItem is the reliable per-row visual ancestor, including nested
		// items created by a hierarchical ItemTemplate.
		_isTreeView = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<TreeViewItem>(*this) != nullptr;

		const auto resolveHeaderPanel = [](winrt::Windows::Foundation::IInspectable const& header) -> Panel
		{
			if (auto table = header.try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>())
			{
				return table;
			}

			if (auto headerObject = header.try_as<DependencyObject>())
			{
				if (auto table = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindDescendant<winrt::XamlToolkit::Labs::WinUI::DataTable>(headerObject))
				{
					return table;
				}

				if (auto grid = header.try_as<Grid>())
				{
					return grid;
				}

				return winrt::XamlToolkit::WinUI::DependencyObjectEx::FindDescendant<Grid>(headerObject);
			}

			return nullptr;
		};

		// Resolve HeaderedTreeView directly as well as through ItemsPresenter so
		// custom templates can choose where the header is hosted.
		if (auto tree = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::XamlToolkit::WinUI::Controls::HeaderedTreeView>(*this))
		{
			panel = resolveHeaderPanel(tree.Header());
		}

		// A nested TreeViewItem can add another ItemsPresenter between the row and the
		// TreeViewList. Preserve support for controls whose header remains in their
		// ItemsPresenter.
		for (const auto& ancestor : winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendants(*this))
		{
			if (panel != nullptr) break;

			auto itemsPresenter = ancestor.try_as<ItemsPresenter>();
			if (itemsPresenter == nullptr || itemsPresenter.Header() == nullptr)
			{
				continue;
			}

			panel = resolveHeaderPanel(itemsPresenter.Header());
		}

		// Preserve the Grid hybrid scenario, but do not mistake TreeViewItem's
		// MultiSelectGrid for the row's column definition source.
		if (panel == nullptr && !_isTreeView)
		{
			panel = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<Panel>(*this,
				[](auto&& element) { return element.template try_as<Grid>() || element.template try_as<DataTable>(); });
		}

		// Cache actual datatable reference
		if (auto table = panel.try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>())
		{
			_parentTable = table;
			winrt::get_self<implementation::DataTable>(_parentTable)->Rows().insert(*this); // Add us to the row list.
		}

		return panel;
	}

	double DataRow::GetTreePadding()
	{
		if (!_isTreeView)
		{
			return 0;
		}

		auto container = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant(*this, L"MultiSelectGrid").try_as<Grid>();
		if (container == nullptr)
		{
			return 0;
		}

		// Account for the fixed outer presenter inset before applying the
		// depth-dependent MultiSelectGrid indentation.
		double padding = container.Margin().Left + container.BorderThickness().Left + container.Padding().Left;
		if (auto presenter = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant(*this, L"ContentPresenterGrid").try_as<Grid>())
		{
			padding += presenter.Margin().Left + presenter.BorderThickness().Left + presenter.Padding().Left;
		}
		auto definitions = container.ColumnDefinitions();
		if (definitions.Size() > 1)
		{
			// DataRow is hosted in the final content column. The preceding template
			// columns contain selection and expand/collapse affordances.
			for (uint32_t i = 0; i + 1 < definitions.Size(); i++)
			{
				padding += definitions.GetAt(i).ActualWidth();
			}
		}
		else
		{
			// Fallback for a customized TreeViewItem template without column
			// definitions.
			auto containerChildren = container.Children();
			for (uint32_t i = 0; i + 1 < containerChildren.Size(); i++)
			{
				padding += containerChildren.GetAt(i).DesiredSize().Width;
			}
		}

		return padding;
	}

	Size DataRow::MeasureOverride(Size availableSize)
	{
		// We should probably only have to do this once ever?
		if (_parentPanel == nullptr) _parentPanel = InitializeParentHeaderConnection();

		double maxHeight = 0;
		auto children = Children();
		uint32_t childCount = children.Size();

		if (childCount > 0)
		{
			// If we don't have a grid, just measure first child to get row height and take available space
			if (_parentPanel == nullptr)
			{
				children.GetAt(0).Measure(availableSize);
				return Size(availableSize.Width, children.GetAt(0).DesiredSize().Height);
			}
			// Handle DataTable Parent
			else if (_parentTable != nullptr
				&& _parentTable.Children().Size() == childCount)
			{
				// TODO: Need to check visibility
				// Measure all children since we need to determine the row's height at minimum
				for (uint32_t i = 0; i < childCount; i++)
				{
					auto childElement = children.GetAt(i);
					auto dataColumn = _parentTable.Children().GetAt(i).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();

					if (dataColumn == nullptr) continue;

					auto colImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(dataColumn);

					if (colImpl->CurrentWidth().GridUnitType == GridUnitType::Auto)
					{
						childElement.Measure(availableSize);

						// TODO: Do we want this to ever shrink back?
						auto& prev = colImpl->MaxChildDesiredWidth;
						colImpl->MaxChildDesiredWidth = std::max<double>(colImpl->MaxChildDesiredWidth, childElement.DesiredSize().Width);
						if (colImpl->MaxChildDesiredWidth != prev)
						{
							// If our measure has changed, then we have to invalidate the arrange of the DataTable
							winrt::get_self<implementation::DataTable>(_parentTable)->ColumnResized();
						}

					}
					else if (colImpl->CurrentWidth().GridUnitType == GridUnitType::Pixel)
					{
						childElement.Measure(Size(static_cast<float>(colImpl->CurrentWidth().Value), availableSize.Height));
					}
					else
					{
						childElement.Measure(availableSize);
					}

					maxHeight = std::max<double>(maxHeight, childElement.DesiredSize().Height);
				}
			}
			// Fallback for Grid Hybrid scenario...
			else if (auto grid = _parentPanel.try_as<Grid>();
				grid && _parentPanel.Children().Size() == childCount
				&& grid.ColumnDefinitions().Size() == Children().Size())
			{
				// TODO: Need to check visibility
				// Measure all children since we need to determine the row's height at minimum
				for (uint32_t i = 0; i < childCount; i++)
				{
					auto childElement = children.GetAt(i);
					auto colDef = grid.ColumnDefinitions().GetAt(i);
					if (colDef.Width().GridUnitType == GridUnitType::Pixel)
					{
						childElement.Measure(Size(static_cast<float>(colDef.Width().Value), availableSize.Height));
					}
					else
					{
						childElement.Measure(availableSize);
					}

					maxHeight = std::max<double>(maxHeight, childElement.DesiredSize().Height);
				}
			}
			// TODO: What do we want to do if there's unequal children in the DataTable vs. DataRow?
		}

		// Keep the row at least as wide as the item container so shrinking columns
		// never shifts the whole row. If the header/table needs more room, propagate
		// that larger desired width so the owning ScrollViewer can expose a horizontal
		// extent instead of clipping the cells.
		double desiredWidth = availableSize.Width;
		if (_parentPanel != nullptr)
		{
			desiredWidth = std::max<double>(desiredWidth, _parentPanel.DesiredSize().Width);
		}
		if (!std::isfinite(desiredWidth))
		{
			desiredWidth = _parentPanel != nullptr ? _parentPanel.DesiredSize().Width : 0;
		}

		return Size(static_cast<float>(desiredWidth), static_cast<float>(maxHeight));
	}

	winrt::Size DataRow::ArrangeOverride(winrt::Size finalSize)
	{
		uint32_t column = 0;
		// Use only the current TreeViewItem template's local layout data. A global
		// transform can still describe a recycled container's previous position
		// while its new Arrange pass is running.
		double x = -GetTreePadding();

		// Try and grab Column Spacing from DataTable, if not a parent Grid, if not 0.
		double spacing = 0.0;

		if (_parentTable)
		{
			spacing = _parentTable.ColumnSpacing();
		}
		else if (auto grid = _parentPanel.try_as<winrt::Microsoft::UI::Xaml::Controls::Grid>())
		{
			spacing = grid.ColumnSpacing();
		}

		double width = 0;

		if (_parentPanel != nullptr)
		{
			int i = 0;
			auto elements = Children()
				| std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible; });

			for (const UIElement& child : elements)
			{
				if (auto grid = _parentPanel.try_as<Grid>(); grid &&
					column < grid.ColumnDefinitions().Size())
				{
					width = grid.ColumnDefinitions().GetAt(column++).ActualWidth();
				}
				// TODO: Need to check Column visibility here as well...
				else
				{
					if (auto table = _parentPanel.try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>(); table && column < table.Children().Size()) {
						auto tableImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataTable>(table);
						width = tableImpl->ColumnWidth(column++);
					}
				}

				// DataRow itself begins after TreeViewItem's indentation. Its first
				// cell cannot start before that origin, but its right edge and every
				// following column use the header's exact coordinates.
				const auto arrangeX = i == 0 && _isTreeView ? 0 : x;
				const auto arrangeWidth = i == 0 && _isTreeView
					? std::max<double>(x + width, 0)
					: width;
				child.Arrange(Rect(static_cast<float>(arrangeX), 0, static_cast<float>(arrangeWidth), finalSize.Height));
				x += width + spacing;
				i++;
			}

			// The row keeps the slot assigned by its item container; horizontal
			// overflow is represented by the desired width returned from Measure.
			return finalSize;
		}

		return finalSize;
	}
}
