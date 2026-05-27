#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <ranges>
#endif
#include "EqualPanel.h"
#if __has_include("EqualPanel.g.cpp")
#include "EqualPanel.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> EqualPanel::OrientationProperty =
		winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
			L"Orientation",
			winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::Orientation>(),
			winrt::xaml_typename<class_type>(),
			winrt::Microsoft::UI::Xaml::PropertyMetadata
			{
				winrt::box_value(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal),
				&EqualPanel::OnEqualPanelPropertyChanged
			});

	winrt::Microsoft::UI::Xaml::Controls::Orientation EqualPanel::Orientation() const
	{
		return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Controls::Orientation>(GetValue(OrientationProperty));
	}

	void EqualPanel::Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation const& value) const
	{
		SetValue(OrientationProperty, winrt::box_value(value));
	}

	const wil::single_threaded_property<DependencyProperty> EqualPanel::SpacingProperty = DependencyProperty::Register(
		L"Spacing",
		winrt::xaml_typename<double>(),
		winrt::xaml_typename<class_type>(),
		PropertyMetadata(winrt::box_value(0.0), &EqualPanel::OnEqualPanelPropertyChanged));

	double EqualPanel::Spacing() const { return winrt::unbox_value<double>(GetValue(SpacingProperty)); }

	void EqualPanel::Spacing(double value) { return SetValue(SpacingProperty, winrt::box_value(value)); }

	EqualPanel::EqualPanel()
	{
		RegisterPropertyChangedCallback(FrameworkElement::HorizontalAlignmentProperty(), { this, &EqualPanel::OnAlignmentChanged });
	}

	Size EqualPanel::MeasureOverride(Size availableSize)
	{
		_maxItemWidth = 0;
		_maxItemHeight = 0;

		auto elements = Children()
			| std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible; });
		_visibleItemsCount = 0;

		for (const auto& child : elements)
		{
			child.Measure(availableSize);
			_maxItemWidth = std::max<double>(_maxItemWidth, child.DesiredSize().Width);
			_maxItemHeight = std::max<double>(_maxItemHeight, child.DesiredSize().Height);
			_visibleItemsCount++;
		}

		// No children, no space taken
		if (_visibleItemsCount == 0)
			return Size(0, 0);

		// Determine if the desired alignment is stretched.
		// Don't stretch if infinite space is available though. Attempting to divide infinite space will result in a crash.
		bool stretch = false;
		switch (Orientation())
		{
		case Orientation::Horizontal:
			stretch =
				(HorizontalAlignment() == HorizontalAlignment::Stretch) &&
				!std::isinf(availableSize.Width);
			break;

		case Orientation::Vertical:
		default:
			stretch =
				(VerticalAlignment() == VerticalAlignment::Stretch) &&
				!std::isinf(availableSize.Height);
			break;
		}

		// Define UV coords for orientation agnostic XY manipulation
		auto uvSize = UVCoord(0.0, 0.0, Orientation());
		auto maxItemSize = UVCoord(_maxItemWidth, _maxItemHeight, Orientation());
		double availableU = Orientation() == Orientation::Horizontal ? availableSize.Width : availableSize.Height;

		if (stretch)
		{
			// Adjust maxItemU to form equal rows/columns by available U space (adjust for spacing)
			double totalU = availableU - (Spacing() * (_visibleItemsCount - 1));
			maxItemSize.U(totalU / _visibleItemsCount);

			// Set uSize/vSize for XY result construction
			uvSize.U(availableU);
			uvSize.V(maxItemSize.V());
		}
		else
		{
			uvSize.U((maxItemSize.U() * _visibleItemsCount) + (Spacing() * (_visibleItemsCount - 1)));
			uvSize.V(maxItemSize.V());
		}

		return Size(static_cast<float>(uvSize.X), static_cast<float>(uvSize.Y));
	}

	Size EqualPanel::ArrangeOverride(Size finalSize)
	{
		// Define UV axis
		auto orientation = Orientation();
		auto spacing = Spacing();
		auto pos = UVCoord(0, 0, orientation);
		double* maxItemU = &_maxItemWidth;
		double finalSizeU = finalSize.Width;
		if (orientation == Orientation::Vertical)
		{
			maxItemU = &_maxItemHeight;
			finalSizeU = finalSize.Height;
		}

		// Check if there's more (little) width available - if so, set max item width to the maximum possible as we have an almost perfect height.
		if (finalSizeU > _visibleItemsCount * *maxItemU + (spacing * (_visibleItemsCount - 1)))
		{
			*maxItemU = (finalSizeU - (spacing * (_visibleItemsCount - 1))) / _visibleItemsCount;
		}

		if (finalSize.Width > _visibleItemsCount * _maxItemWidth + (spacing * (_visibleItemsCount - 1)))
		{
			_maxItemWidth = (finalSize.Width - (spacing * (_visibleItemsCount - 1))) / _visibleItemsCount;
		}

		auto elements = Children()
			| std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible; });
		for (const auto& child : elements)
		{
			child.Arrange(Rect(
				static_cast<float>(pos.X), 
				static_cast<float>(pos.Y), 
				static_cast<float>(_maxItemWidth), 
				static_cast<float>(_maxItemHeight)));

			pos.U(pos.U() + *maxItemU + spacing);
		}

		return finalSize;
	}

	void EqualPanel::OnAlignmentChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		InvalidateMeasure();
	}

	void EqualPanel::OnEqualPanelPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		if (auto panel = d.try_as<class_type>())
			panel.InvalidateMeasure();
	}
}
