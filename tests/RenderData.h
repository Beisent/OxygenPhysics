#pragma once
#include "Common/OxygenMathLite.h"
#include "Components/ShapeComponent.h"

namespace OxyPhysics
{

    struct RenderData
    {
        Vec2 position;
        real rotation;
        ShapeType shapeType;
        AABBComponent aabb;

        // 形状相关数据
        real circleRadius = 0.0f;
        real boxSize = 0.0f;
        std::vector<Vec2> polygonVertices;

        // 形状局部变换
        Vec2 localPosition;
        real localRotation;

        // 颜色（可由外部决定如何生成）
        uint32_t entityId;
    };

} // namespace OxyPhysics