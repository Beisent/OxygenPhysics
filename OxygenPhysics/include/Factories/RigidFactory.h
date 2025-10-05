#pragma once
#include <entt/entt.hpp>
#include "Components/Components.h"
#include "Factories/ShapeFactory.h"
#include "Common/ComputeInertia.h"

namespace OxyPhysics
{
    struct BodyDef
    {
        Vec2 position{0, 0};
        real angle{0};
        Vec2 velocity{0, 0};
        real angularVelocity{0};
        real mass{1.0};
        bool isStatic{false};
    };
    class RigidFactory
    {
    public:
        static entt::entity CreateRigid(entt::registry &registry, const BodyDef &def, const Components::ShapeComponent &shape)
        {
            auto e = registry.create();

            registry.emplace<Components::TransformComponent>(e, def.position, def.angle);
            registry.emplace<Components::VelocityComponent>(e, def.velocity,def.angularVelocity);
            
            Components::MassComponent massComp;
            massComp.mass = def.mass;
            massComp.invMass = 1.0f / massComp.mass;
            massComp.inertia = computeInertia(def.mass, shape);
            massComp.invInertia = 1.0f / massComp.inertia;
            massComp.isStatic = def.isStatic;
            registry.emplace<Components::MassComponent>(e, massComp);

            registry.emplace<Components::ShapeComponent>(e, shape);
            registry.emplace<Components::AABBComponent>(e, computeAABB(registry.get<Components::TransformComponent>(e), shape));

            return e;
        }

        static void DestroyRigid(entt::registry &registry, entt::entity e)
        {
            if (registry.valid(e))
            {
                registry.destroy(e);
            }
        }

    private:
        static real computeInertia(real mass, const Components::ShapeComponent &shape)
        {
            switch (shape.type)
            {
            case ShapeType::Circle:
                return ComputeInertia::computeCircle(mass, shape.circle.radius);
            case ShapeType::Box:
                return ComputeInertia::computeBox(mass, shape.box.size);
            case ShapeType::Polygon:
            {
                return ComputeInertia::computePolygon(mass, shape.polygon.vertices);
            }
            }
            return 0.0f;
        }
    };
}