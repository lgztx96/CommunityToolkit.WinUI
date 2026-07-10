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

    void ResizeThumb::OnDirectionPropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto resizeThumb = d.try_as<class_type>())
        {
            winrt::ManipulationModes mode = winrt::ManipulationModes::None;

            switch (resizeThumb.Direction())
            {
            case ResizeDirection::Top:
            case ResizeDirection::Bottom:
                mode = winrt::ManipulationModes::TranslateY;
                break;

            case ResizeDirection::Left:
            case ResizeDirection::Right:
                mode = winrt::ManipulationModes::TranslateX;
                break;

            case ResizeDirection::TopLeft:
            case ResizeDirection::TopRight:
            case ResizeDirection::BottomLeft:
            case ResizeDirection::BottomRight:
                mode = static_cast<winrt::ManipulationModes>((std::to_underlying(winrt::ManipulationModes::TranslateX) | std::to_underlying(winrt::ManipulationModes::TranslateY)));
                break;

            default:
                mode = winrt::ManipulationModes::None;
                break;
            }

            resizeThumb.ManipulationMode(mode);
        }
    }

    const wil::single_threaded_property<winrt::DependencyProperty> ResizeThumb::DirectionProperty =
        winrt::DependencyProperty::Register(
            L"Direction",
            winrt::xaml_typename<ResizeDirection>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(ResizeDirection::None), &ResizeThumb::OnDirectionPropertyChanged));

    ResizePositionMode ResizeThumb::PositionMode() const
    {
        return winrt::unbox_value<ResizePositionMode>(GetValue(PositionModeProperty()));
    }

    void ResizeThumb::PositionMode(ResizePositionMode value)
    {
        SetValue(PositionModeProperty(), winrt::box_value(value));
    }

    const wil::single_threaded_property<DependencyProperty> ResizeThumb::PositionModeProperty =
        winrt::DependencyProperty::Register(
            L"PositionMode",
            winrt::xaml_typename<ResizePositionMode>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(ResizePositionMode::Canvas)));

    CursorEnum ResizeThumb::Cursor() const
    {
        return winrt::unbox_value<CursorEnum>(GetValue(CursorProperty()));
    }

    void ResizeThumb::Cursor(CursorEnum value)
    {
        SetValue(CursorProperty(), winrt::box_value(value));
    }

    void ResizeThumb::OnCursorPropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto resizeThumb = d.try_as<class_type>())
        {
            auto cursorValue = resizeThumb.ReadLocalValue(CursorProperty());

            if (cursorValue == winrt::DependencyProperty::UnsetValue() || cursorValue == nullptr)
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
                auto inputCursor = cursor.try_as<winrt::InputSystemCursor>();

                if (!inputCursor || inputCursor.CursorShape() != desired)
                {
                    resizeThumbImpl->ProtectedCursor(winrt::InputSystemCursor::Create(desired));
                }
            }
        }
    }

    const wil::single_threaded_property<winrt::DependencyProperty> ResizeThumb::CursorProperty =
        winrt::DependencyProperty::Register(
            L"Cursor",
            winrt::xaml_typename<CursorEnum>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &ResizeThumb::OnCursorPropertyChanged));

    winrt::FrameworkElement ResizeThumb::TargetControl() const
    {
        return GetValue(TargetControlProperty()).try_as<winrt::FrameworkElement>();
    }

    void ResizeThumb::TargetControl(winrt::FrameworkElement const& value)
    {
        SetValue(TargetControlProperty(), value);
    }

    const wil::single_threaded_property<winrt::DependencyProperty> ResizeThumb::TargetControlProperty =
        winrt::DependencyProperty::Register(
            L"TargetControl",
            winrt::xaml_typename<winrt::FrameworkElement>(),
            winrt::xaml_typename<class_type>(), 
            winrt::PropertyMetadata(nullptr));

    double ResizeThumb::DragIncrement() const
    {
        return winrt::unbox_value<double>(GetValue(DragIncrementProperty()));
    }

    void ResizeThumb::DragIncrement(double value)
    {
        SetValue(DragIncrementProperty(), winrt::box_value(value));
    }

    const wil::single_threaded_property<DependencyProperty> ResizeThumb::DragIncrementProperty =
        winrt::DependencyProperty::Register(
            L"DragIncrement",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(1.0)));
}
