#pragma once
#include "PhysicsThread.h"
#include "RenderData.h"
#include "Common/OxygenMathLite.h"
#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>
#include <cstdint>

using namespace OxyPhysics;

namespace OxyPhysics
{
    // 简单的物理调试渲染器 - 使用SFML
    class PhysicsDebugDraw
    {
    public:
        PhysicsDebugDraw(PhysicsThread &simulation, sf::RenderWindow &window)
            : m_simulation(simulation), m_window(window), m_AABBDraw(true) {}

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

                sf::Color color(static_cast<uint8_t>(r * 255), 
                                static_cast<uint8_t>(g * 255), 
                                static_cast<uint8_t>(b * 255));

                float cosTheta = std::cos(data.rotation + data.localRotation);
                float sinTheta = std::sin(data.rotation + data.localRotation);
                OxygenMathLite::Vec2 pos = data.position + OxygenMathLite::Vec2{
                                                                data.localPosition.x * cosTheta - data.localPosition.y * sinTheta,
                                                                data.localPosition.x * sinTheta + data.localPosition.y * cosTheta};

                // 转换为SFML坐标系（SFML原点在左上角，我们的物理世界原点在中心）
                sf::Vector2f sfmlPos(m_window.getSize().x / 2 + pos.x, m_window.getSize().y / 2 - pos.y);

                if (m_AABBDraw)
                {
                    OxygenMathLite::Vec2 min = data.aabb.min;
                    OxygenMathLite::Vec2 max = data.aabb.max;
                    
                    // 转换AABB到SFML坐标系
                    sf::Vector2f sfmlMin(m_window.getSize().x / 2 + min.x, m_window.getSize().y / 2 - min.y);
                    sf::Vector2f sfmlMax(m_window.getSize().x / 2 + max.x, m_window.getSize().y / 2 - max.y);
                    
                    // 创建AABB矩形
                    sf::RectangleShape aabbRect(sf::Vector2f(sfmlMax.x - sfmlMin.x, sfmlMin.y - sfmlMax.y));
                    aabbRect.setPosition(sf::Vector2f(sfmlMin.x, sfmlMax.y));
                    aabbRect.setOutlineThickness(1);
                    aabbRect.setFillColor(sf::Color::Transparent);
                    
                    // 默认红色
                    sf::Color aabbColor(255, 0, 0);

                    // 检查是否在碰撞对中改成蓝色
                    for (auto &pair : data.collisionPairs)
                    {
                        if (pair.a == data.entityId || pair.b == data.entityId)
                        {
                            aabbColor = sf::Color(0, 0, 255);
                            break;
                        }
                    }
                    aabbRect.setOutlineColor(aabbColor);
                    m_window.draw(aabbRect);
                }
                
                switch (data.shapeType)
                {
                case ShapeType::Circle:
                {
                    sf::CircleShape circle(data.circleRadius);
                    circle.setPosition(sf::Vector2f(sfmlPos.x - data.circleRadius, sfmlPos.y - data.circleRadius));
                    circle.setFillColor(color);
                    m_window.draw(circle);
                }
                break;

                case ShapeType::Box:
                {
                    float halfSize = data.boxSize * 0.5f;
                    std::vector<OxygenMathLite::Vec2> vertices = {
                        {-halfSize, -halfSize}, {halfSize, -halfSize}, {halfSize, halfSize}, {-halfSize, halfSize}};
                    
                    sf::ConvexShape box;
                    box.setPointCount(4);
                    
                    for (size_t i = 0; i < vertices.size(); ++i)
                    {
                        // 应用旋转和位置变换
                        float x = vertices[i].x * cosTheta - vertices[i].y * sinTheta + pos.x;
                        float y = vertices[i].x * sinTheta + vertices[i].y * cosTheta + pos.y;
                        
                        // 转换到SFML坐标系
                        box.setPoint(i, sf::Vector2f(m_window.getSize().x / 2 + x, m_window.getSize().y / 2 - y));
                    }
                    box.setFillColor(color);
                    m_window.draw(box);
                }
                break;

                case ShapeType::Polygon:
                {
                    sf::ConvexShape polygon;
                    polygon.setPointCount(data.polygonVertices.size());
                    
                    for (size_t i = 0; i < data.polygonVertices.size(); ++i)
                    {
                        // 应用旋转和位置变换
                        float x = data.polygonVertices[i].x * cosTheta - data.polygonVertices[i].y * sinTheta + pos.x;
                        float y = data.polygonVertices[i].x * sinTheta + data.polygonVertices[i].y * cosTheta + pos.y;
                        
                        // 转换到SFML坐标系
                        polygon.setPoint(i, sf::Vector2f(m_window.getSize().x / 2 + x, m_window.getSize().y / 2 - y));
                    }
                    polygon.setFillColor(color);
                    m_window.draw(polygon);
                }
                break;
                }
            }
        }

        // 设置是否显示AABB
        void SetAABBDraw(bool draw) { m_AABBDraw = draw; }

    private:
        PhysicsThread &m_simulation;
        sf::RenderWindow &m_window;
        bool m_AABBDraw;

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
