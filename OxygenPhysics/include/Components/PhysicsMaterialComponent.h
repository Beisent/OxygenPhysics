#pragma once
#include <entt/entt.hpp>
namespace OxyPhysics
{
    struct PhysicsMaterialComponent
    {
        real restitution{0.2};
        real friction{0.5};
    };

}
