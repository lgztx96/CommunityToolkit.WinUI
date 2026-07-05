#pragma once

#include "WrapPanel.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <algorithm>
#include <vector>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct WrapPanel : WrapPanelT<WrapPanel>
	{
	public:
		WrapPanel() = default;

		winrt::Size MeasureOverride(winrt::Size availableSize);

		winrt::Size ArrangeOverride(winrt::Size finalSize);

		static void LayoutPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		double HorizontalSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(HorizontalSpacingProperty()));
		}

		void HorizontalSpacing(double value)
		{
			SetValue(HorizontalSpacingProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> HorizontalSpacingProperty;

		double VerticalSpacing() const
		{
			return winrt::unbox_value<double>(GetValue(VerticalSpacingProperty));
		}

		void VerticalSpacing(double value)
		{
			SetValue(VerticalSpacingProperty, winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> VerticalSpacingProperty;

		winrt::Orientation Orientation() const
		{
			return winrt::unbox_value<winrt::Orientation>(GetValue(OrientationProperty));
		}

		void Orientation(winrt::Orientation value)
		{
			SetValue(OrientationProperty, winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty;

		winrt::Thickness Padding() const
		{
			return winrt::unbox_value<winrt::Thickness>(GetValue(PaddingProperty()));
		}

		void Padding(winrt::Thickness const& value)
		{
			SetValue(PaddingProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> PaddingProperty;

		winrt::XamlToolkit::WinUI::Controls::StretchChild StretchChild() const
		{
			return winrt::unbox_value<enum StretchChild>(GetValue(StretchChildProperty()));
		}

		void StretchChild(winrt::XamlToolkit::WinUI::Controls::StretchChild value)
		{
			SetValue(StretchChildProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> StretchChildProperty;

	private:
		struct UvMeasure
		{
			static UvMeasure Zero() { return UvMeasure{ 0.0, 0.0 }; }

			double U;
			double V;

			UvMeasure() = default;

			UvMeasure(double u, double v) : U(u), V(v) {}

			UvMeasure(winrt::Orientation orientation, winrt::Size size)
				: UvMeasure(orientation, size.Width, size.Height)
			{
			}

			UvMeasure(winrt::Orientation orientation, double width, double height)
			{
				if (orientation == winrt::Orientation::Horizontal)
				{
					U = width;
					V = height;
				}
				else
				{
					U = height;
					V = width;
				}
			}

			UvMeasure Add(double u, double v)
			{
				return UvMeasure{ U + u, V + v };
			}

			UvMeasure Add(UvMeasure measure)
			{
				return Add(measure.U, measure.V);
			}

			winrt::Size ToSize(winrt::Orientation orientation)
			{
				return orientation == winrt::Orientation::Horizontal
					? Size(static_cast<float>(U), static_cast<float>(V))
					: Size(static_cast<float>(V), static_cast<float>(U));
			}
		};

		struct UvRect
		{
			UvMeasure Position;

			UvMeasure Size;

			winrt::Rect ToRect(winrt::Orientation orientation)
			{
				if (orientation == winrt::Orientation::Vertical)
				{
					return winrt::Rect
					{ 
						static_cast<float>(Position.V), 
						static_cast<float>(Position.U), 
						static_cast<float>(Size.V), 
						static_cast<float>(Size.U)
					};
				}
				else if (orientation == winrt::Orientation::Horizontal)
				{
					return winrt::Rect
					{ 
						static_cast<float>(Position.U), 
						static_cast<float>(Position.V),
						static_cast<float>(Size.U), 
						static_cast<float>(Size.V)
					};
				}
				else
				{
					throw winrt::hresult_invalid_argument(L"The input orientation is not valid.");
				}
			};
		};

		struct Row
		{
			Row() = default;
			Row(std::vector<UvRect> const& childrenRects, UvMeasure size) : ChildrenRects(childrenRects), Size(size)
			{

			}

			std::vector<UvRect> ChildrenRects;

			UvMeasure Size;

			UvRect Rect() const
			{
				if (!ChildrenRects.empty())
				{
					return UvRect{ ChildrenRects.front().Position, Size };
				}
				else
				{
					return UvRect{ UvMeasure(), Size };
				}
			}

			void Add(UvMeasure const& position, UvMeasure const& size)
			{
				UvRect rect{ position, size };
				ChildrenRects.push_back(rect);
				double newU = position.U + size.U;
				double newV = std::max<double>(Size.V, size.V);
				Size = UvMeasure(newU, newV);
			}
		};

		winrt::Size UpdateRows(winrt::Size availableSize);

		std::vector<Row> _rows;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct WrapPanel : WrapPanelT<WrapPanel, implementation::WrapPanel>
	{
	};
}
