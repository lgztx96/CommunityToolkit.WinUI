#include "pch.h"
#include "winrt_module_imports.h"
#include "ResizeThumb.h"

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    ResizeDirection ResizeThumb::Direction() const
    {
        return winrt::unbox_value<ResizeDirection>(GetValue(DirectionProperty()));
    }

    void ResizeThumb::Direction(ResizeDirection value)
    {
        SetValue(DirectionProperty(), winrt::box_value(value));
    }

    void ResizeThumb::OnDirectionPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& args)
    {
        if (auto resizeThumb = d.try_as<class_type>())
        {
            ManipulationModes mode = ManipulationModes::None;

            switch (resizeThumb.Direction())
            {
            case ResizeDirection::Top:
            case ResizeDirection::Bottom:
                mode = ManipulationModes::TranslateY;
                break;

            case ResizeDirection::Left:
            case ResizeDirection::Right:
                mode = ManipulationModes::TranslateX;
                break;

            case ResizeDirection::TopLeft:
            case ResizeDirection::TopRight:
            case ResizeDirection::BottomLeft:
            case ResizeDirection::BottomRight:
                mode = std::bit_cast<ManipulationModes>((static_cast<uint32_t>(ManipulationModes::TranslateX) | static_cast<uint32_t>(ManipulationModes::TranslateY)));
                break;

            default:
                mode = ManipulationModes::None;
                break;
            }

            resizeThumb.ManipulationMode(mode);
        }
    }

    const wil::single_threaded_property<DependencyProperty>
        ResizeThumb::DirectionProperty =
        DependencyProperty::Register(
            L"Direction",
            winrt::xaml_typename<ResizeDirection>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(ResizeDirection::None), &ResizeThumb::OnDirectionPropertyChanged));

    ResizePositionMode ResizeThumb::PositionMode() const
    {
        return winrt::unbox_value<ResizePositionMode>(GetValue(PositionModeProperty()));
    }

    void ResizeThumb::PositionMode(ResizePositionMode value)
    {
        SetValue(PositionModeProperty(), winrt::box_value(value));
    }

    const wil::single_threaded_property<DependencyProperty>
        ResizeThumb::PositionModeProperty =
        DependencyProperty::Register(
            L"PositionMode",
            winrt::xaml_typename<ResizePositionMode>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(ResizePositionMode::Canvas)));

    CursorEnum ResizeThumb::Cursor() const
    {
        return winrt::unbox_value<CursorEnum>(GetValue(CursorProperty()));
    }

    void ResizeThumb::Cursor(CursorEnum value)
    {
        SetValue(CursorProperty(), winrt::box_value(value));
    }

    void ResizeThumb::OnCursorPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& args)
    {
        if (auto resizeThumb = d.try_as<class_type>())
        {
            auto cursorValue = resizeThumb.ReadLocalValue(CursorProperty());

            if (cursorValue == DependencyProperty::UnsetValue() || cursorValue == nullptr)
            {
                CursorEnum cursor = CursorEnum::UniversalNo;

                switch (resizeThumb.Direction())
                {
                case ResizeDirection::Top:
                case ResizeDirection::Bottom:
                    cursor = CursorEnum::SizeNorthSouth;
                    break;

                case ResizeDirection::Left:
                case ResizeDirection::Right:
                    cursor = CursorEnum::SizeWestEast;
                    break;

                case ResizeDirection::TopLeft:
                case ResizeDirection::BottomRight:
                    cursor = CursorEnum::SizeNorthwestSoutheast;
                    break;

                case ResizeDirection::TopRight:
                case ResizeDirection::BottomLeft:
                    cursor = CursorEnum::SizeNortheastSouthwest;
                    break;

                default:
                    cursor = CursorEnum::UniversalNo;
                    break;
                }

                cursorValue = winrt::box_value(cursor);
            }

            if (cursorValue)
            {
                auto desired = winrt::unbox_value<CursorEnum>(cursorValue);

                auto resizeThumbImpl = winrt::get_self<ResizeThumb>(resizeThumb);
                auto cursor = resizeThumbImpl->ProtectedCursor();
                if (!cursor ||
                    (cursor.try_as<winrt::Microsoft::UI::Input::InputSystemCursor>() &&
                        cursor.try_as<winrt::Microsoft::UI::Input::InputSystemCursor>().CursorShape() != desired))
                {
                    resizeThumbImpl->ProtectedCursor(winrt::Microsoft::UI::Input::InputSystemCursor::Create(desired));
                }
            }
        }
    }

    const wil::single_threaded_property<DependencyProperty>
        ResizeThumb::CursorProperty =
        DependencyProperty::Register(
            L"Cursor",
            winrt::xaml_typename<CursorEnum>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr, &ResizeThumb::OnCursorPropertyChanged));

    FrameworkElement ResizeThumb::TargetControl() const
    {
        return GetValue(TargetControlProperty()).try_as<FrameworkElement>();
    }

    void ResizeThumb::TargetControl(FrameworkElement const& value)
    {
        SetValue(TargetControlProperty(), value);
    }

    const wil::single_threaded_property<DependencyProperty>
        ResizeThumb::TargetControlProperty =
        DependencyProperty::Register(
            L"TargetControl",
            winrt::xaml_typename<FrameworkElement>(),
            winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

    double ResizeThumb::DragIncrement() const
    {
        return winrt::unbox_value<double>(GetValue(DragIncrementProperty()));
    }

    void ResizeThumb::DragIncrement(double value)
    {
        SetValue(DragIncrementProperty(), winrt::box_value(value));
    }

    const wil::single_threaded_property<DependencyProperty>
        ResizeThumb::DragIncrementProperty =
        DependencyProperty::Register(
            L"DragIncrement",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(1.0)));
}
