#pragma once
#include <entt/entt.hpp>
namespace OxyPhysics
{
    struct MassComponent
    {
        double mass{1.0};
        double invMass{1.0};
        double inertia{1.0};
        double invInertia{1.0};
        bool isStatic{false};
    };
}
