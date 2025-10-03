#include "Systems/Collision.h"

namespace OxyPhysics
{
    void BroadPhaseNaive::findPairs(entt::registry &reg, std::vector<Components::CollisionPair> &pairs)
    {
        pairs.clear();
        auto view = reg.view<Components::AABBComponent>();
        std::vector<entt::entity> entities(view.begin(), view.end());

        for (size_t i = 0; i < entities.size(); ++i)
        {
            auto e1 = entities[i];
            const auto &a1 = view.get<Components::AABBComponent>(e1);

            for (size_t j = i + 1; j < entities.size(); ++j)
            {
                auto e2 = entities[j];
                const auto &a2 = view.get<Components::AABBComponent>(e2);

                if (overlapAABB(a1, a2))
                    pairs.push_back({e1, e2});
            }
        }
    }
}