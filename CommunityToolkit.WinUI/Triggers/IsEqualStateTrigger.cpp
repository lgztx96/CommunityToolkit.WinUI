#include "pch.h"
#include "IsEqualStateTrigger.h"
#if __has_include("IsEqualStateTrigger.g.cpp")
#include "IsEqualStateTrigger.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::implementation
{
    void IsEqualStateTrigger::UpdateTrigger() { SetActive(IsEqualStateTrigger::AreValuesEqual(Value(), To(), true)); }

    void IsEqualStateTrigger::OnValuePropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        winrt::get_self<IsEqualStateTrigger>(d.as<class_type>())->UpdateTrigger();
    }

    bool IsEqualStateTrigger::AreValuesEqual(IInspectable const& value1, IInspectable const& value2, bool convertType)
    {
        if (value1 == value2)
        {
            return true;
        }

        // If they are the same type but fail with Equals check, don't bother with conversion.
        if (value1 != nullptr && value2 != nullptr && convertType
            //&& value1.GetType() != value2.GetType()
            )
        {
            // Try the conversion in both ways:
            return ConvertTypeEquals(value1, value2) || ConvertTypeEquals(value2, value1);
        }

        return false;
    }

    bool IsEqualStateTrigger::ConvertTypeEquals(IInspectable const& value1, IInspectable const& value2)
    {
        auto type1 = winrt::get_class_name(value1);
        auto type2 = winrt::get_class_name(value2);

        if (type1 != type2) {
            return false;
        }

        auto v1 = value1.try_as<Windows::Foundation::IPropertyValue>();
        auto v2 = value1.try_as<Windows::Foundation::IPropertyValue>();

        if (v1 && v2 && v1.IsNumericScalar())
        {
            switch (v1.Type())
            {
            case PropertyType::Int16:
            case PropertyType::UInt16:
                return v1.GetUInt16() == v2.GetUInt16();
            case PropertyType::Int32:
            case PropertyType::UInt32:
                return v1.GetUInt32() == v2.GetUInt32();
            case PropertyType::Int64:
            case PropertyType::UInt64:
                return v1.GetUInt64() == v2.GetUInt64();
            }
        }

        return false;
    }
}
