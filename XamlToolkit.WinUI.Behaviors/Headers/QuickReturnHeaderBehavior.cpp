#include "pch.h"
#include "winrt_module_imports.h"
#include "QuickReturnHeaderBehavior.h"
#include "../Helper.h"
#if __has_include("QuickReturnHeaderBehavior.g.cpp")
#include "QuickReturnHeaderBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    void QuickReturnHeaderBehavior::Show()
    {
        if (_headerVisual && _scrollViewer && _animationProperties)
        {
            _animationProperties.InsertScalar(L"OffsetY", 0.0f);
        }
    }

    bool QuickReturnHeaderBehavior::AssignAnimation()
    {
        if (HeaderBehaviorBase::AssignAnimation())
        {
            _animationProperties.InsertScalar(L"OffsetY", 0.0f);

            _viewChangedRevoker = _scrollViewer.ViewChanged(winrt::auto_revoke, { this, &QuickReturnHeaderBehavior::OnViewChanged });

            auto compositor = _animationProperties.Compositor();
            auto expressionAnimation = compositor.CreateExpressionAnimation(L"max(min(animationProps.OffsetY, -scrollProps.Translation.Y), 0)");
            expressionAnimation.SetReferenceParameter(L"animationProps", _animationProperties);
            expressionAnimation.SetReferenceParameter(L"scrollProps", _scrollProperties);

            _headerVisual.StartAnimation(L"Offset.Y", expressionAnimation);

            return true;
        }

        return false;
    }

    void QuickReturnHeaderBehavior::StopAnimation()
    {
        if (_animationProperties)
        {
            _animationProperties.InsertScalar(L"OffsetY", 0.0f);
        }

        if (_headerVisual)
        {
            _headerVisual.StopAnimation(L"Offset.Y");

            auto offset = _headerVisual.Offset();
            offset.y = 0.0f;
            _headerVisual.Offset(offset);
        }
    }

    void QuickReturnHeaderBehavior::RemoveAnimation()
    {
        _viewChangedRevoker.revoke();

        HeaderBehaviorBase::RemoveAnimation();
    }

    void QuickReturnHeaderBehavior::OnViewChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ScrollViewerViewChangedEventArgs const& e)
    {
        if (!_animationProperties || !_scrollViewer) return;

        double headerHeight = AssociatedObject().ActualHeight();
        if (_headerPosition + headerHeight < _scrollViewer.VerticalOffset())
        {
            // scrolling down: move header down, so it is just above screen
            _headerPosition = _scrollViewer.VerticalOffset() - headerHeight;
            _animationProperties.InsertScalar(L"OffsetY", static_cast<float>(_headerPosition));
        }
        else if (_headerPosition > _scrollViewer.VerticalOffset())
        {
            // scrolling up: move header up, align with top border.
            // the expression animation makes sure it never really is shown below border, so no lag effect!
            _headerPosition = _scrollViewer.VerticalOffset();
            _animationProperties.InsertScalar(L"OffsetY", static_cast<float>(_headerPosition));
        }
    }
}
