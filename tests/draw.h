#pragma once
#include "World.h"
#include "Graphics2D.h"
#include "Components/Components.h"
#include <entt/entt.hpp>
#include <cmath>
#include <random>

namespace OxyPhysics
{
    class PhysicsDebugDraw
    {
    private:
        float hashToFloat(uint32_t x)
        {
            x = (x ^ 61) ^ (x >> 16);
            x = x + (x << 3);
            x = x ^ (x >> 4);
            x = x * 0x27d4eb2d;
            x = x ^ (x >> 15);
            return (x & 0xFFFFFF) / float(0xFFFFFF);
        }

    public:
        PhysicsDebugDraw(PhysicsWorld &world, OxyRender::Graphics2D &graphics)
            : m_world(world), m_graphics(graphics) {}

        void Draw()
        {
            auto view = m_world.registry.view<TransformComponent, ShapeComponent>();

            view.each([this](auto entity, auto &tf, auto &shape)
                      {

        // 用 entity ID 通过hash生成固定颜色
        uint32_t id = static_cast<uint32_t>(entt::to_integral(entity));
        float r = hashToFloat(id);
        float g = hashToFloat(id * 7 + 3); 
        float b = hashToFloat(id * 13 + 5);

        OxyRender::OxyColor color{r, g, b, 1.0f};

        float cosTheta = std::cos(tf.rotation + shape.localRotation);
        float sinTheta = std::sin(tf.rotation + shape.localRotation);
        Vec2 pos = tf.position + Vec2{
            shape.localPosition.x * cosTheta - shape.localPosition.y * sinTheta,
            shape.localPosition.x * sinTheta + shape.localPosition.y * cosTheta
        };

        switch (shape.type)
        {
        case ShapeType::Circle:
            m_graphics.drawCircle(pos.x, pos.y, shape.circle.radius,color);
            break;

        case ShapeType::Box:
        {
            float halfSize = shape.box.size * 0.5f;
            std::vector<MathLite::Vec2> vertices = {
                {-halfSize, -halfSize}, { halfSize, -halfSize},
                { halfSize,  halfSize}, {-halfSize,  halfSize}
            };
            std::vector<MathLite::Vec2> worldVertices;
            for (auto &v : vertices)
            {
                float x = v.x * cosTheta - v.y * sinTheta + pos.x;
                float y = v.x * sinTheta + v.y * cosTheta + pos.y;
                worldVertices.push_back({x, y});
            }
            m_graphics.drawPolygon(worldVertices,color);
        }
            break;

        case ShapeType::Polygon:
        {
            std::vector<MathLite::Vec2> worldVertices;
            for (auto &v : shape.polygon.vertices)
            {
                float x = v.x * cosTheta - v.y * sinTheta + pos.x;
                float y = v.x * sinTheta + v.y * cosTheta + pos.y;
                worldVertices.push_back({x, y});
            }
            m_graphics.drawPolygon(worldVertices,color);
        }
            break;
        } });
        }

    private:
        PhysicsWorld &m_world;
        OxyRender::Graphics2D &m_graphics;
    };
}
