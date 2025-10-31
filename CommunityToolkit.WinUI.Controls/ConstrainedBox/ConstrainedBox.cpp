#include "pch.h"
#include "ConstrainedBox.h"
#if __has_include("ConstrainedBox.g.cpp")
#include "ConstrainedBox.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    Size ConstrainedBox::MeasureOverride(Size availableSize)
    {
        _originalSize = availableSize;

        CalculateConstrainedSize(availableSize);

        _lastMeasuredSize = availableSize;

        // Call base.MeasureOverride so any child elements know what room there is to work with.
        // Don't return this though. An image that hasn't loaded yet for example will request very little space.
        base_type::MeasureOverride(_lastMeasuredSize);
        return _lastMeasuredSize;
    }

    Size ConstrainedBox::ArrangeOverride(Size finalSize)
    {
        // Even though we requested in measure to be a specific size, that doesn't mean our parent
        // panel respected that request. Grid for instance can by default Stretch and if you don't
        // set Horizontal/VerticalAlignment on the control it won't constrain as we expect.
        // We could also be in a StackPanel/ScrollViewer where it wants to provide as much space as possible.
        // However, if we always re-calculate even if we are provided the proper finalSize, this can trigger
        // multiple arrange passes and cause a rounding error in layout. Therefore, we only want to
        // re-calculate if we think we will have a significant impact.
        if (std::abs(finalSize.Width - _lastMeasuredSize.Width) > CalculationTolerance ||
            std::abs(finalSize.Height - _lastMeasuredSize.Height) > CalculationTolerance)
        {
            // Check if we can re-use our measure calculation if we're given effectively
            // the same size as we had in the measure step.
            if (std::abs(finalSize.Width - _originalSize.Width) <= CalculationTolerance &&
                std::abs(finalSize.Height - _originalSize.Height) <= CalculationTolerance)
            {
                finalSize = _lastMeasuredSize;
            }
            else
            {
                CalculateConstrainedSize(finalSize);

                // Copy again so if Arrange is re-triggered we won't re-re-calculate.
                _lastMeasuredSize = finalSize;
            }
        }

        return base_type::ArrangeOverride(finalSize);
    }

    void ConstrainedBox::CalculateConstrainedSize(Size& availableSize)
    {
        // 1) We check for Infinity, in the case we have no constraint from parent
        //    we'll request the child's measurements first, so we can use that as
        //    a starting point to constrain it's dimensions based on the criteria
        //    set in our properties.
        auto hasWidth = IsPositiveRealNumber(availableSize.Width);
        auto hasHeight = IsPositiveRealNumber(availableSize.Height);

        if (!hasWidth && !hasHeight)
        {
            // We have infinite space, like a ScrollViewer with both scrolling directions
            // Ask child how big they want to be first.
            availableSize = base_type::MeasureOverride(availableSize);

            hasWidth = IsPositiveRealNumber(availableSize.Width);
            hasHeight = IsPositiveRealNumber(availableSize.Height);

            if (!hasWidth && !hasHeight)
            {
                // At this point we have no way to determine a constraint, the Panel won't do anything
                // This should be rare? We don't really have a way to provide a warning here.
                return;
            }
        }

        // 2) Apply Scales to constrain based on a percentage
        // --------------------------------------------------
        availableSize.Width *= static_cast<float>(ScaleX());
        availableSize.Height *= static_cast<float>(ScaleY());

        // 3) Apply Multiples
        // ------------------
        // These floor the Width/Height values to the nearest multiple of the property (if set).
        // For instance you may have a responsive 4x4 repeated checkerboard pattern for transparency and
        // want to snap to the nearest interval of 4 so the checkerboard is consistency across the layout.
        if (hasWidth &&
            ReadLocalValue(MultipleXProperty) != DependencyProperty::UnsetValue() &&
            MultipleX() > 0)
        {
            availableSize.Width -= static_cast<float>(std::fmod(availableSize.Width, MultipleX()));
        }

        if (hasHeight &&
            ReadLocalValue(MultipleYProperty) != DependencyProperty::UnsetValue() &&
            MultipleY() > 0)
        {
            availableSize.Height -= static_cast<float>(std::fmod(availableSize.Height, MultipleY()));
        }

        // 4) Apply AspectRatio
        // --------------------
        // Finally, we apply the AspectRatio property after we've determined the general
        // area we have to work with based on the other constraints.
        // Devs should be careful if they use both a MultipleX&Y that the AspectRatio is also
        // within that same ratio. The Ratio will take preference here as the last step.
        if (ReadLocalValue(AspectRatioProperty) == DependencyProperty::UnsetValue())
        {
            // Skip as last constraint if we have nothing to do.
            return;
        }

        // Calculate the Aspect Ratio constraint based on the newly scaled size.
        auto currentAspect = availableSize.Width / availableSize.Height;

        auto aspectRatio = AspectRatio().Value();

        if (!hasWidth)
        {
            // If available width is infinite, set width based on height
            availableSize.Width = availableSize.Height * static_cast<float>(aspectRatio);
        }
        else if (!hasHeight)
        {
            // If avalable height is infinite, set height based on width
            availableSize.Height = availableSize.Width / static_cast<float>(aspectRatio);
        }
        else if (currentAspect > aspectRatio)
        {
            // If the container aspect ratio is wider than our aspect ratio, set width based on height
            availableSize.Width = availableSize.Height * static_cast<float>(aspectRatio);
        }
        else
        {
            // If the container aspect ratio is taller than our aspect ratio, set height based on width
            availableSize.Height = availableSize.Width / static_cast<float>(aspectRatio);
        }
    }

    void ConstrainedBox::ConstraintPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        auto panel = d.try_as<class_type>();

        if (!panel)
        {
            return;
		}

		auto self = winrt::get_self<ConstrainedBox>(panel)->get_strong();
        if (!self->_propertyUpdating)
        {
            self->_propertyUpdating = true;

            self->CoerceValues();

            self->InvalidateMeasure();

            self->_propertyUpdating = false;
        }
    }

    void ConstrainedBox::CoerceValues()
    {
        // Check if scale properties are in range
        auto scaleX = ScaleX();
        if (!std::isnan(scaleX))
        {
            if (scaleX < 0)
            {
                scaleX = 0;
            }
            else if (scaleX > 1.0)
            {
                scaleX = 1.0;
            }
        }
        else
        {
            scaleX = 1.0;
        }
        auto scaleY = ScaleY();
        if (!std::isnan(scaleY))
        {
            if (scaleY < 0)
            {
                scaleY = 0;
            }
            else if (scaleY > 1.0)
            {
                scaleY = 1.0;
            }
        }
        else
        {
            scaleY = 1.0;
        }

        // Clear invalid values less than 0 for other properties
        if (auto value = ReadLocalValue(MultipleXProperty).try_as<int>(); value && *value <= 0)
        {
            ClearValue(MultipleXProperty);
        }

        if (auto value2 = ReadLocalValue(MultipleYProperty).try_as<int>(); value2 && value2 <= 0)
        {
            ClearValue(MultipleYProperty);
        }

        if (auto ratio = ReadLocalValue(AspectRatioProperty).try_as<struct AspectRatio>(); ratio && ratio <= 0)
        {
            ClearValue(AspectRatioProperty);
        }
    }
}
