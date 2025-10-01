#include "World.h"
#include "Components/Components.h"
#include "Factories/Factories.h"
namespace OxyPhysics
{
    entt::entity PhysicsWorld::CreateCircleBody(const Vec2 &position, real radius, real mass, bool isStatic)
    {
        BodyDef def;
        def.position = position;
        def.mass = mass;
        def.isStatic = isStatic;

        auto shape = ShapeFactory::CreateCircle(radius);
        return BodyFactory::CreateBody(registry, def, shape);
    }
    entt::entity PhysicsWorld::CreateBoxBody(const Vec2 &position, real size, real mass, bool isStatic)
    {
        BodyDef def;
        def.position = position;
        def.mass = mass;
        def.isStatic = isStatic;

        auto shape = ShapeFactory::CreateBox(size);
        return BodyFactory::CreateBody(registry, def, shape);
    }
    entt::entity PhysicsWorld::CreatePolygonBody(const Vec2 &position, const std::vector<Vec2> &vertices, real mass, bool isStatic)
    {
        BodyDef def;
        def.position = position;
        def.mass = mass;
        def.isStatic = isStatic;

        auto shape = ShapeFactory::CreatePolygon(vertices);
        return BodyFactory::CreateBody(registry, def, shape);
    }

    void PhysicsWorld::DestroyBody(entt::entity e)
    {
        BodyFactory::DestroyBody(registry, e);
    }

    // 物理步进
    void PhysicsWorld::Step(real dt)
    {
        auto view = registry.view<TransformComponent, VelocityComponent, MassComponent>();

        view.each([dt](auto entity, auto &transform, auto &velocity, auto &mass)
                  {
                if (mass.isStatic)
                    return;

                transform.position += velocity.linearVelocity * dt;
                transform.rotation += velocity.angularVelocity * dt; });
    }

}