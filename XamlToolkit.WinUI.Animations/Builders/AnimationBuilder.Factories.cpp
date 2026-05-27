// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <memory>
#endif
#include "AnimationBuilder.h"

namespace winrt::XamlToolkit::WinUI::Animations
{
    namespace
    {
        class ExternalCompositionAnimation final : public ICompositionAnimationFactory
        {
        public:
            ExternalCompositionAnimation(CompositionObject const& target, CompositionAnimation const& animation)
                : target(target), animation(animation)
            {
            }

            CompositionAnimation GetAnimation(CompositionObject const&, CompositionObject& actualTarget) override
            {
                actualTarget = target;
                return animation;
            }

        private:
            CompositionObject target{ nullptr };
            CompositionAnimation animation{ nullptr };
        };

        class ExternalXamlAnimation final : public IXamlAnimationFactory
        {
        public:
            explicit ExternalXamlAnimation(Timeline const& animation)
                : animation(animation)
            {
            }

            Timeline GetAnimation(DependencyObject const&) override
            {
                return animation;
            }

        private:
            Timeline animation{ nullptr };
        };
    }

    AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory(CompositionAnimation const& animation, CompositionObject const& target)
    {
        return AddCompositionAnimationFactory(std::make_unique<ExternalCompositionAnimation>(target, animation));
    }

    AnimationBuilder& AnimationBuilder::ExternalAnimation(CompositionAnimation const& animation)
    {
        return AddCompositionAnimationFactory(animation, nullptr);
    }

    AnimationBuilder& AnimationBuilder::ExternalAnimation(CompositionObject const& target, CompositionAnimation const& animation)
    {
        return AddCompositionAnimationFactory(animation, target);
    }

    AnimationBuilder& AnimationBuilder::AddXamlAnimationFactory(Timeline const& animation)
    {
        return AddXamlAnimationFactory(std::make_unique<ExternalXamlAnimation>(animation));
    }

    AnimationBuilder& AnimationBuilder::ExternalAnimation(Timeline const& animation)
    {
        return AddXamlAnimationFactory(animation);
    }
}
