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

    void DataRow::DataRow_Unloaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        // Remove our references on unloaded
        if (_parentTable)
        {
			winrt::get_self<implementation::DataTable>(_parentTable)->Rows().erase(*this); // Notify table that we may have changed size
            _parentTable = nullptr;
        }
       
        _parentPanel = nullptr;
    }

    winrt::Panel DataRow::InitializeParentHeaderConnection()
    {
        // TODO: Think about this expression instead...
        //       Drawback: Can't have Grid between table and header
        //       Positive: don't have to restart climbing the Visual Tree if we don't find ItemsPresenter...
        ////var parent = this.FindAscendant<FrameworkElement>(static (element) => element is ItemsPresenter or Grid);

        // TODO: Investigate what a scenario with an ItemsRepeater would look like (with a StackLayout, but using DataRow as the item's panel inside)
        winrt::Panel panel { nullptr };

        // 1a. Get parent ItemsPresenter to find header
        if (auto itemsPresenter = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::ItemsPresenter>(*this))
        {
            // 2. Quickly check if the header is just what we're looking for.
			auto header = itemsPresenter.Header();
            if (header.try_as<winrt::Grid>() || header.try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>())
            {
                panel = itemsPresenter.Header().try_as<winrt::Panel>();
            }
            else
            {
                // 3. Otherwise, try and find the inner thing we want.
                panel = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindDescendant<winrt::Panel>(itemsPresenter,
                    [](auto&& element) { return element.template try_as<winrt::Grid>() || element.template try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>(); });
            }

            // Check if we're in a TreeView
            _isTreeView = static_cast<bool>(winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::TreeView>(itemsPresenter).try_as<winrt::TreeView>());
        }

        // 1b. If we can't find the ItemsPresenter, then we reach up outside to find the next thing we could use as a parent
        if (panel == nullptr)
        {
            panel = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant<winrt::Panel>(*this, [](auto&& element) 
            { 
                return element.template try_as<winrt::Grid>() || element.template try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>();
            });
        }
        // Cache actual datatable reference
        if (auto table = panel.try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>())
        {
            _parentTable = table;
            winrt::get_self<implementation::DataTable>(_parentTable)->Rows().insert(*this); // Add us to the row list.
        }

        return panel;
    }

    winrt::Size DataRow::MeasureOverride(winrt::Size availableSize)
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
                auto first = children.GetAt(0);
                first.Measure(availableSize);
                return winrt::Size(availableSize.Width, first.DesiredSize().Height);
        }
            // Handle DataTable Parent
            else if (_parentTable
                && _parentTable.Children().Size() == childCount)
            {
                // TODO: Need to check visibility
                // Measure all children since we need to determine the row's height at minimum
                for (uint32_t i = 0; i < childCount; i++)
                {
                    auto childElement = children.GetAt(i);
                    auto dataColumn = _parentTable.Children().GetAt(i).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();

					if (dataColumn == nullptr) continue;

					auto colImpl = winrt::get_self<implementation::DataColumn>(dataColumn);

                    if (colImpl->CurrentWidth().GridUnitType == winrt::GridUnitType::Auto)
                    {
                        childElement.Measure(availableSize);

                        // For TreeView in the first column, we want the header to expand to encompass
                        // the maximum indentation of the tree.
                        double padding = 0;
                        //// TODO: We only want/need to do this once? We may want to do if we're not an Auto column too...?
                        if (i == 0 && _isTreeView)
                        {
                            // Get our containing grid from TreeViewItem, start with our indented padding
                            auto parentContainer = winrt::XamlToolkit::WinUI::DependencyObjectEx::FindAscendant(*this, L"MultiSelectGrid").try_as<winrt::Grid>();
                            if (parentContainer)
                            {
                                _treePadding = parentContainer.Padding().Left;
                                // We assume our 'DataRow' is in the last child slot of the Grid, need to know how large the other columns are.
                                auto containerChildren = parentContainer.Children();
                                uint32_t containerChildrenCount = containerChildren.Size();
                                for (int j = 0; j < static_cast<int>(containerChildrenCount) - 1; j++)
                                {
                                    // TODO: We may need to get the actual size here later in Arrange?
                                    _treePadding += containerChildren.GetAt(j).DesiredSize().Width;
                                }
                            }
                            padding = _treePadding;
                        }

                        // TODO: Do we want this to ever shrink back?
                        double prev = colImpl->MaxChildDesiredWidth;
                        colImpl->MaxChildDesiredWidth = std::max<double>(colImpl->MaxChildDesiredWidth, childElement.DesiredSize().Width + padding);
                        if (colImpl->MaxChildDesiredWidth != prev)
                        {
                            // If our measure has changed, then we have to invalidate the arrange of the DataTable
                            winrt::get_self<implementation::DataTable>(_parentTable)->ColumnResized();
                        }

                    }
                    else if (colImpl->CurrentWidth().GridUnitType == winrt::GridUnitType::Pixel)
                    {
                        childElement.Measure(winrt::Size(static_cast<float>(colImpl->DesiredWidth().Value), availableSize.Height));
                    }
                    else
                    {
                        childElement.Measure(availableSize);
                    }

                    maxHeight = std::max<double>(maxHeight, childElement.DesiredSize().Height);
                }
            }
            // Fallback for Grid Hybrid scenario...
            else if (auto grid = _parentPanel.try_as<winrt::Grid>();
                grid && _parentPanel.Children().Size() == childCount
                && grid.ColumnDefinitions().Size() == Children().Size())
            {
                // TODO: Need to check visibility
                // Measure all children since we need to determine the row's height at minimum
                for (uint32_t i = 0; i < childCount; i++)
                {
                    auto childElement = children.GetAt(i);
					auto colDef = grid.ColumnDefinitions().GetAt(i);
                    if (colDef.Width().GridUnitType == winrt::GridUnitType::Pixel)
                    {
                        childElement.Measure(winrt::Size(static_cast<float>(colDef.Width().Value), availableSize.Height));
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

        // Otherwise, return our parent's size as the desired size.
        float desiredWidth = availableSize.Width;

        if (_parentPanel)
        {
            desiredWidth = std::max(_parentPanel.DesiredSize().Width, desiredWidth);
        }

        return winrt::Size(desiredWidth, static_cast<float>(maxHeight));
    }

    winrt::Size DataRow::ArrangeOverride(winrt::Size finalSize)
    {
        uint32_t column = 0;
        double x = 0;

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

        if (_parentPanel)
        {
            int i = 0;
            auto elements = Children()
                | std::ranges::views::filter([](auto&& e) { return e.Visibility() == winrt::Visibility::Visible; });

            for (const auto& child : elements)
            {
                if (auto grid = _parentPanel.try_as<winrt::Grid>(); grid &&
                    column < grid.ColumnDefinitions().Size())
                {
                    width = grid.ColumnDefinitions().GetAt(column++).ActualWidth();
                }
                // TODO: Need to check Column visibility here as well...
                else
                {
                    if (auto table = _parentPanel.try_as<winrt::XamlToolkit::Labs::WinUI::DataTable>(); table && column < table.Children().Size()) {
                        // TODO: This is messy...
						auto col = table.Children().GetAt(column++).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();
                        width = (col) ? col.ActualWidth() : 0;
                    }
                }

                // Note: For Auto, since we measured our children and bubbled that up to the DataTable layout, then the DataColumn size we grab above should account for the largest of our children.
                if (i == 0)
                {
                    child.Arrange(winrt::Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
                }
                else
                {
                    // If we're in a tree, remove the indentation from the layout of columns beyond the first.
                    child.Arrange(winrt::Rect(static_cast<float>(x - _treePadding), 0, static_cast<float>(width), finalSize.Height));
                }

                x += width + spacing;
                i++;
            }

            return winrt::Size(static_cast<float>(x - spacing), finalSize.Height);
        }

        return finalSize;
    }
}
