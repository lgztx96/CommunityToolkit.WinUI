#include "pch.h"
#include "winrt_module_imports.h"
#include "ResizeElementAdorner.h"
#if __has_include("ResizeElementAdorner.g.cpp")
#include "ResizeElementAdorner.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    ResizeElementAdorner::ResizeElementAdorner()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        DataContext(*this);
    }

    void ResizeElementAdorner::OnApplyTemplate()
    {
        OnDetaching();

        TopThumbPart = GetTemplateChild(TopThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();
        BottomThumbPart = GetTemplateChild(BottomThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();
        LeftThumbPart = GetTemplateChild(LeftThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();
        RightThumbPart = GetTemplateChild(RightThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();
        TopLeftThumbPart = GetTemplateChild(TopLeftThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();
        TopRightThumbPart = GetTemplateChild(TopRightThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();
        BottomLeftThumbPart = GetTemplateChild(BottomLeftThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();
        BottomRightThumbPart = GetTemplateChild(BottomRightThumbPartName).try_as<winrt::XamlToolkit::Labs::WinUI::ResizeThumb>();

        // OnApplyTemplate can be called after OnAttached, especially if the Adorner isn't initially visible, so we need to re-apply the TargetControl here.
        if (AdornedElement())
        {
            // Guard this incase we're getting removed from the visual tree...
            // Not sure if this is a bug in the Adorner lifecycle or not or specific to how we've set this up here.
            OnAttached();
        }
    }

    void ResizeElementAdorner::OnAttached()
    {
        auto element = AdornedElement();
        if (element == nullptr)
        {
            if (auto baseElement = base_type::AdornedElement())
            {
                throw winrt::hresult_no_interface(
                    winrt::format(L"AdornedElement {} is not of type FrameworkElement", winrt::get_class_name(baseElement)).c_str());
            }
        }

        std::array<winrt::XamlToolkit::Labs::WinUI::ResizeThumb, 8> parts =
        {
            TopThumbPart,
            BottomThumbPart,
            LeftThumbPart,
            RightThumbPart,
            TopLeftThumbPart,
            TopRightThumbPart,
            BottomLeftThumbPart,
            BottomRightThumbPart
        };

        for (size_t i = 0; i < parts.size(); i++)
        {
            if (const auto& part = parts[i])
            {
                part.TargetControl(AdornedElement());
                _thumbTokens[i] = part.TargetControlResized({ this, &ResizeElementAdorner::OnTargetControlResized });
            }
        }

        // If the adorned element moves than we need to update our layout.
        _manipulationDeltaRevoker = element.ManipulationDelta(winrt::auto_revoke, { this, &ResizeElementAdorner::OnTargetManipulated });
    }

    void ResizeElementAdorner::OnDetaching()
    {
        base_type::OnDetaching();

        std::array<winrt::XamlToolkit::Labs::WinUI::ResizeThumb, 8> parts =
        {
            TopThumbPart,
            BottomThumbPart,
            LeftThumbPart,
            RightThumbPart,
            TopLeftThumbPart,
            TopRightThumbPart,
            BottomLeftThumbPart,
            BottomRightThumbPart
        };

        for (size_t i = 0; i < parts.size(); i++)
        {
            if (const auto& part = parts[i])
            {
                part.TargetControlResized(_thumbTokens[i]);
                part.TargetControl(nullptr);
            }
        }

        _manipulationDeltaRevoker.revoke();
    }

    void ResizeElementAdorner::OnTargetManipulated([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ManipulationDeltaRoutedEventArgs const& e)
    {
        // If the underlying adorned element moves than we need to update our layout.
        UpdateLayout();
    }

    void ResizeElementAdorner::OnTargetControlResized([[maybe_unused]] winrt::XamlToolkit::Labs::WinUI::ResizeThumb const& sender, [[maybe_unused]] ITargetControlResizedEventArgs const& args)
    {
        // TODO: Investigate more
        // Note: I'm not sure why the AdornedElement's SizeChanged/LayoutUpdate isn't getting triggered by our changes...
        // So for now, we'll just force a layout update here of the Adorner itself to realign to the new size of the AdornedElement.
        UpdateLayout();
    }
}
