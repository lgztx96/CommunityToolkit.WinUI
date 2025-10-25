#include "pch.h"
#include "RadialGauge.h"
#if __has_include("RadialGauge.g.cpp")
#include "RadialGauge.g.cpp"
#endif
#include "RadialGaugeAutomationPeer.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Foundation::Numerics;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	RadialGauge::RadialGauge()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		SmallChange(1);
		LargeChange(10);

		SetKeyboardAccelerators();
	}

	void RadialGauge::RadialGauge_Unloaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		// TODO: We should just use a WeakEventListener for ThemeChanged here, but ours currently doesn't support it.
		// See proposal for general helper here: https://github.com/CommunityToolkit/dotnet/issues/404
		ThemeListener.ThemeChanged(_themeChangedToken);
		PointerReleased(_pointerReleasedToken);
		IsEnabledChanged(_isEnabledChangedToken);
		Unloaded(_unloadedToken);
	}

	void RadialGauge::OnApplyTemplate()
	{
		ThemeListener.ThemeChanged(_themeChangedToken);
		PointerReleased(_pointerReleasedToken);
		IsEnabledChanged(_isEnabledChangedToken);
		Unloaded(_unloadedToken);

		// Remember local brushes.
		_needleBrush = ReadLocalValue(NeedleBrushProperty).try_as<SolidColorBrush>();
		_needleBorderBrush = ReadLocalValue(NeedleBorderBrushProperty).try_as<SolidColorBrush>();
		_trailBrush = ReadLocalValue(TrailBrushProperty).try_as<SolidColorBrush>();
		_scaleBrush = ReadLocalValue(ScaleBrushProperty).try_as<SolidColorBrush>();
		_scaleTickBrush = ReadLocalValue(ScaleTickBrushProperty).try_as<SolidColorBrush>();
		_tickBrush = ReadLocalValue(TickBrushProperty).try_as<SolidColorBrush>();
		_foreground = ReadLocalValue(Control::ForegroundProperty()).try_as<SolidColorBrush>();

		_pointerReleasedToken = PointerReleased({ this, &RadialGauge::RadialGauge_PointerReleased });
		ThemeListener.ThemeChanged({ get_weak(), &RadialGauge::ThemeListener_ThemeChanged });
		_isEnabledChangedToken = IsEnabledChanged({ this, &RadialGauge::RadialGauge_IsEnabledChanged });
		_unloadedToken = Unloaded({ this, &RadialGauge::RadialGauge_Unloaded });

		// Apply color scheme.
		OnColorsChanged();
		OnUnitChanged(*this);
		OnEnabledChanged();
		OnInteractivityChanged(*this);
		base_type::OnApplyTemplate();
	}

	void RadialGauge::ThemeListener_ThemeChanged([[maybe_unused]] struct ThemeListener const& sender, [[maybe_unused]] IInspectable const& e)
	{
		OnColorsChanged();
	}

	void RadialGauge::RadialGauge_IsEnabledChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		OnEnabledChanged();
	}

	AutomationPeer RadialGauge::OnCreateAutomationPeer()
	{
		return winrt::make<RadialGaugeAutomationPeer>(*this);
	}

	void RadialGauge::OnMinimumChanged(double oldMinimum, double newMinimum)
	{
		base_type::OnMinimumChanged(oldMinimum, newMinimum);
		OnScaleChanged(*this);
	}

	void RadialGauge::OnMaximumChanged(double oldMaximum, double newMaximum)
	{
		base_type::OnMaximumChanged(oldMaximum, newMaximum);
		OnScaleChanged(*this);
	}

	void RadialGauge::OnValueChanged(double oldValue, double newValue)
	{
		OnValueChanged(*this);
		base_type::OnValueChanged(oldValue, newValue);
		if (AutomationPeer::ListenerExists(AutomationEvents::LiveRegionChanged))
		{
			if (auto peer = FrameworkElementAutomationPeer::FromElement(*this)
				.try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGaugeAutomationPeer>())
			{
				peer.RaiseValueChangedEvent(oldValue, newValue);
			}
		}
	}

	void RadialGauge::OnValueChanged(DependencyObject const& d)
	{
		auto control = d.try_as<class_type>();
		auto radialGauge = winrt::get_self<RadialGauge>(control)->get_strong();

		if (!std::isnan(radialGauge->Value()))
		{
			if (radialGauge->StepSize() != 0)
			{
				radialGauge->Value(radialGauge->RoundToMultiple(radialGauge->Value(), radialGauge->StepSize()));
			}

			auto middleOfScale = 100 - radialGauge->ScalePadding() - (radialGauge->ScaleWidth() / 2);
			if (middleOfScale >= 0)
			{
				auto valueText = radialGauge->GetTemplateChild(ValueTextPartName).try_as<TextBlock>();
				radialGauge->ValueAngle(radialGauge->ValueToAngle(radialGauge->Value()));

				// Needle
				if (radialGauge->_needle != nullptr)
				{
					radialGauge->_needle.RotationAngleInDegrees(static_cast<float>(radialGauge->ValueAngle()));
				}

				// Trail
				auto trail = radialGauge->GetTemplateChild(TrailPartName).try_as<Path>();
				if (trail != nullptr)
				{
					if (radialGauge->ValueAngle() > radialGauge->NormalizedMinAngle())
					{
						trail.Visibility(Visibility::Visible);

						if (radialGauge->ValueAngle() - radialGauge->NormalizedMinAngle() == 360)
						{
							// Draw full circle.
							EllipseGeometry eg;
							eg.Center(Point(100, 100));
							eg.RadiusX(100 - radialGauge->ScalePadding() - (radialGauge->ScaleWidth() / 2));
							eg.RadiusY(eg.RadiusX());
							trail.Data(eg);
						}
						else
						{
							// Draw arc.
							PathGeometry pg;
							PathFigure pf;
							pf.IsClosed(false);
							pf.StartPoint(radialGauge->ScalePoint(radialGauge->NormalizedMinAngle(), middleOfScale));

							ArcSegment seg;
							seg.SweepDirection(SweepDirection::Clockwise);
							seg.IsLargeArc(radialGauge->ValueAngle() > (180 + radialGauge->NormalizedMinAngle()));
							seg.Size(Size(static_cast<float>(middleOfScale), static_cast<float>(middleOfScale)));
							seg.Point(radialGauge->ScalePoint(std::min<double>(radialGauge->ValueAngle(), radialGauge->NormalizedMaxAngle()), middleOfScale));  // On overflow, stop trail at MaxAngle.

							pf.Segments().Append(seg);
							pg.Figures().Append(pf);
							trail.Data(pg);
						}
					}
					else
					{
						trail.Visibility(Visibility::Collapsed);
					}
				}

				// Value Text
				if (valueText != nullptr)
				{
					double value = radialGauge->Value();
					auto args = std::make_wformat_args(value);
					auto text = std::vformat(std::wstring_view(radialGauge->ValueStringFormat()), args);
					valueText.Text(text);
				}
				ToolTipService::SetToolTip(control, winrt::box_value(radialGauge->Value()));
			}
		}
	}

	void RadialGauge::OnInteractivityChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		OnInteractivityChanged(d);
	}

	void RadialGauge::OnInteractivityChanged(DependencyObject const& d)
	{
		auto control = d.try_as<class_type>();
		auto radialGauge = winrt::get_self<RadialGauge>(control)->get_strong();

		if (radialGauge->IsInteractive())
		{
			radialGauge->Tapped({ radialGauge->get_weak(), &RadialGauge::RadialGauge_Tapped });
			radialGauge->ManipulationDelta({ radialGauge->get_weak(), &RadialGauge::RadialGauge_ManipulationDelta });
			radialGauge->ManipulationMode(ManipulationModes::TranslateX | ManipulationModes::TranslateY);
		}
		else
		{
			radialGauge->Tapped(radialGauge->_tappedToken);
			radialGauge->ManipulationDelta(radialGauge->_manipulationDeltaToken);
			radialGauge->ManipulationMode(ManipulationModes::None);
		}
	}

	void RadialGauge::OnScaleChanged(DependencyObject const& d)
	{
		auto control = d.try_as<class_type>();
		auto radialGauge = winrt::get_self<RadialGauge>(control)->get_strong();

		radialGauge->UpdateNormalizedAngles();

		if (auto scale = radialGauge->GetTemplateChild(ScalePartName).try_as<Path>())
		{
			if (radialGauge->NormalizedMaxAngle() - radialGauge->NormalizedMinAngle() == 360)
			{
				// Draw full circle.
				EllipseGeometry eg;
				eg.Center(Point(100, 100));
				eg.RadiusX(100 - radialGauge->ScalePadding() - (radialGauge->ScaleWidth() / 2));

				eg.RadiusY(eg.RadiusX());
				scale.Data(eg);
			}
			else
			{
				// Draw arc.
				PathGeometry pg;
				PathFigure pf;
				pf.IsClosed(false);

				auto middleOfScale = 100 - radialGauge->ScalePadding() - (radialGauge->ScaleWidth() / 2);
				pf.StartPoint(radialGauge->ScalePoint(radialGauge->NormalizedMinAngle(), middleOfScale));
				ArcSegment seg;

				seg.SweepDirection(SweepDirection::Clockwise);
				seg.IsLargeArc(radialGauge->NormalizedMaxAngle() > (radialGauge->NormalizedMinAngle() + 180));
				seg.Size(Size(static_cast<float>(middleOfScale), static_cast<float>(middleOfScale)));
				seg.Point(radialGauge->ScalePoint(radialGauge->NormalizedMaxAngle(), middleOfScale));

				pf.Segments().Append(seg);
				pg.Figures().Append(pf);
				scale.Data(pg);
			}

			if (!DesignTimeHelpers::IsRunningInLegacyDesignerMode())
			{
				OnFaceChanged(control);
			}
		}
	}

	void RadialGauge::OnFaceChanged(DependencyObject const& d)
	{
		auto control = d.try_as<class_type>();
		auto radialGauge = winrt::get_self<RadialGauge>(control)->get_strong();

		auto container = radialGauge->GetTemplateChild(ContainerPartName).try_as<Grid>();

		if (container == nullptr || DesignTimeHelpers::IsRunningInLegacyDesignerMode())
		{
			// Bad template.
			return;
		}

		// TO DO: Replace with _radialGauge->_root = container.GetVisual();
		auto hostVisual = ElementCompositionPreview::GetElementVisual(container);
		auto root = hostVisual.Compositor().CreateContainerVisual();
		ElementCompositionPreview::SetElementChildVisual(container, root);
		radialGauge->_root = root;
		//

		radialGauge->_root.Children().RemoveAll();
		radialGauge->_compositor = radialGauge->_root.Compositor();

		if (radialGauge->TickSpacing() > 0)
		{
			// Ticks.
			auto tick = radialGauge->_compositor.CreateShapeVisual();
			tick.Size(float2((float)(radialGauge->Height()), (float)(radialGauge->Width())));
			tick.BorderMode(CompositionBorderMode::Soft);
			tick.Opacity((float)radialGauge->TickBrush().Opacity());

			auto roundedTickRectangle = radialGauge->_compositor.CreateRoundedRectangleGeometry();
			roundedTickRectangle.Size(float2((float)radialGauge->TickWidth(), (float)radialGauge->TickLength()));
			roundedTickRectangle.CornerRadius(float2((float)radialGauge->TickCornerRadius(), (float)radialGauge->TickCornerRadius()));

			for (double i = radialGauge->Minimum(); i <= radialGauge->Maximum(); i += radialGauge->TickSpacing())
			{
				auto tickSpriteShape = radialGauge->_compositor.CreateSpriteShape(roundedTickRectangle);
				tickSpriteShape.FillBrush(radialGauge->_compositor.CreateColorBrush(radialGauge->TickBrush().Color()));
				tickSpriteShape.Offset(float2(100 - ((float)radialGauge->TickWidth() / 2), (float)radialGauge->TickPadding()));
				tickSpriteShape.CenterPoint(float2((float)radialGauge->TickWidth() / 2, 100 - (float)radialGauge->TickPadding()));
				tickSpriteShape.RotationAngleInDegrees((float)radialGauge->ValueToAngle(i));
				tick.Shapes().Append(tickSpriteShape);
			}

			radialGauge->_root.Children().InsertAtTop(tick);

			// Scale Ticks.
			auto scaleTick = radialGauge->_compositor.CreateShapeVisual();
			scaleTick.Size(float2((float)(radialGauge->Height()), (float)(radialGauge->Width())));
			scaleTick.BorderMode(CompositionBorderMode::Soft);
			scaleTick.Opacity((float)radialGauge->ScaleTickBrush().Opacity());

			auto roundedScaleTickRectangle = radialGauge->_compositor.CreateRoundedRectangleGeometry();
			roundedScaleTickRectangle.Size(float2((float)radialGauge->ScaleTickWidth(), (float)radialGauge->ScaleTickLength()));
			roundedScaleTickRectangle.CornerRadius(float2((float)radialGauge->ScaleTickCornerRadius(), (float)radialGauge->ScaleTickCornerRadius()));

			for (double i = radialGauge->Minimum(); i <= radialGauge->Maximum(); i += radialGauge->TickSpacing())
			{
				auto scaleTickSpriteShape = radialGauge->_compositor.CreateSpriteShape(roundedScaleTickRectangle);
				scaleTickSpriteShape.FillBrush(radialGauge->_compositor.CreateColorBrush(radialGauge->ScaleTickBrush().Color()));
				scaleTickSpriteShape.Offset(float2(100 - ((float)radialGauge->ScaleTickWidth() / 2), (float)radialGauge->ScalePadding()));
				scaleTickSpriteShape.CenterPoint(float2((float)radialGauge->ScaleTickWidth() / 2, 100 - (float)radialGauge->ScalePadding()));
				scaleTickSpriteShape.RotationAngleInDegrees((float)radialGauge->ValueToAngle(i));
				scaleTick.Shapes().Append(scaleTickSpriteShape);
			}
			radialGauge->_root.Children().InsertAtTop(scaleTick);
		}

		// Needle.
		auto shapeVisual = radialGauge->_compositor.CreateShapeVisual();
		shapeVisual.Size(float2((float)radialGauge->Height(), (float)radialGauge->Width()));
		shapeVisual.BorderMode(CompositionBorderMode::Soft);
		shapeVisual.Opacity((float)radialGauge->NeedleBrush().Opacity());
		auto roundedNeedleRectangle = radialGauge->_compositor.CreateRoundedRectangleGeometry();
		roundedNeedleRectangle.Size(float2((float)radialGauge->NeedleWidth(), (float)radialGauge->NeedleLength()));
		roundedNeedleRectangle.CornerRadius(float2((float)radialGauge->NeedleWidth() / 2, (float)radialGauge->NeedleWidth() / 2));
		radialGauge->_needle = radialGauge->_compositor.CreateSpriteShape(roundedNeedleRectangle);
		radialGauge->_needle.FillBrush(radialGauge->_compositor.CreateColorBrush(radialGauge->NeedleBrush().Color()));
		radialGauge->_needle.CenterPoint(float2((float)radialGauge->NeedleWidth() / 2, (float)radialGauge->NeedleLength()));
		radialGauge->_needle.Offset(float2(100 - ((float)radialGauge->NeedleWidth() / 2), 100 - (float)radialGauge->NeedleLength()));
		radialGauge->_needle.StrokeThickness((float)radialGauge->NeedleBorderThickness());
		radialGauge->_needle.StrokeBrush(radialGauge->_compositor.CreateColorBrush(radialGauge->NeedleBorderBrush().Color()));
		shapeVisual.Shapes().Append(radialGauge->_needle);

		radialGauge->_root.Children().InsertAtTop(shapeVisual);

		OnValueChanged(control);
	}

	void RadialGauge::OnColorsChanged()
	{
		if (ThemeListener.IsHighContrast())
		{
			// Apply High Contrast Theme.
			ClearBrush(_needleBrush, NeedleBrushProperty);
			ClearBrush(_needleBorderBrush, NeedleBorderBrushProperty);
			ClearBrush(_trailBrush, TrailBrushProperty);
			ClearBrush(_scaleBrush, ScaleBrushProperty);
			ClearBrush(_scaleTickBrush, ScaleTickBrushProperty);
			ClearBrush(_tickBrush, TickBrushProperty);
			ClearBrush(_foreground, Control::ForegroundProperty());
		}
		else
		{
			// Apply User Defined or Default Theme.
			RestoreBrush(_needleBrush, NeedleBrushProperty);
			RestoreBrush(_needleBorderBrush, NeedleBorderBrushProperty);
			RestoreBrush(_trailBrush, TrailBrushProperty);
			RestoreBrush(_scaleBrush, ScaleBrushProperty);
			RestoreBrush(_scaleTickBrush, ScaleTickBrushProperty);
			RestoreBrush(_tickBrush, TickBrushProperty);
			RestoreBrush(_foreground, Control::ForegroundProperty());
		}

		OnScaleChanged(*this);
	}

	void RadialGauge::OnEnabledChanged()
	{
		VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, true);
		// OnColorsChanged();
	}

	void RadialGauge::OnUnitChanged(DependencyObject const& d)
	{
		auto control = d.try_as<class_type>();
		auto radialGauge = winrt::get_self<RadialGauge>(control)->get_strong();

		if (auto unitTextBlock = radialGauge->GetTemplateChild(UnitTextPartName).try_as<TextBlock>())
		{
			if (radialGauge->Unit().empty())
			{
				unitTextBlock.Visibility(Visibility::Collapsed);
			}
			else
			{
				unitTextBlock.Visibility(Visibility::Visible);
			}
		}
	}

	void RadialGauge::ClearBrush(Brush const& brush, DependencyProperty const& prop)
	{
		if (brush != nullptr)
		{
			ClearValue(prop);
		}
	}

	void RadialGauge::RestoreBrush(Brush const& source, DependencyProperty const& prop)
	{
		if (source != nullptr)
		{
			SetValue(prop, source);
		}
	}

	void RadialGauge::UpdateNormalizedAngles()
	{
		auto result = Mod(MinAngle(), 360);

		if (result >= 180)
		{
			result = result - 360;
		}

		_normalizedMinAngle = result;

		result = Mod(MaxAngle(), 360);

		if (result < 180)
		{
			result = result + 360;
		}

		if (result > NormalizedMinAngle() + 360)
		{
			result = result - 360;
		}

		_normalizedMaxAngle = result;
	}

	void RadialGauge::SetGaugeValueFromPoint(Point const& p)
	{
		auto pt = Point(static_cast<float>(p.X - (ActualWidth() / 2)), static_cast<float>(-p.Y + (ActualHeight() / 2)));

		auto angle = std::atan2(pt.X, pt.Y) / Degrees2Radians;
		auto divider = Mod(NormalizedMaxAngle() - NormalizedMinAngle(), 360);
		if (divider == 0)
		{
			divider = 360;
		}

		auto value = Minimum() + ((Maximum() - Minimum()) * Mod(angle - NormalizedMinAngle(), 360) / divider);
		if (value < Minimum() || value > Maximum())
		{
			// Ignore positions outside the scale angle.
			return;
		}

		Value(RoundToMultiple(value, StepSize()));
	}

	Point RadialGauge::ScalePoint(double angle, double middleOfScale)
	{
		return Point(
			static_cast<float>(100 + (std::sin(Degrees2Radians * angle) * middleOfScale)),
			static_cast<float>(100 - (std::cos(Degrees2Radians * angle) * middleOfScale)));
	}

	double RadialGauge::ValueToAngle(double value)
	{
		// Off-scale on the left.
		if (value < Minimum())
		{
			return MinAngle();
		}

		// Off-scale on the right.
		if (value > Maximum())
		{
			return MaxAngle();
		}

		return ((value - Minimum()) / (Maximum() - Minimum()) * (NormalizedMaxAngle() - NormalizedMinAngle())) + NormalizedMinAngle();
	}

	double RadialGauge::Mod(double number, double divider)
	{
		auto result = std::fmod(number, divider);
		result = result < 0 ? result + divider : result;
		return result;
	}

	double RadialGauge::RoundToMultiple(double number, double multiple)
	{
		auto modulo = std::fmod(number, multiple);
		if (std::isnan(modulo))
		{
			return number;
		}

		if ((multiple - modulo) <= modulo)
		{
			modulo = multiple - modulo;
		}
		else
		{
			modulo *= -1;
		}

		return number + modulo;
	}

	void RadialGauge::OnUnitChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		OnUnitChanged(d);
	}

	void RadialGauge::OnScaleChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		OnScaleChanged(d);
	}

	void RadialGauge::OnFaceChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		if (!DesignTimeHelpers::IsRunningInLegacyDesignerMode())
		{
			OnFaceChanged(d);
		}
	}
}
