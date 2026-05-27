#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#endif
#include "ResizeThumb.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    bool ResizeThumb::IsValidHeight(FrameworkElement const& target, double newHeight, double parentActualHeight)
    {
        auto minHeight = target.MinHeight();
        if (newHeight < 0 || (!std::isnan(minHeight) && newHeight < minHeight))
        {
            return false;
        }

        auto maxHeight = target.MaxHeight();
        if (!std::isnan(maxHeight) && newHeight > maxHeight)
        {
            return false;
        }

        if (newHeight <= parentActualHeight)
        {
            return false;
        }

        return true;
    }

    bool ResizeThumb::IsValidWidth(FrameworkElement const& target, double newWidth, double parentActualWidth)
    {
        auto minWidth = target.MinWidth();
        if (newWidth < 0 || (!std::isnan(minWidth) && newWidth < minWidth))
        {
            return false;
        }

        auto maxWidth = target.MaxWidth();
        if (!std::isnan(maxWidth) && newWidth > maxWidth)
        {
            return false;
        }

        if (newWidth <= parentActualWidth)
        {
            return false;
        }

        return true;
    }
}
