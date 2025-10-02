#pragma once
#include <entt/entt.hpp>
#include "Common/OxygenMathLite.h"
#include <vector>
using namespace OxygenMathLite;
namespace OxyPhysics
{
    struct AABBComponent
    {
        Vec2 min;
        Vec2 max;
    };

    struct CollisionPair
    {
        entt::entity a;
        entt::entity b;
    };

    struct ContactPoint
    {
        Vec2 position;    // 接触点
        Vec2 normal;      // 法线（指向B）
        real penetration; // 渗透深度
    };

    struct ContactManifold
    {
        entt::entity a, b;
        std::vector<ContactPoint> points;
    };
}