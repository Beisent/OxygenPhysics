#pragma once
#include "ShapeComponent.h"

namespace OxyPhysics
{
    class ShapeFactory
    {
    public:
        // 创建圆形
        static Components::ShapeComponent CreateCircle(real radius, const OxygenMathLite::Vec2& localPos = {0,0})
        {
            Circle c{radius};
            Components::ShapeComponent shape(c);
            shape.localPosition = localPos;
            return shape;
        }

        // 创建方形/Box
        static Components::ShapeComponent CreateBox(real size, const OxygenMathLite::Vec2& localPos = {0,0})
        {
            Box b{size};
            Components::ShapeComponent shape(b);
            shape.localPosition = localPos;
            return shape;
        }

        // 创建多边形
        static Components::ShapeComponent CreatePolygon(const std::vector<OxygenMathLite::Vec2>& vertices, const OxygenMathLite::Vec2& localPos = {0,0})
        {
            Polygon p;
            p.vertices = vertices;
            Components::ShapeComponent shape(p);
            shape.localPosition = localPos;
            return shape;
        }
    };
}