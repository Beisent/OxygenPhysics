#pragma once
#include "Common/OxygenMathLite.h"
#include "Components/Components.h"
namespace OxyPhysics
{
    AABBComponent ComputeAABB(const TransformComponent &tf, const ShapeComponent &shape)
    {
        AABBComponent aabb;
        switch (shape.type)
        {
        case ShapeType::Circle:
            aabb.min = tf.position - Vec2(shape.circle.radius, shape.circle.radius);
            aabb.max = tf.position + Vec2(shape.circle.radius, shape.circle.radius);
            break;
        case ShapeType::Box:
        {
            real halfSize = shape.box.size * 0.5f;
            Vec2 corners[4] = {
                {-halfSize, -halfSize}, {halfSize, -halfSize}, {halfSize, halfSize}, {-halfSize, halfSize}};
            Vec2 min = Vec2{Constants::Infinity, Constants::Infinity};
            Vec2 max = Vec2{-Constants::Infinity, -Constants::Infinity};
            for (auto &c : corners)
            {
                Vec2 rotated = c.rotate(tf.rotation) + tf.position;
                min.x = std::min(min.x, rotated.x);
                min.y = std::min(min.y, rotated.y);
                max.x = std::max(max.x, rotated.x);
                max.y = std::max(max.y, rotated.y);
            }
            aabb.min = min;
            aabb.max = max;
            break;
        }
        case ShapeType::Polygon:
        {
            Vec2 min = Vec2{Constants::Infinity, Constants::Infinity};
            Vec2 max = Vec2{-Constants::Infinity, -Constants::Infinity};
            for (auto &v : shape.polygon.vertices)
            {
                Vec2 rotated = v.rotate(tf.rotation) + tf.position;
                min.x = std::min(min.x, rotated.x);
                min.y = std::min(min.y, rotated.y);
                max.x = std::max(max.x, rotated.x);
                max.y = std::max(max.y, rotated.y);
            }
            aabb.min = min;
            aabb.max = max;
            break;
        }
        }
        return aabb;
    }
}