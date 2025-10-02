#pragma once
#include "Common/OxygenMathLite.h"
#include <vector>


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
        std::vector<OxygenMathLite::Vec2> vertices = {{0.0f,5.0f},{-5.0f,0.0f},{5.0f,0.0f}};
        Polygon() {}
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
        OxygenMathLite::Vec2 localPosition{0, 0};
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