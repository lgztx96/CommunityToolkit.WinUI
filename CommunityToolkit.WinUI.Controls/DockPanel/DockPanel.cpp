#include "pch.h"
#include "DockPanel.h"
#if __has_include("DockPanel.g.cpp")
#include "DockPanel.g.cpp"
#endif
#include "../CommunityToolkit.WinUI/common.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void DockPanel::DockChanged(DependencyObject const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		auto senderElement = sender.try_as<FrameworkElement>();
		if (auto dockPanel = FrameworkElementEx::FindParent<class_type>(senderElement))
		{
			dockPanel.InvalidateArrange();
		}
	}

	void DockPanel::OnPropertyChanged(DependencyObject const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		if (auto dockPanel = sender.try_as<class_type>())
		{
			dockPanel.InvalidateMeasure();
		}
	}

	Size DockPanel::ArrangeOverride(Size finalSize)
	{
		auto children = Children();
		if (children.Size() == 0)
			return finalSize;

		auto padding = Padding();

		auto currentBounds = Rect(
			static_cast<float>(padding.Left),
			static_cast<float>(padding.Top),
			static_cast<float>(std::max<double>(0, finalSize.Width - padding.Left - padding.Right)),
			static_cast<float>(std::max<double>(0, finalSize.Height - padding.Top - padding.Bottom)));
		auto childrenCount = LastChildFill() ? children.Size() - 1 : children.Size();

		for (auto index = 0; index < static_cast<int32_t>(childrenCount); ++index)
		{
			auto child = children.GetAt(index);
			if (child.Visibility() == Visibility::Collapsed)
				continue;
			auto dock = winrt::unbox_value<Dock>(child.GetValue(DockProperty));
			double width, height;
			switch (dock)
			{
			case Dock::Left:

				width = std::min<double>(child.DesiredSize().Width, currentBounds.Width);
				child.Arrange(Rect(currentBounds.X, currentBounds.Y, static_cast<float>(width), currentBounds.Height));
				width += HorizontalSpacing();
				currentBounds.X += static_cast<float>(width);
				currentBounds.Width = static_cast<float>(std::max<double>(0, currentBounds.Width - width));

				break;
			case Dock::Top:

				height = std::min<double>(child.DesiredSize().Height, currentBounds.Height);
				child.Arrange(Rect(currentBounds.X, currentBounds.Y, currentBounds.Width, static_cast<float>(height)));
				height += VerticalSpacing();
				currentBounds.Y += static_cast<float>(height);
				currentBounds.Height = static_cast<float>(std::max<double>(0, currentBounds.Height - height));

				break;
			case Dock::Right:

				width = std::min<double>(child.DesiredSize().Width, currentBounds.Width);
				child.Arrange(Rect(currentBounds.X + currentBounds.Width - static_cast<float>(width), currentBounds.Y, static_cast<float>(width), currentBounds.Height));
				width += HorizontalSpacing();
				currentBounds.Width = std::max<float>(0, currentBounds.Width - static_cast<float>(width));

				break;
			case Dock::Bottom:

				height = std::min<double>(child.DesiredSize().Height, currentBounds.Height);
				child.Arrange(Rect(currentBounds.X, currentBounds.Y + currentBounds.Height - static_cast<float>(height), currentBounds.Width, static_cast<float>(height)));
				height += VerticalSpacing();
				currentBounds.Height = std::max<float>(0, currentBounds.Height - static_cast<float>(height));

				break;
			}
		}

		if (LastChildFill() && children.Size() > 0)
		{
			auto child = children.GetAt(children.Size() - 1);
			child.Arrange(Rect(currentBounds.X, currentBounds.Y, currentBounds.Width, currentBounds.Height));
		}

		return finalSize;
	}

	Size DockPanel::MeasureOverride(Size availableSize)
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
			auto childConstraint = Size(
				std::max<float>(0, availableSize.Width - static_cast<float>(accumulatedWidth)),
				std::max<float>(0, availableSize.Height - static_cast<float>(accumulatedHeight)));

			child.Measure(childConstraint);
			auto childDesiredSize = child.DesiredSize();
			auto dock = winrt::unbox_value<Dock>(child.GetValue(DockProperty));
			switch (dock)
			{
			case Dock::Left:
			case Dock::Right:
				parentHeight = std::max<double>(parentHeight, accumulatedHeight + childDesiredSize.Height);
				if (child.Visibility() == Visibility::Visible)
				{
					accumulatedWidth += HorizontalSpacing();
					horizontalSpacing = true;
				}

				accumulatedWidth += childDesiredSize.Width;
				break;

			case Dock::Top:
			case Dock::Bottom:
				parentWidth = std::max<double>(parentWidth, accumulatedWidth + childDesiredSize.Width);
				if (child.Visibility() == Visibility::Visible)
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
			auto childConstraint = Size(
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
		return Size(static_cast<float>(parentWidth), static_cast<float>(parentHeight));
	}
}
