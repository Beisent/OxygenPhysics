#pragma once
namespace OxyPhysics
{
    struct MassComponent
    {
        real mass{1.0};
        real invMass{1.0};
        real inertia{1.0};
        real invInertia{1.0};
        bool isStatic{false};
        MassComponent(){}
    };
}
