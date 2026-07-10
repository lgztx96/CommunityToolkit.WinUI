#pragma once

#include "WrapPanel2.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct WrapPanel2 : WrapPanel2T<WrapPanel2>
	{
		WrapPanel2();

		winrt::Size MeasureOverride(winrt::Size availableSize);

		winrt::Size ArrangeOverride(winrt::Size finalSize);

		static const wil::single_threaded_property<winrt::DependencyProperty> LayoutLengthProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> ItemSpacingProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> LineSpacingProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> ItemsJustificationProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> ItemsStretchProperty;

		winrt::Orientation Orientation() const
		{
			return winrt::unbox_value<winrt::Orientation>(GetValue(OrientationProperty()));
		}

		void Orientation(winrt::Orientation const& value)
		{
			SetValue(OrientationProperty(), winrt::box_value(value));
		}

		double ItemSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(ItemSpacingProperty()));
		}

		void ItemSpacing(double value) const
		{
			SetValue(ItemSpacingProperty(), winrt::box_value(value));
		}

		double LineSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(LineSpacingProperty()));
		}

		void LineSpacing(double value)
		{
			SetValue(LineSpacingProperty(), winrt::box_value(value));
		}

		WrapPanelItemsJustification ItemsJustification() const
		{
			return winrt::unbox_value<WrapPanelItemsJustification>(GetValue(ItemsJustificationProperty()));
		}

		void ItemsJustification(WrapPanelItemsJustification value)
		{
			SetValue(ItemsJustificationProperty(), winrt::box_value(value));
		}

		WrapPanelItemsStretch ItemsStretch() const
		{
			return winrt::unbox_value<WrapPanelItemsStretch>(GetValue(ItemsStretchProperty()));
		}

		void ItemsStretch(WrapPanelItemsStretch value)
		{
			SetValue(ItemsStretchProperty(), winrt::box_value(value));
		}

		static winrt::GridLength GetLayoutLength(winrt::FrameworkElement const& obj)
		{
			return winrt::unbox_value<winrt::GridLength>(obj.GetValue(LayoutLengthProperty()));
		}

		static void SetLayoutLength(winrt::FrameworkElement const& obj, winrt::GridLength const& value)
		{
			obj.SetValue(LayoutLengthProperty(), winrt::box_value(value));
		}

	private:

		static void OnPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		static void OnAlignmentPropertyChanged(winrt::DependencyObject const& obj, winrt::DependencyProperty const& prop);

		static bool IsSpacingJustified(WrapPanelItemsJustification justification);

		static double GetTotalSpacing(int itemCount, double itemsSpacing, WrapPanelItemsJustification justification);

		struct UVCoord
		{
			double X{};
			double Y{};

		private:
			bool _horizontal{};

		public:
			UVCoord(double x, double y, winrt::Orientation orientation)
				: X(x), Y(y), _horizontal(orientation == winrt::Orientation::Horizontal)
			{}

			UVCoord(winrt::Size const& size, winrt::Orientation orientation)
				: UVCoord(size.Width, size.Height, orientation)
			{}

			double U() const
			{
				return _horizontal ? X : Y;
			}

			void U(double value)
			{
				if (_horizontal)
					X = value;
				else
					Y = value;
			}

			double V() const
			{
				return _horizontal ? Y : X;
			}

			void V(double value)
			{
				if (_horizontal)
					Y = value;
				else
					X = value;
			}

			winrt::Size ToSize() const
			{
				return { static_cast<float>(X), static_cast<float>(Y) };
			}
		};

		struct RowSpec
		{
			double ReservedSpace{ 0.0 };
			double PortionsSum{ 0.0 };
			double MaxInAxisSize{ 0.0 };
			double MaxOffAxisSize{ 0.0 };
			double MinPortionSize{ std::numeric_limits<double>::quiet_NaN() };
			int ItemsCount{ 0 };

			RowSpec() = default;

			RowSpec(winrt::GridLength const& layout, UVCoord const& desiredSize)
			{
				switch (layout.GridUnitType)
				{
				case winrt::GridUnitType::Auto:
					ReservedSpace = desiredSize.U();
					break;

				case winrt::GridUnitType::Pixel:
					ReservedSpace = layout.Value;
					break;

				case winrt::GridUnitType::Star:
					PortionsSum = layout.Value;
					MinPortionSize = desiredSize.U() / layout.Value;
					break;
				}

				MaxInAxisSize = desiredSize.U();
				MaxOffAxisSize = desiredSize.V();
				ItemsCount = 1;
			}

			bool TryAdd(
				RowSpec const& addend,
				double spacing,
				double maxSize,
				WrapPanelItemsStretch stretching,
				WrapPanelItemsJustification justification)
			{
				auto sum = *this + addend;

				if (sum.Measure(spacing, stretching, justification) > maxSize)
					return false;

				*this = sum;
				return true;
			}

			double Measure(double spacing, WrapPanelItemsStretch stretching, WrapPanelItemsJustification justification) const
			{
				double totalSpacing = WrapPanel2::GetTotalSpacing(ItemsCount, spacing, justification);

				bool equalStretching =
					(stretching == WrapPanelItemsStretch::Equal) &&
					WrapPanel2::IsSpacingJustified(justification);

				// Handle equal-sized items child stretching.
				// Without this check, children might become scrunched in the arrange
				// step when they are made equal sizes.
				if (equalStretching && PortionsSum == 0.0)
				{
					return (MaxInAxisSize * ItemsCount) + totalSpacing;
				}

				// Otherwise, base size is reserved space + spacing
				double totalSize = ReservedSpace + totalSpacing;

				// Also add star-sized items if applicable
				if (!std::isnan(MinPortionSize) && !std::isinf(MinPortionSize))
				{
					totalSize += MinPortionSize * PortionsSum;
				}

				return totalSize;
			}

			// operator+
			friend RowSpec operator+(RowSpec const& a, RowSpec const& b)
			{
				RowSpec combined;
				combined.ReservedSpace = a.ReservedSpace + b.ReservedSpace;
				combined.PortionsSum = a.PortionsSum + b.PortionsSum;
				combined.MinPortionSize = std::max(a.MinPortionSize, b.MinPortionSize);
				combined.MaxInAxisSize = std::max(a.MaxInAxisSize, b.MaxInAxisSize);
				combined.MaxOffAxisSize = std::max(a.MaxOffAxisSize, b.MaxOffAxisSize);
				combined.ItemsCount = a.ItemsCount + b.ItemsCount;
				return combined;
			}
		};

		enum class Alignment
		{
			Start,
			Center,
			End,
			Stretch
		};

		void ArrangeRow(UVCoord& pos, RowSpec const& row, UVCoord const& uvFinalSize, std::vector<winrt::FrameworkElement> const& childQueue, size_t& cursor);

		double GetChildSize(winrt::FrameworkElement const& child) const;

		UVCoord GetChildSize(winrt::FrameworkElement const& child, int indexInRow, RowSpec const& row, double portionSize, bool forceStretch);

		double GetStartByAlignment(Alignment alignment, double size, double availableSize);

		Alignment GetAlignment() const;

		Alignment GetOffAlignment() const;

		Alignment GetJustificationAlignment() const;

		bool IsMainAxisStretch(double availableSize) const;

		bool JustifiedSpacing() const;

		WrapPanelItemsJustification RealJustification() const;

		std::vector<RowSpec> _rowSpecs;
		double _longestRowSize = 0;
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct WrapPanel2 : WrapPanel2T<WrapPanel2, implementation::WrapPanel2>{ };
}
