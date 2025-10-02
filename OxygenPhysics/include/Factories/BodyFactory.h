#pragma once
#include <entt/entt.hpp>
#include "Components/Components.h"
#include "Factories/ShapeFactory.h"
#include "Common/InertiaCalculate.h"
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
    class RigidFactory
    {
    public:
        static entt::entity CreateRigid(entt::registry &registry, const BodyDef &def, const ShapeComponent &shape)
        {
            auto e = registry.create();

            registry.emplace<TransformComponent>(e, def.position, def.angle);
            registry.emplace<VelocityComponent>(e, def.velocity);
            
            MassComponent massComp;
            massComp.mass = def.mass;
            massComp.invMass = 1.0f / massComp.mass;
            massComp.inertia = computeInertia(def.mass, shape);
            massComp.invInertia = 1.0f / massComp.inertia;
            massComp.isStatic = def.isStatic;
            registry.emplace<MassComponent>(e, massComp);

            registry.emplace<ShapeComponent>(e, shape);

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
        static real computeInertia(real mass, const ShapeComponent &shape)
        {
            switch (shape.type)
            {
            case ShapeType::Circle:
                return InertiaCalculate::calculateCircle(mass, shape.circle.radius);
            case ShapeType::Box:
                return InertiaCalculate::calculateBox(mass, shape.box.size);
            case ShapeType::Polygon:
            {
                return InertiaCalculate::calculatePolygon(mass, shape.polygon.vertices);
            }
            }
            return 0.0f;
        }
    };
}