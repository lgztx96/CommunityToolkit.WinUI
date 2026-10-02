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
        if (_headerVisual && _scrollViewer.get() && _animationProperties)
        {
            _animationProperties.InsertScalar(L"OffsetY", 0.0f);
        }
    }

    bool QuickReturnHeaderBehavior::AssignAnimation()
    {
        if (HeaderBehaviorBase::AssignAnimation())
        {
            _animationProperties.InsertScalar(L"OffsetY", 0.0f);

            auto scrollViewer = _scrollViewer.get();
            _viewChangedRevoker = scrollViewer.ViewChanged(winrt::auto_revoke, { this, &QuickReturnHeaderBehavior::OnViewChanged });

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
        auto scrollViewer = _scrollViewer.get();
        auto header = AssociatedObject();
        if (!_animationProperties || !scrollViewer || !header) return;

        double headerHeight = header.ActualHeight();
        if (_headerPosition + headerHeight < scrollViewer.VerticalOffset())
        {
            // scrolling down: move header down, so it is just above screen
            _headerPosition = scrollViewer.VerticalOffset() - headerHeight;
            _animationProperties.InsertScalar(L"OffsetY", static_cast<float>(_headerPosition));
        }
        else if (_headerPosition > scrollViewer.VerticalOffset())
        {
            // scrolling up: move header up, align with top border.
            // the expression animation makes sure it never really is shown below border, so no lag effect!
            _headerPosition = scrollViewer.VerticalOffset();
            _animationProperties.InsertScalar(L"OffsetY", static_cast<float>(_headerPosition));
        }
    }
}
