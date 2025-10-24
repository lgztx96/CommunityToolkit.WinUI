#pragma once
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <wil/cppwinrt_helpers.h>

#include <SettingsControls/SettingsCard/SettingsCard.h>
#include <SettingsControls/Helpers/StyleExtensions.h>
#include <SettingsControls/Helpers/CornerRadiusConverter.h>
#include <SettingsControls/SettingsExpander/SettingsExpander.h>
#include <SettingsControls/SettingsExpander/SettingsExpanderItemStyleSelector.h>
#include <Segmented/Segmented.h>
#include <Segmented/SegmentedItem.h>
#include "Segmented/SegmentedMarginConverter.h"
#include "Segmented/EqualPanel.h"
#include "Sizers/SizerBase.h"
#include "Sizers/ContentSizer/ContentSizer.h"
#include "Sizers/PropertySizer/PropertySizer.h"
#include "Sizers/GridSplitter/GridSplitter.h"
#include "LayoutTransformControl\LayoutTransformControl.h"
#include "ImageCropper\ImageCropper.h"
#include "ImageCropper\ImageCropperThumb.h"
#include "StaggeredLayout\StaggeredLayout.h"
#include "ColorPicker\ColorPicker.h"
#include "ColorPicker\ColorPreviewer.h"
#include "ColorPicker\ColorPickerButton.h"
#include "ColorPicker\ColorPickerSlider.h"
#include "SwitchPresenter\Case.h"
#include "SwitchPresenter\SwitchPresenter.h"
#include "MetadataControl\MetadataControl.h"

