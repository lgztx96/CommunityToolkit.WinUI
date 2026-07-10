#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "CanvasLayout.h"
#if __has_include("CanvasLayout.g.cpp")
#include "CanvasLayout.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	void CanvasLayout::InitializeForContextCore(winrt::VirtualizingLayoutContext const& context)
	{
		base_type::InitializeForContextCore(context);

		if (!context.LayoutState().try_as<ICanvasLayoutState>())
		{
			// Store any state we might need since (in theory) the layout could be in use by multiple
			// elements simultaneously
			context.LayoutState(winrt::make<CanvasLayoutState>());
		}
	}

	void CanvasLayout::UninitializeForContextCore(winrt::VirtualizingLayoutContext const& context)
	{
		base_type::UninitializeForContextCore(context);

		// clear any state
		context.LayoutState(nullptr);
	}

	winrt::Size CanvasLayout::MeasureOverride(winrt::VirtualizingLayoutContext const& context, [[maybe_unused]] winrt::Size availableSize)
	{
        int maxWidth = 0;
        int maxHeight = 0;

        // Get underlying data about positioning of items and determine if in viewport.
		auto realizationRect = context.RealizationRect();
        for (int i = 0; i < context.ItemCount(); i++)
        {
            if (auto item = context.GetItemAt(i).try_as<ICanvasLayoutItem>())
            {
                // See if this item pushes our maximum boundary
                maxWidth = std::max<int>(item.Left() + item.Width(), maxWidth);
                maxHeight = std::max<int>(item.Top() + item.Height(), maxHeight);

                // Calculate if this item is in our current viewport
                float itemLeft = static_cast<float>(item.Left());
                float itemTop = static_cast<float>(item.Top());
                float itemWidth = static_cast<float>(item.Width());
                float itemHeight = static_cast<float>(item.Height());

                winrt::Rect rect(itemLeft, itemTop, itemWidth, itemHeight);
                rect = winrt::RectHelper::Intersect(rect, realizationRect);

                // Check if we're in view now so we can compare to if we were last time.
                bool nowInView = rect.Width > 0 || rect.Height > 0;

                // If it wasn't visible and now is, realize the container
                if (nowInView && !item.IsInView())
                {
                    auto element = context.GetOrCreateElementAt(i);
                    element.Measure(winrt::Size(itemWidth, itemHeight));
                }
                // If it was visible, but now isn't recycle the container
                else if (!nowInView && item.IsInView())
                {
                    auto element = context.GetOrCreateElementAt(i);
                    context.RecycleElement(element);
                }

                // Update our current visibility
                item.IsInView(rect.Width > 0 || rect.Height > 0);
            }
        }

        return winrt::Size(static_cast<float>(maxWidth), static_cast<float>(maxHeight));
	}

	winrt::Size CanvasLayout::ArrangeOverride(winrt::VirtualizingLayoutContext const& context, winrt::Size finalSize)
	{
        for (int i = 0; i < context.ItemCount(); i++)
        {
            if (auto item = context.GetItemAt(i).try_as<ICanvasLayoutItem>(); item && item.IsInView())
            {
                auto container = context.GetOrCreateElementAt(i);
                // Is it better to have cached this from above?
                float itemLeft = static_cast<float>(item.Left());
                float itemTop = static_cast<float>(item.Top());
                float itemWidth = static_cast<float>(item.Width());
                float itemHeight = static_cast<float>(item.Height());
                container.Arrange(winrt::Rect(itemLeft, itemTop, itemWidth, itemHeight));
            }
        }

        return finalSize;
	}
}
