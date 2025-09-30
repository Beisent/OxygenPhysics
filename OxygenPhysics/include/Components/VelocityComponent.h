#pragma once
#include "OxyMathLite.h"
using namespace OxygenMathLite;
namespace OxyPhysics
{
    struct VelocityComponent
    {
        Vec2 linearVelocity{0.0, 0.0};
        real angularVelocity{0.0};

        Vec2 linearDamping{0.0, 0.0};
        real angularDamping{0.0};

        Vec2 accumulatedForce{0.0, 0.0};
        real accumulatedTorque{0.0};
    };

}
