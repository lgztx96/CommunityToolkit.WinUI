#pragma once
#include "AnimationBuilder.h"
#include "AnimationBuilder.Factories.h"

namespace winrt::XamlToolkit::WinUI::Animations
{
    inline AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory(CompositionAnimation const& animation, CompositionObject const& target)
    {
        return AddCompositionAnimationFactory(std::make_unique<ExternalCompositionAnimation>(target, animation));
    }

    inline AnimationBuilder& AnimationBuilder::AddXamlAnimationFactory(Timeline const& animation)
    {
        return AddXamlAnimationFactory(std::make_unique<ExternalXamlAnimation>(animation));
    }

    inline AnimationBuilder& AnimationBuilder::ExternalAnimation(CompositionAnimation const& animation)
    {
        return AddCompositionAnimationFactory(animation, nullptr);
    }

    inline AnimationBuilder& AnimationBuilder::ExternalAnimation(CompositionObject const& target, CompositionAnimation const& animation)
    {
        return AddCompositionAnimationFactory(animation, target);
    }

    inline AnimationBuilder& AnimationBuilder::ExternalAnimation(Timeline const& animation)
    {
        return AddXamlAnimationFactory(animation);
    }
}