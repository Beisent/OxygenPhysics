#pragma once
#include <variant>
#include <vector>
#include "OxyMathLite.h"
using namespace OxygenMathLite;
namespace OxyPhysics
{
    struct Circle
    {
        real radius{10.0};
    };

    struct Box
    {
        real size{10.0};
    };

    struct Polygon
    {
        std::vector<Vec2> vertices;
        Polygon() { vertices.reserve(8); } // 默认最多8个顶点
    };

    enum class ShapeType : uint8_t
    {
        Circle,
        Box,
        Polygon
    };

    struct ShapeComponent
    {
        ShapeType type;
        Vec2 localPosition{0, 0};
        real localRotation{0.0};
        Circle circle;
        Box box;
        Polygon polygon;

        ShapeComponent() : type(ShapeType::Circle), circle{} {}
        ShapeComponent(const Circle &c) : type(ShapeType::Circle), circle(c) {}
        ShapeComponent(const Box &b) : type(ShapeType::Box), box(b) {}
        ShapeComponent(const Polygon &p) : type(ShapeType::Polygon), polygon(p) {}
    };
}