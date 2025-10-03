#include "World.h"
#include "Components/Components.h"
#include <iostream>

namespace OxyPhysics
{

    PhysicsWorld::PhysicsWorld() : m_broadPhase(std::make_unique<BroadPhaseNaive>())
    {
    }
    entt::entity PhysicsWorld::CreateCircleRigid(const Vec2 &position, real radius, real mass, bool isStatic)
    {
        BodyDef def;
        def.position = position;
        def.mass = mass;
        def.isStatic = isStatic;

        auto shape = ShapeFactory::CreateCircle(radius);
        return RigidFactory::CreateRigid(registry, def, shape);
    }
    entt::entity PhysicsWorld::CreateBoxRigid(const Vec2 &position, real size, real mass, bool isStatic)
    {
        BodyDef def;
        def.position = position;
        def.mass = mass;
        def.isStatic = isStatic;

        auto shape = ShapeFactory::CreateBox(size);
        return RigidFactory::CreateRigid(registry, def, shape);
    }
    entt::entity PhysicsWorld::CreatePolygonRigid(const Vec2 &position, const std::vector<Vec2> &vertices, real mass, bool isStatic)
    {
        BodyDef def;
        def.position = position;
        def.mass = mass;
        def.isStatic = isStatic;

        auto shape = ShapeFactory::CreatePolygon(vertices);
        return RigidFactory::CreateRigid(registry, def, shape);
    }

    entt::entity PhysicsWorld::CreateRigid(const BodyDef &def, const Components::ShapeComponent &shape)
    {
        return RigidFactory::CreateRigid(registry, def, shape);
    }
    void PhysicsWorld::DestroyRigid(entt::entity e)
    {
        RigidFactory::DestroyRigid(registry, e);
    }

    void PhysicsWorld::SetBroadPhase(std::unique_ptr<BroadPhase> bp)
    {
        m_broadPhase = std::move(bp);
    }
    // 物理步进
    void PhysicsWorld::Step(real dt)
    {
        auto view = registry.view<Components::TransformComponent, Components::VelocityComponent,
                                  Components::MassComponent, Components::ShapeComponent, Components::AABBComponent>();

        view.each([this, dt](auto entity, auto &tf, auto &vel, auto &mass, auto &shape, auto &aabb)
                  {
                      if (mass.isStatic)
                          return;

                      // 移动
                      tf.position += vel.linearVelocity * dt;
                      tf.rotation += vel.angularVelocity * dt;

                      // 更新 AABB
                      aabb = computeAABB(tf, shape); });
        m_broadPhase->findPairs(registry, m_collisionPairs);
    }
}