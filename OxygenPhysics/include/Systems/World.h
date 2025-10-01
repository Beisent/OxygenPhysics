#pragma once
#include <entt/entt.hpp>
#include "OxyMathLite.h"
using namespace OxygenMathLite;
namespace OxyPhysics
{
    class PhysicsWorld
    {
    public:
        entt::registry registry;

        PhysicsWorld() = default;

        // 创建圆形刚体
        entt::entity CreateCircleBody(const Vec2 &position, real radius, real mass = 1.0f, bool isStatic = false);

        // 创建方形刚体
        entt::entity CreateBoxBody(const Vec2 &position, real size, real mass = 1.0f, bool isStatic = false);

        // 创建多边形刚体
        entt::entity CreatePolygonBody(const Vec2 &position, const std::vector<Vec2> &vertices, real mass = 1.0f, bool isStatic = false);

        // 删除刚体
        void DestroyBody(entt::entity e);

        // 物理步进
        void Step(real dt);
    };
}