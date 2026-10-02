# Header behavior lifetime

## Ownership defect

An element stores Interaction.Behaviors as an attached dependency property. The
collection owns its behaviors. Both native Behavior implementations stored a
strong AssociatedObject, closing an element -> collection -> behavior -> element
cycle. BehaviorCollection's own weak AssociatedObject did not remove the
behavior's separate strong edge. Interaction's Loaded/Unloaded hooks are disabled
in this fork, and BehaviorBase deliberately does not detach on Unloaded.

StickyHeaderBehavior, FadeHeaderBehavior and QuickReturnHeaderBehavior also
cached a strong ancestor ScrollViewer. Its content contains the header, creating
another independent cycle. A surrounding Page can be destroyed while that
header, scroll content, row templates and their native resources remain alive.

Both Behavior implementations now keep a weak AssociatedObject. HeaderBehaviorBase
keeps a weak ScrollViewer and resolves it locally while assigning animations.
On Unloaded, it revokes header/scroller events, stops the derived animation and
releases the Visual and composition property sets. Loaded reacquires resources
for the current visual tree. A size callback during unloading cannot restart the
animation. The behavior remains in its collection for temporary unload/reload.

This changes ownership and animation resource lifetime, not public IDL or the
scrolling expressions. Other behavior classes may contain their own strong
ancestor caches and need separate investigation; this change does not claim to
fix every behavior lifetime.

## Windows regression cases

In a XAML UI-thread host, keep the behavior alive while dropping an unrooted
header. This checks the reverse ownership edge without relying on Unloaded:

```cpp
void CheckHeaderBehaviorDoesNotOwnHeader()
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::XamlToolkit::WinUI;
    Behaviors::StickyHeaderBehavior behavior;
    winrt::weak_ref<Border> weakHeader;
    {
        Border header;
        weakHeader = header;
        Interactivity::Interaction::GetBehaviors(header).Append(behavior);
        assert(behavior.AssociatedObject() == header);
    }
    assert(!weakHeader.get());
    assert(!behavior.AssociatedObject());
}
```

Repeat with Interactivity.Behavior to cover the runtime-class implementation.
In a real HeaderedTreeView, scroll until the header sticks, remove the host,
wait for deferred XAML work, and check weak references to the header, behavior,
ScrollViewer, DataTable and DataRow. Reload the same host and verify that the
sticky header still follows scrolling. Also test detaching while loaded and
reattaching to another ScrollViewer, plus Fade and QuickReturn behaviors.

OpenNet's PageMemoryDiagnostics records these object categories without owning
them and writes delayed summaries to a text file. The source-level ownership
defect is confirmed; Windows runtime checks are still required to establish the
remaining memory growth after the fix.
