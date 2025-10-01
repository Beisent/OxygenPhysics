#pragma once
#include "World.h"
#include "Graphics2D.h"
#include "Components/Components.h"
#include <entt/entt.hpp>
#include <cmath>

namespace OxyPhysics
{
    class PhysicsDebugDraw
    {
    public:
        PhysicsDebugDraw(PhysicsWorld &world, OxyRender::Graphics2D &graphics)
            : m_world(world), m_graphics(graphics) {}

        void Draw()
        {
            auto view = m_world.registry.view<TransformComponent, ShapeComponent>();

            // 使用 each 遍历
            view.each([this](auto entity, auto &tf, auto &shape)
            {
                // 将局部位置和旋转应用到全局
                float cosTheta = std::cos(tf.rotation + shape.localRotation);
                float sinTheta = std::sin(tf.rotation + shape.localRotation);
                Vec2 pos = tf.position + Vec2{
                    shape.localPosition.x * cosTheta - shape.localPosition.y * sinTheta,
                    shape.localPosition.x * sinTheta + shape.localPosition.y * cosTheta
                };

                switch (shape.type)
                {
                case ShapeType::Circle:
                    m_graphics.drawCircle(pos.x, pos.y, shape.circle.radius);
                    break;

                case ShapeType::Box:
                    {
                        float halfSize = shape.box.size * 0.5f;
                        std::vector<MathLite::Vec2> vertices = {
                            {-halfSize, -halfSize},
                            { halfSize, -halfSize},
                            { halfSize,  halfSize},
                            {-halfSize,  halfSize}
                        };
                        std::vector<MathLite::Vec2> worldVertices;
                        for (auto &v : vertices)
                        {
                            float x = v.x * cosTheta - v.y * sinTheta + pos.x;
                            float y = v.x * sinTheta + v.y * cosTheta + pos.y;
                            worldVertices.push_back({x, y});
                        }
                        m_graphics.drawPolygon(worldVertices);
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
                        m_graphics.drawPolygon(worldVertices);
                    }
                    break;
                }
            });
        }

    private:
        PhysicsWorld &m_world;
        OxyRender::Graphics2D &m_graphics;
    };
}
