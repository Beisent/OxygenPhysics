#pragma once
#include <entt/entt.hpp>
#include "OxyMathLite.h"
#include "Factories/Factories.h"
using namespace OxygenMathLite;
namespace OxyPhysics
{
    class PhysicsWorld
    {
    public:
        entt::registry registry;

        PhysicsWorld() = default;

        // 创建圆形刚体
        entt::entity CreateCircleRigid(const Vec2 &position, real radius, real mass = 1.0f, bool isStatic = false);

        // 创建方形刚体
        entt::entity CreateBoxRigid(const Vec2 &position, real size, real mass = 1.0f, bool isStatic = false);

        // 创建多边形刚体
        entt::entity CreatePolygonRigid(const Vec2 &position, const std::vector<Vec2> &vertices, real mass = 1.0f, bool isStatic = false);

        // 创建刚体
        entt::entity CreateRigid(const BodyDef &def, const ShapeComponent &shape);

        // 删除刚体
        void DestroyRigid(entt::entity e);

        // 物理步进
        void Step(real dt);
    };
}