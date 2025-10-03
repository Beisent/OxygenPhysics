#pragma once
#include "Components.h"
#include <entt/entt.hpp>
#include <vector>

namespace OxyPhysics
{
    inline bool overlapAABB(const AABBComponent &a, const AABBComponent &b)
    {
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
               (a.min.y <= b.max.y && a.max.y >= b.min.y);
    }

       // 粗检测，用于快速排除无用的检测
    class BroadPhase
    {
    public:
        virtual ~BroadPhase() = default;

        // 可选：更新代理体数据（例如动态树、网格需要）
        virtual void compute(entt::registry &reg) {}

        // 必选：寻找潜在碰撞对
        virtual void findPairs(entt::registry &reg, std::vector<CollisionPair> &pairs) = 0;
    };

    class BroadPhaseNaive : public BroadPhase
    {
    public:
        void findPairs(entt::registry &reg, std::vector<CollisionPair> &pairs) override;
    };
}
