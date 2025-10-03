#pragma once
#include "PhysicsThread.h"
#include "Graphics2D.h"
#include "RenderData.h"
#include "Common/OxygenMathLite.h"
#include <cmath>
#include <random>

using namespace OxyPhysics;
using namespace OxyRender;

namespace OxyPhysics
{
    // 简单的物理调试渲染器
    class PhysicsDebugDraw
    {
    public:
        PhysicsDebugDraw(PhysicsThread
                             &simulation,
                         Graphics2D &graphics)
            : m_simulation(simulation), m_graphics(graphics), AABBDraw(true) {}

        void Draw()
        {
            // 首先检查物理模拟是否已准备好渲染数据
            if (!m_simulation.IsReadyForRender())
            {
                return; // 如果未准备好，就不进行渲染
            }

            std::vector<RenderData> renderData;
            m_simulation.GetRenderData(renderData);

            for (const auto &data : renderData)
            {
                // 用 entity ID 通过hash生成固定颜色
                uint32_t id = static_cast<uint32_t>(entt::to_integral(data.entityId));
                float r = hashToFloat(id);
                float g = hashToFloat(id * 7 + 3);
                float b = hashToFloat(id * 13 + 5);

                OxyRender::OxyColor color{r, g, b, 1.0f};

                float cosTheta = std::cos(data.rotation + data.localRotation);
                float sinTheta = std::sin(data.rotation + data.localRotation);
                OxygenMathLite::Vec2 pos = data.position + OxygenMathLite::Vec2{
                                                               data.localPosition.x * cosTheta - data.localPosition.y * sinTheta,
                                                               data.localPosition.x * sinTheta + data.localPosition.y * cosTheta};

                if (AABBDraw)
                {
                    OxygenMathLite::Vec2 min = data.aabb.min;
                    OxygenMathLite::Vec2 max = data.aabb.max;
                    std::vector<MathLite::Vec2> aabbVerts = {
                        {min.x, min.y},
                        {max.x, min.y},
                        {max.x, max.y},
                        {min.x, max.y}};

                    // 默认红色
                    OxyRender::OxyColor aabbColor{1.0f, 0.0f, 0.0f, 1.0f};

                    // 检查是否在碰撞对中改成蓝色
                    for (auto &pair : data.collisionPairs)
                    {
                        if (pair.a == data.entityId || pair.b == data.entityId)
                        {
                            aabbColor = {0.0f, 0.0f, 1.0f, 1.0f};
                            break;
                        }
                    }
                    m_graphics.drawPolygonOutline(aabbVerts, aabbColor);
                }
                switch (data.shapeType)
                {
                case ShapeType::Circle:
                    m_graphics.drawCircle(pos.x, pos.y, data.circleRadius, color);
                    break;

                case ShapeType::Box:
                {
                    float halfSize = data.boxSize * 0.5f;
                    std::vector<MathLite::Vec2> vertices = {
                        {-halfSize, -halfSize}, {halfSize, -halfSize}, {halfSize, halfSize}, {-halfSize, halfSize}};
                    std::vector<MathLite::Vec2> worldVertices;
                    for (auto &v : vertices)
                    {
                        float x = v.x * cosTheta - v.y * sinTheta + pos.x;
                        float y = v.x * sinTheta + v.y * cosTheta + pos.y;
                        worldVertices.push_back({x, y});
                    }
                    m_graphics.drawPolygon(worldVertices, color);
                }
                break;

                case ShapeType::Polygon:
                {
                    std::vector<MathLite::Vec2> worldVertices;
                    for (auto &v : data.polygonVertices)
                    {
                        float x = v.x * cosTheta - v.y * sinTheta + pos.x;
                        float y = v.x * sinTheta + v.y * cosTheta + pos.y;
                        worldVertices.push_back({x, y});
                    }
                    m_graphics.drawPolygon(worldVertices, color);
                }
                break;
                }
            }
        }

    private:
        PhysicsThread &m_simulation;
        bool AABBDraw;
        Graphics2D &m_graphics;

        float hashToFloat(uint32_t id)
        {
            // 简单的哈希函数，将整数映射到[0,1]范围
            id ^= id >> 16;
            id *= 0x85ebca6b;
            id ^= id >> 13;
            id *= 0xc2b2ae35;
            id ^= id >> 16;
            return static_cast<float>(id & 0x00FFFFFF) / 0x00FFFFFF;
        }
    };
}
