#pragma once
#include <entt/entt.hpp>
#include "Components/Components.h"
#include "Factories/ShapeFactory.h"
namespace OxyPhysics
{
    struct BodyDef
    {
        Vec2 position{0, 0};
        real angle{0};
        Vec2 velocity{0, 0};
        real mass{1.0};
        bool isStatic{false};
    };
    class BodyFactory
    {
    public:
        template <typename Shape>
        static entt::entity CreateBody(entt::registry &registry, const BodyDef &def, const Shape &shape)
        {
            auto e = registry.create();

            registry.emplace<TransformComponent>(e, def.position, def.angle);
            registry.emplace<VelocityComponent>(e, def.velocity);
            registry.emplace<MassComponent>(e, def.mass, def.isStatic);

            registry.emplace<ShapeComponent>(e, shape);

            return e;
        }

        static void DestroyBody(entt::registry &registry, entt::entity e)
        {
            if (registry.valid(e))
            {
                registry.destroy(e);
            }
        }
    };
}