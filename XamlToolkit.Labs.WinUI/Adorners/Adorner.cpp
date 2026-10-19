#include "pch.h"
#include "winrt_module_imports.h"
#include "Adorner.h"
#include "AdornerLayer.h"
#if __has_include("Adorner.g.cpp")
#include "Adorner.g.cpp"
#endif
#include "../Diagnostics/ToolkitProfilerTracing.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    Adorner::Adorner() : _adornedElement(nullptr), AdornerLayer(nullptr)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        XAMLTOOLKIT_TRACE_OBJECT_CREATED(winrt::name_of<class_type>());
    }

    Adorner::~Adorner()
    {
        XAMLTOOLKIT_TRACE_OBJECT_DESTROYED(winrt::name_of<class_type>());
    }

    winrt::fire_and_forget Adorner::final_release(std::unique_ptr<Adorner> self)
    {
        try
        {
            co_await wil::resume_foreground(self->DispatcherQueue());
            if (const auto element = self->_adornedElement.try_as<winrt::FrameworkElement>())
            {
                element.Loaded(self->_adornedElementLoadedToken);
                element.Unloaded(self->_adornedElementUnloadedToken);
                element.SizeChanged(self->_adornedElementSizeChangedToken);
            }
        }
        catch (winrt::hresult_error const& e)
        {
            if (e.code() != RPC_E_WRONG_THREAD)
            {
                throw;
            }
        }
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
            oldfe.SizeChanged(_adornedElementSizeChangedToken);
            oldfe.Loaded(_adornedElementLoadedToken);
            oldfe.Unloaded(_adornedElementUnloadedToken);
            // TODO: Should we explicitly detach the WEL here?
        }

        if (auto newfe = newvalue.try_as<winrt::FrameworkElement>())
        {
            // Track changes to the AdornedElement's size
            _adornedElementSizeChangedToken = newfe.SizeChanged({ this, &Adorner::OnSizeChanged });
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
            _adornedElementLoadedToken = newfe.Loaded({ this, &Adorner::OnAdornedElementLoaded });
            
            // Track if AdornedElement is unloaded
            _adornedElementUnloadedToken = newfe.Unloaded({ this, &Adorner::OnAdornedElementUnloaded });

            overridable().OnAttached();
        }
    }

    void Adorner::OnSizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
    {
        if (auto adornedElement = AdornedElement())
        {
            auto size = adornedElement.ActualSize();
            Width(size.x);
            Height(size.y);
        }
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

        overridable().OnAttached();
    }

    void Adorner::OnAdornedElementUnloaded([[maybe_unused]] winrt::IInspectable const& source, [[maybe_unused]] winrt::RoutedEventArgs const& eventArgs)
    {
        if (AdornerLayer() == nullptr) return;

        overridable().OnDetaching();

        // TODO: Need to evaluate lifecycle a bit more, right now AdornerLayer (via attached property) mostly constrols the lifecycle
        // We could use private WeakReference to AdornedElement to re-listen for Loaded event and still remove/re-add via those
        // We just like to have the harder reference while we're active to make binding/interaction for Adorner implementer easier in XAML...
        //// AdornerLayer.RemoveAdorner(AdornerLayer, this);        
        if (auto element = _adornedElement.try_as<winrt::FrameworkElement>())
        {
            implementation::AdornerLayer::RemoveAdorner(AdornerLayer(), *this);
            implementation::AdornerLayer::AttachAdornerOnLoad(element);
        }
    }

    void Adorner::UpdateLayout()
    {
        OnLayoutUpdated(*this, nullptr);

        base_type::UpdateLayout();
    }
}
