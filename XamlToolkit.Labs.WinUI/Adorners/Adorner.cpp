#include "pch.h"
#include "winrt_module_imports.h"
#include "Adorner.h"
#if __has_include("Adorner.g.cpp")
#include "Adorner.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    Adorner::Adorner() : _adornedElement(nullptr), AdornerLayer(nullptr)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    winrt::UIElement Adorner::AdornedElement() const noexcept
    {
        return _adornedElement;
    }

    void Adorner::AdornedElement(winrt::UIElement const& value)
    {
        auto oldvalue = _adornedElement;
        _adornedElement = value;
        OnAdornedElementChanged(oldvalue, value);
    }

    void Adorner::OnAdornedElementChanged(winrt::UIElement const& oldvalue, winrt::UIElement const& newvalue)
    {
        if (auto oldfe = oldvalue.try_as<winrt::FrameworkElement>())
        {
            _adornedElementSizeChangedRevoker.revoke();
            _adornedElementLoadedRevoker.revoke();
            _adornedElementUnloadedRevoker.revoke();
            // TODO: Should we explicitly detach the WEL here?
        }

        if (auto newfe = newvalue.try_as<winrt::FrameworkElement>())
        {
            // Track changes to the AdornedElement's size
            _adornedElementSizeChangedRevoker = newfe.SizeChanged(winrt::auto_revoke, { this, &Adorner::OnSizeChanged });
            // Track changes to the AdornedElement's layout
            // Note: This is pretty spammy, thinking we don't need this?
            /*var weakPropertyChangedListenerLayout = new WeakEventListener<Adorner, object?, object>(this)
            {
                OnEventAction = static (instance, source, eventArgs) => instance.OnLayoutUpdated(source, eventArgs),
                OnDetachAction = (weakEventListener) => newfe.LayoutUpdated -= weakEventListener.OnEvent // Use Local References Only
            };
            newfe.LayoutUpdated += weakPropertyChangedListenerLayout.OnEvent;*/

            // Initial size & layout update
            OnSizeChanged(nullptr, nullptr);
            OnLayoutUpdated(nullptr, nullptr);

            // Track if AdornedElement is loaded
            _adornedElementLoadedRevoker = newfe.Loaded(winrt::auto_revoke, { this, &Adorner::OnAdornedElementLoaded });
            
            // Track if AdornedElement is unloaded
            _adornedElementUnloadedRevoker = newfe.Unloaded(winrt::auto_revoke, { this, &Adorner::OnAdornedElementUnloaded });

            OnAttached();
        }
    }

    void Adorner::OnSizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
    {
        if (AdornedElement() == nullptr) return;
        auto size = AdornedElement().ActualSize();
        Width(size.x);
        Height(size.y);
    }

    void Adorner::OnLayoutUpdated([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& e)
    {
        // Note: Also called by the parent AdornerLayer when its size changes
        if (AdornerLayer() && AdornedElement())
        {
            auto coord = winrt::XamlToolkit::WinUI::UIElementExtensions::CoordinatesTo(AdornerLayer(), _adornedElement);

            winrt::Canvas::SetLeft(*this, coord.X);
            winrt::Canvas::SetTop(*this, coord.Y);

            // Also update size
            OnSizeChanged(*this, nullptr);
        }
    }

    void Adorner::OnAdornedElementLoaded([[maybe_unused]] winrt::IInspectable const& source, [[maybe_unused]] winrt::RoutedEventArgs const& eventArgs)
    {
        if (AdornerLayer() == nullptr) return;

        OnAttached();
    }

    void Adorner::OnAdornedElementUnloaded([[maybe_unused]] winrt::IInspectable const& source, [[maybe_unused]] winrt::RoutedEventArgs const& eventArgs)
    {
        if (AdornerLayer() == nullptr) return;

        OnDetaching();

        // TODO: Need to evaluate lifecycle a bit more, right now AdornerLayer (via attached property) mostly constrols the lifecycle
        // We could use private WeakReference to AdornedElement to re-listen for Loaded event and still remove/re-add via those
        // We just like to have the harder reference while we're active to make binding/interaction for Adorner implementer easier in XAML...
        //// AdornerLayer.RemoveAdorner(AdornerLayer, this);        
    }

    void Adorner::UpdateLayout()
    {
        OnLayoutUpdated(*this, nullptr);

        base_type::UpdateLayout();
    }
}
