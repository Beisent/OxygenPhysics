#pragma once
#include <entt/entt.hpp>
namespace OxyPhysics
{
    struct PhysicsMaterialComponent
    {
        double restitution{0.2};
        double friction{0.5};
    };

}
