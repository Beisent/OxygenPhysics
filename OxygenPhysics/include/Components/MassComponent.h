#pragma once
#include <entt/entt.hpp>
namespace OxyPhysics
{
    struct MassComponent
    {
        real mass{1.0};
        real invMass{1.0};
        real inertia{1.0};
        real invInertia{1.0};
        bool isStatic{false};
        MassComponent(real m, bool stat)
            : mass(m), isStatic(stat)
        {
            invMass = isStatic || mass == 0.0f ? 0.0f : 1.0f / mass;
            inertia = mass; 
            invInertia = isStatic || inertia == 0.0f ? 0.0f : 1.0f / inertia;
        }
    };
}
