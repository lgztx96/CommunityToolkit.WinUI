#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "DockPanel.h"
#if __has_include("DockPanel.g.cpp")
#include "DockPanel.g.cpp"
#endif
#include "../XamlToolkit.WinUI/common.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> DockPanel::DockProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"Dock",
			winrt::xaml_typename<winrt::Dock>(),
			winrt::xaml_typename<winrt::FrameworkElement>(),
			winrt::PropertyMetadata{ winrt::box_value(winrt::Dock::Left), &DockPanel::DockChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> DockPanel::LastChildFillProperty =
		winrt::DependencyProperty::Register(
			L"LastChildFill",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(true), &DockPanel::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> DockPanel::PaddingProperty =
		winrt::DependencyProperty::Register(
			L"Padding",
			winrt::xaml_typename<winrt::Thickness>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }), &DockPanel::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> DockPanel::HorizontalSpacingProperty =
		winrt::DependencyProperty::Register(
			L"HorizontalSpacing",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(0.0), &DockPanel::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> DockPanel::VerticalSpacingProperty =
		winrt::DependencyProperty::Register(
			L"VerticalSpacing",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(0.0), &DockPanel::OnPropertyChanged });

	void DockPanel::DockChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		auto senderElement = sender.try_as<winrt::FrameworkElement>();
		if (const auto dockPanel = FrameworkElementEx::FindParent<class_type>(senderElement))
		{
			dockPanel.InvalidateArrange();
		}
	}

	void DockPanel::OnPropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (const auto dockPanel = sender.try_as<class_type>())
		{
			dockPanel.InvalidateMeasure();
		}
	}

	winrt::Size DockPanel::ArrangeOverride(winrt::Size finalSize)
	{
		auto children = Children();
		if (children.Size() == 0)
			return finalSize;

		auto padding = Padding();

		auto currentBounds = winrt::Rect(
			static_cast<float>(padding.Left),
			static_cast<float>(padding.Top),
			static_cast<float>(std::max<double>(0, finalSize.Width - padding.Left - padding.Right)),
			static_cast<float>(std::max<double>(0, finalSize.Height - padding.Top - padding.Bottom)));
		auto childrenCount = LastChildFill() ? children.Size() - 1 : children.Size();

		for (auto index = 0; index < static_cast<int32_t>(childrenCount); ++index)
		{
			auto child = children.GetAt(index);
			if (child.Visibility() == winrt::Visibility::Collapsed)
				continue;
			auto dock = winrt::unbox_value<winrt::Dock>(child.GetValue(DockProperty));
			double width, height;
			switch (dock)
			{
			case winrt::Dock::Left:

				width = std::min<double>(child.DesiredSize().Width, currentBounds.Width);
				child.Arrange(winrt::Rect(currentBounds.X, currentBounds.Y, static_cast<float>(width), currentBounds.Height));
				width += HorizontalSpacing();
				currentBounds.X += static_cast<float>(width);
				currentBounds.Width = static_cast<float>(std::max<double>(0, currentBounds.Width - width));

				break;
			case winrt::Dock::Top:

				height = std::min<double>(child.DesiredSize().Height, currentBounds.Height);
				child.Arrange(winrt::Rect(currentBounds.X, currentBounds.Y, currentBounds.Width, static_cast<float>(height)));
				height += VerticalSpacing();
				currentBounds.Y += static_cast<float>(height);
				currentBounds.Height = static_cast<float>(std::max<double>(0, currentBounds.Height - height));

				break;
			case winrt::Dock::Right:

				width = std::min<double>(child.DesiredSize().Width, currentBounds.Width);
				child.Arrange(winrt::Rect(currentBounds.X + currentBounds.Width - static_cast<float>(width), currentBounds.Y, static_cast<float>(width), currentBounds.Height));
				width += HorizontalSpacing();
				currentBounds.Width = std::max<float>(0, currentBounds.Width - static_cast<float>(width));

				break;
			case winrt::Dock::Bottom:

				height = std::min<double>(child.DesiredSize().Height, currentBounds.Height);
				child.Arrange(winrt::Rect(currentBounds.X, currentBounds.Y + currentBounds.Height - static_cast<float>(height), currentBounds.Width, static_cast<float>(height)));
				height += VerticalSpacing();
				currentBounds.Height = std::max<float>(0, currentBounds.Height - static_cast<float>(height));

				break;
			}
		}

		if (LastChildFill() && children.Size() > 0)
		{
			auto child = children.GetAt(children.Size() - 1);
			child.Arrange(winrt::Rect(currentBounds.X, currentBounds.Y, currentBounds.Width, currentBounds.Height));
		}

		return finalSize;
	}

	winrt::Size DockPanel::MeasureOverride(winrt::Size availableSize)
	{
		auto children = Children();
		auto parentWidth = 0.0;
		auto parentHeight = 0.0;
		auto accumulatedWidth = 0.0;
		auto accumulatedHeight = 0.0;

		auto horizontalSpacing = false;
		auto verticalSpacing = false;
		auto childrenCount = LastChildFill() ? children.Size() - 1 : children.Size();

		for (auto index = 0; index < static_cast<int32_t>(childrenCount); ++index)
		{
			auto child = children.GetAt(index);
			auto childConstraint = winrt::Size(
				std::max<float>(0, availableSize.Width - static_cast<float>(accumulatedWidth)),
				std::max<float>(0, availableSize.Height - static_cast<float>(accumulatedHeight)));

			child.Measure(childConstraint);
			auto childDesiredSize = child.DesiredSize();
			auto dock = winrt::unbox_value<winrt::Dock>(child.GetValue(DockProperty));
			switch (dock)
			{
			case winrt::Dock::Left:
			case winrt::Dock::Right:
				parentHeight = std::max<double>(parentHeight, accumulatedHeight + childDesiredSize.Height);
				if (child.Visibility() == winrt::Visibility::Visible)
				{
					accumulatedWidth += HorizontalSpacing();
					horizontalSpacing = true;
				}

				accumulatedWidth += childDesiredSize.Width;
				break;

			case winrt::Dock::Top:
			case winrt::Dock::Bottom:
				parentWidth = std::max<double>(parentWidth, accumulatedWidth + childDesiredSize.Width);
				if (child.Visibility() == winrt::Visibility::Visible)
				{
					accumulatedHeight += VerticalSpacing();
					verticalSpacing = true;
				}

				accumulatedHeight += childDesiredSize.Height;
				break;
			}
		}

		if (LastChildFill() && children.Size() > 0)
		{
			auto child = children.GetAt(children.Size() - 1);
			auto childConstraint = winrt::Size(
				std::max<float>(0, availableSize.Width - static_cast<float>(accumulatedWidth)),
				std::max<float>(0, availableSize.Height - static_cast<float>(accumulatedHeight)));

			child.Measure(childConstraint);
			auto childDesiredSize = child.DesiredSize();
			parentHeight = std::max<double>(parentHeight, accumulatedHeight + childDesiredSize.Height);
			parentWidth = std::max<double>(parentWidth, accumulatedWidth + childDesiredSize.Width);
			accumulatedHeight += childDesiredSize.Height;
			accumulatedWidth += childDesiredSize.Width;
		}
		else
		{
			if (horizontalSpacing)
				accumulatedWidth -= HorizontalSpacing();
			if (verticalSpacing)
				accumulatedHeight -= VerticalSpacing();
		}

		// Make sure the final accumulated size is reflected in parentSize.
		parentWidth = std::max<double>(parentWidth, accumulatedWidth);
		parentHeight = std::max<double>(parentHeight, accumulatedHeight);
		return winrt::Size(static_cast<float>(parentWidth), static_cast<float>(parentHeight));
	}
}
