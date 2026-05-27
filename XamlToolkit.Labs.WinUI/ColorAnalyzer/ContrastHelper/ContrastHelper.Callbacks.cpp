#include "pch.h"
#include "winrt_module_imports.h"
#include "ContrastHelper.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    void ContrastHelper::OnOpponentChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        // Subscribe to brush updates if not already
        if (GetCallbackObject(d) == nullptr)
        {
            SubscribeToUpdates(d);
        }

        // Update the actual color to ensure contrast
        ApplyContrastCheck(d);
    }

    void ContrastHelper::OnMinRatioChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        // No opponent has been set, nothing to do
        if (GetCallback(d) == 0)
            return;

        // Update the actual color to ensure contrast
        ApplyContrastCheck(d);
    }

    void ContrastHelper::OnOriginalChangedFromSource(DependencyObject const& obj, DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& prop)
    {
        // The contrast helper is updating the color
        // Ignore the assignment.
        if (_selfUpdate)
            return;

        // Get the original color from the brush.
        // We use the sender, not the obj, because the sender is the object that changed.
        // Use Transparent as a sentinel value if the brush is not a SolidColorBrush
		DependencyProperty dp{ nullptr };
        auto brush = FindBrush(sender, &dp);
        auto color = brush ? brush.Color() : winrt::Microsoft::UI::Colors::Transparent();

        // Update original color
        SetOriginalColor(obj, color);

        // The sender is the Foreground property, not the brush itself.
        // This means the brush changed and our callback on the brush is dead.
        // We need to subscribe to the new brush if it's a SolidColorBrush.
        if (!sender.try_as<SolidColorBrush>())
        {
            // Subscribe to the new brush
            // Notice we're finding the brush on the object, not the sender this time.
            // We may not find a SolidColorBrush, and that's ok.
            auto solidColorBrush = FindBrush(obj, &dp);
            SubscribeToBrushUpdates(obj, solidColorBrush);
        }

        // Apply contrast correction
        ApplyContrastCheck(obj);
    }

    void ContrastHelper::SubscribeToUpdates(DependencyObject const& d)
    {
        // Get the original color from the brush and the property to monitor.
        // Use Transparent as a sentinel value if the brush is not a SolidColorBrush
        DependencyProperty dp{ nullptr };
        auto solidColorBrush = FindBrush(d, &dp);
        auto color = solidColorBrush ? solidColorBrush.Color() : winrt::Microsoft::UI::Colors::Transparent();

        // Record the original color
        SetOriginalColor(d, color);

        // Rhetortical Question: Why don't we return if the solidColorBrush is null?
        // Just because the brush is not a SolidColorBrush doesn't mean we can't monitor the
        // Foreground property. We just can't monitor the brush's Color property

        // If the original is not a SolidColorBrush, we need to monitor the Foreground property
        if (!d.try_as< SolidColorBrush>())
        {
            // Subscribe to updates from the source Foreground
            d.RegisterPropertyChangedCallback(dp, [=](auto&& sender, auto&& prop)
            {
                OnOriginalChangedFromSource(d, sender, prop);
            });
        }

        // Subscribe to updates from the source SolidColorBrush
        // If solidColorBrush is null, this is a no-op
        SubscribeToBrushUpdates(d, solidColorBrush);
    }

    void ContrastHelper::SubscribeToBrushUpdates(DependencyObject const& d, SolidColorBrush const& brush)
    {
        // No brush, nothing to do
        if (brush == nullptr)
            return;

        // Unsubscribe from previous brush if any
        auto oldBrush = GetCallbackObject(d);
        auto oldCallback = GetCallback(d);
        if (oldBrush) oldBrush.UnregisterPropertyChangedCallback(SolidColorBrush::ColorProperty(), oldCallback);

        // Subscribe to updates from the source SolidColorBrush
        auto callback = brush.RegisterPropertyChangedCallback(SolidColorBrush::ColorProperty(), [=](auto&& sender, auto&& prop)
        {
            OnOriginalChangedFromSource(d, sender, prop);
        });

        // Track the callback so we don't double subscribe and can unsubscribe if needed
        SetCallbackObject(d, brush);
        SetCallback(d, callback);
    }
}
