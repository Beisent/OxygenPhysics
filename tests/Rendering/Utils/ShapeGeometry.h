#pragma once
#include <vector>
#include "Common/OxygenMathLite.h"
#include "Components/ShapeComponent.h"

namespace OxyPhysics::Utils
{
    using namespace OxygenMathLite;
    using namespace OxyPhysics::Components;

    class ShapeGeometry
    {
    public:
        // 根据形状类型生成顶点数据（局部空间）
        static std::vector<Vec2> GetVertices(const ShapeComponent &shape)
        {
            std::vector<Vec2> vertices;
            switch (shape.type)
            {
            case ShapeType::Circle:
                vertices = GenerateCircleVertices(shape.circle.radius);
                break;
            case ShapeType::Box:
                vertices = GenerateBoxVertices(shape.box.size);
                break;
            case ShapeType::Polygon:
                vertices = shape.polygon.vertices;
                break;
            }
            return vertices;
        }

        static std::vector<Vec2> GenerateCircleVertices(real radius, int segments = 24)
        {
            std::vector<Vec2> points;
            points.reserve(segments);
            const real step = 2.0f * M_PI / segments;
            for (int i = 0; i < segments; ++i)
            {
                real angle = i * step;
                points.emplace_back(Vec2(cos(angle) * radius, sin(angle) * radius));
            }
            return points;
        }

        static std::vector<Vec2> GenerateBoxVertices(const real &size)
        {
            return {
                Vec2(-size, -size),
                Vec2(size, -size),
                Vec2(size, size),
                Vec2(-size, size)
            };
        }
    };
}
