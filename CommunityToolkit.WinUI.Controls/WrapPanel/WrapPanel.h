#pragma once

#include "WrapPanel.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct WrapPanel : WrapPanelT<WrapPanel>
	{
	public:
		WrapPanel() = default;

		Size MeasureOverride(Size availableSize);

		Size ArrangeOverride(Size finalSize);

		static void LayoutPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		double HorizontalSpacing()
		{
			return winrt::unbox_value<double>(GetValue(HorizontalSpacingProperty));
		}
		void HorizontalSpacing(double value)
		{
			SetValue(HorizontalSpacingProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> HorizontalSpacingProperty{
			DependencyProperty::Register(
				L"HorizontalSpacing",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(0.0), &WrapPanel::LayoutPropertyChanged))
		};

		double VerticalSpacing()
		{
			return winrt::unbox_value<double>(GetValue(VerticalSpacingProperty));
		}
		void VerticalSpacing(double value)
		{
			SetValue(VerticalSpacingProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> VerticalSpacingProperty{
			DependencyProperty::Register(
				L"VerticalSpacing",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(0.0), &WrapPanel::LayoutPropertyChanged))
		};

		Orientation Orientation()
		{
			return winrt::unbox_value<enum Orientation>(GetValue(OrientationProperty));
		}
		void Orientation(Microsoft::UI::Xaml::Controls::Orientation value)
		{
			SetValue(OrientationProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> OrientationProperty{
			DependencyProperty::Register(
				L"Orientation",
				winrt::xaml_typename<enum Orientation>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(Orientation::Horizontal), &WrapPanel::LayoutPropertyChanged))
		};

		Thickness Padding()
		{
			return winrt::unbox_value<Thickness>(GetValue(PaddingProperty));
		}
		void Padding(Thickness const& value)
		{
			SetValue(PaddingProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> PaddingProperty{
			DependencyProperty::Register(
				L"Padding",
				winrt::xaml_typename<Thickness>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(Thickness{ 0,0,0,0 }), &WrapPanel::LayoutPropertyChanged))
		};

		winrt::CommunityToolkit::WinUI::Controls::StretchChild StretchChild()
		{
			return winrt::unbox_value<enum StretchChild>(GetValue(StretchChildProperty));
		}
		void StretchChild(winrt::CommunityToolkit::WinUI::Controls::StretchChild value)
		{
			SetValue(StretchChildProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> StretchChildProperty{
			DependencyProperty::Register(
				L"StretchChild",
				winrt::xaml_typename<enum StretchChild>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(StretchChild::None), &WrapPanel::LayoutPropertyChanged))
		};

	private:
		struct UvMeasure
		{
			static UvMeasure Zero() { return UvMeasure{ 0.0, 0.0 }; }
			double U;

			double V;

			UvMeasure() = default;

			UvMeasure(double u, double v) : U(u), V(v) {}

			UvMeasure(Microsoft::UI::Xaml::Controls::Orientation orientation, Size size)
				: UvMeasure(orientation, size.Width, size.Height)
			{
			}

			UvMeasure(Microsoft::UI::Xaml::Controls::Orientation orientation, double width, double height)
			{
				if (orientation == Orientation::Horizontal)
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

			Size ToSize(Microsoft::UI::Xaml::Controls::Orientation orientation)
			{
				return orientation == Microsoft::UI::Xaml::Controls::Orientation::Horizontal
					? Size(static_cast<float>(U), static_cast<float>(V))
					: Size(static_cast<float>(V), static_cast<float>(U));
			}
		};

		struct UvRect
		{
			UvMeasure Position;

			UvMeasure Size;

			Rect ToRect(Microsoft::UI::Xaml::Controls::Orientation orientation)
			{
				if (orientation == winrt::Microsoft::UI::Xaml::Controls::Orientation::Vertical)
				{
					return Rect
					{ 
						static_cast<float>(Position.V), 
						static_cast<float>(Position.U), 
						static_cast<float>(Size.V), 
						static_cast<float>(Size.U)
					};
				}
				else if (orientation == winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal)
				{
					return Rect
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

		Size UpdateRows(Size availableSize);

		std::vector<Row> _rows;
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct WrapPanel : WrapPanelT<WrapPanel, implementation::WrapPanel>
	{
	};
}
