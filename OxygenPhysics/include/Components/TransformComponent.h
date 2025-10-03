#pragma once

#include "Common/OxygenMathLite.h"
using namespace OxygenMathLite;
namespace OxyPhysics
{
    namespace Components
    {
    struct TransformComponent
    {
        Vec2 position{0.0, 0.0}; 
        real rotation{0.0};    
    };
    }
}
