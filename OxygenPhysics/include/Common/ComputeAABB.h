#pragma once
#include "Common/OxygenMathLite.h"
#include "Components/TransformComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/ManifoldComponent.h"

namespace OxyPhysics
{
    AABBComponent computeAABB(const TransformComponent &tf, const ShapeComponent &shape);
}