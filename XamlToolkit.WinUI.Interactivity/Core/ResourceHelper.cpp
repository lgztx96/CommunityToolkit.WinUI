#include "pch.h"
#include "winrt_module_imports.h"
#include "ResourceHelper.h"

namespace
{
    using namespace winrt::Microsoft::Windows::ApplicationModel::Resources;

    winrt::hstring LookupString(std::wstring_view resourceName)
    {
        static const auto strings = ResourceLoader(ResourceLoader::GetDefaultResourceFilePath(), L"XamlToolkit.WinUI.Interactivity/Strings");
        return strings.GetString(resourceName);
    }
}

namespace winrt::XamlToolkit::WinUI::Interactivity
{
    winrt::hstring ResourceHelper::GetString(std::wstring_view resourceName)
    {
        return LookupString(resourceName);
    }

    winrt::hstring ResourceHelper::CallMethodActionValidMethodNotFoundExceptionMessage()
    {
        return GetString(L"CallMethodActionValidMethodNotFoundExceptionMessage");
    }

    winrt::hstring ResourceHelper::ChangePropertyActionCannotFindPropertyNameExceptionMessage()
    {
        return GetString(L"ChangePropertyActionCannotFindPropertyNameExceptionMessage");
    }

    winrt::hstring ResourceHelper::ChangePropertyActionCannotSetValueExceptionMessage()
    {
        return GetString(L"ChangePropertyActionCannotSetValueExceptionMessage");
    }

    winrt::hstring ResourceHelper::ChangePropertyActionPropertyIsReadOnlyExceptionMessage()
    {
        return GetString(L"ChangePropertyActionPropertyIsReadOnlyExceptionMessage");
    }

    winrt::hstring ResourceHelper::GoToStateActionTargetHasNoStateGroups()
    {
        return GetString(L"GoToStateActionTargetHasNoStateGroups");
    }

    winrt::hstring ResourceHelper::CannotAttachBehaviorMultipleTimesExceptionMessage()
    {
        return GetString(L"CannotAttachBehaviorMultipleTimesExceptionMessage");
    }

    winrt::hstring ResourceHelper::CannotFindEventNameExceptionMessage()
    {
        return GetString(L"CannotFindEventNameExceptionMessage");
    }

    winrt::hstring ResourceHelper::InvalidAssociatedObjectExceptionMessage()
    {
        return GetString(L"InvalidAssociatedObjectExceptionMessage");
    }

    winrt::hstring ResourceHelper::NonActionAddedToActionCollectionExceptionMessage()
    {
        return GetString(L"NonActionAddedToActionCollectionExceptionMessage");
    }

    winrt::hstring ResourceHelper::NonBehaviorAddedToBehaviorCollectionExceptionMessage()
    {
        return GetString(L"NonBehaviorAddedToBehaviorCollectionExceptionMessage");
    }

    winrt::hstring ResourceHelper::DuplicateBehaviorInCollectionExceptionMessage()
    {
        return GetString(L"DuplicateBehaviorInCollectionExceptionMessage");
    }

    winrt::hstring ResourceHelper::InvalidLeftOperand()
    {
        return GetString(L"InvalidLeftOperand");
    }

    winrt::hstring ResourceHelper::InvalidRightOperand()
    {
        return GetString(L"InvalidRightOperand");
    }

    winrt::hstring ResourceHelper::InvalidOperands()
    {
        return GetString(L"InvalidOperands");
    }
}
