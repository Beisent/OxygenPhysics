#pragma once
#include "Common/OxygenMathLite.h"
using namespace OxygenMathLite;
namespace OxyPhysics
{
    namespace Components
    {
        struct MaterialComponent
        {
            real restitution{0.2};
            real friction{0.5};
        };
    }

}
