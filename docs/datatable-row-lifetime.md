# DataTable row lifetime

DataTable records rows during measurement so column changes can invalidate
them. That registry must not own rows: a container may be measured and discarded
without receiving Loaded/Unloaded. The previous strong row registry and strong
DataRow header references formed a cycle that relied on Unloaded to break it.

The registry now holds weak row references keyed by interface identity. Rows()
returns a temporary strong snapshot and removes expired entries. DataRow's
cached table/panel references are also weak and are resolved for each layout
pass. Unloaded still unregisters a live row, but destruction does not depend on
that event. Public IDL properties and column layout calculations are unchanged.

## Regression checks on the UI thread

In a Debug test host that imports the toolkit projections and includes the
DataTable implementation header, this checks registration without Unloaded:

```cpp
void CheckDataTableWeakRowRegistration()
{
    namespace labs = winrt::XamlToolkit::Labs::WinUI;
    labs::DataTable table;
    auto implementation = winrt::get_self<labs::implementation::DataTable>(table);
    winrt::weak_ref<labs::DataRow> weak;
    {
        labs::DataRow row;
        weak = row;
        implementation->RegisterRow(row);
        implementation->RegisterRow(row); // Re-registration must be idempotent.
        assert(implementation->Rows().size() == 1);
    }
    assert(!weak.get()); // The header must not keep the unrooted row alive.
    assert(implementation->Rows().empty()); // Also prune the expired weak entry.
}
```

Also measure a DataRow in an unrooted Grid, remove it before attaching the Grid
to XamlRoot, and verify that weak references to both objects expire after
dropping the external handles. This exercises the weak fallback parent-panel
cache. In a real list/tree host, repeatedly scroll, replace sorted rows, and
navigate away; live DataRow counts must settle. Reloading a table and resizing
its columns must still invalidate every live registered row.

These runtime checks require a Windows XAML UI thread; compilation alone does
not establish that the lifetime regression is resolved.
