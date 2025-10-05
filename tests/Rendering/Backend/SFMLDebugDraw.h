#pragma once
#include "../DebugDrawBase.h"
#include "../Utils/CoordinateConverter.h"
#include "../Utils/ColorGenerator.h"
#include "../Utils/ShapeGeometry.h"
#include "PhysicsThread.h"
#include "../RenderData.h"
#include "Common/OxygenMathLite.h"
#include <SFML/Graphics.hpp>
#include <entt/entity/entity.hpp>
#include <cstdint>
#include <cmath>

namespace OxyPhysics
{
    class SFMLDebugDraw : public DebugDrawBase
    {
    public:
        SFMLDebugDraw(PhysicsThread &simulation, sf::RenderWindow &window)
            : m_simulation(simulation), m_window(window), m_drawAABB(true) {}

        void DrawFromSimulation() override
        {
            if (!m_simulation.IsReadyForRender()) return;
            std::vector<RenderData> data;
            m_simulation.GetRenderData(data);
            Draw(data);
        }

        void Draw(const std::vector<RenderData> &data) override
        {
            for (const auto &rd : data)
            {
                // 用 entity ID 通过hash生成固定颜色，与draw.h保持一致
                uint32_t id = static_cast<uint32_t>(entt::to_integral(rd.entityId));
                float r = hashToFloat(id);
                float g = hashToFloat(id * 7 + 3);
                float b = hashToFloat(id * 13 + 5);

                sf::Color color(static_cast<std::uint8_t>(r * 255), 
                                static_cast<std::uint8_t>(g * 255), 
                                static_cast<std::uint8_t>(b * 255));

                // 计算旋转和位置变换，与draw.h保持一致
                float cosTheta = std::cos(rd.rotation + rd.localRotation);
                float sinTheta = std::sin(rd.rotation + rd.localRotation);
                OxygenMathLite::Vec2 pos = rd.position + OxygenMathLite::Vec2{
                                                                rd.localPosition.x * cosTheta - rd.localPosition.y * sinTheta,
                                                                rd.localPosition.x * sinTheta + rd.localPosition.y * cosTheta};

                // 转换为SFML坐标系（与draw.h保持一致）
                sf::Vector2u windowSize = m_window.getSize();
                sf::Vector2f sfmlPos(windowSize.x / 2 + pos.x, windowSize.y / 2 - pos.y);

                // 绘制AABB，与draw.h保持一致
                if (m_drawAABB)
                {
                    OxygenMathLite::Vec2 min = rd.aabb.min;
                    OxygenMathLite::Vec2 max = rd.aabb.max;
                     
                    // 转换AABB到SFML坐标系，与draw.h保持一致
                    sf::Vector2f sfmlMin(windowSize.x / 2 + min.x, windowSize.y / 2 - min.y);
                    sf::Vector2f sfmlMax(windowSize.x / 2 + max.x, windowSize.y / 2 - max.y);
                     
                    // 创建AABB矩形，与draw.h保持一致
                    sf::RectangleShape aabbRect(sf::Vector2f(sfmlMax.x - sfmlMin.x, sfmlMin.y - sfmlMax.y));
                    aabbRect.setPosition(sf::Vector2f(sfmlMin.x, sfmlMax.y));
                    aabbRect.setOutlineThickness(1);
                    aabbRect.setFillColor(sf::Color::Transparent);
                     
                    // 默认红色，与draw.h保持一致
                    sf::Color aabbColor(255, 0, 0);

                    // 检查是否在碰撞对中改成蓝色，与draw.h保持一致
                    for (auto &pair : rd.collisionPairs)
                    {
                        if (pair.a == rd.entityId || pair.b == rd.entityId)
                        {
                            aabbColor = sf::Color(0, 0, 255);
                            break;
                        }
                    }
                    aabbRect.setOutlineColor(aabbColor);
                    m_window.draw(aabbRect);
                }

                // 绘制形状
                switch (rd.shapeType)
                {
                case ShapeType::Circle:
                    DrawCircle(rd, color, sfmlPos);
                    break;
                case ShapeType::Box:
                    DrawBox(rd, color, pos, cosTheta, sinTheta);
                    break;
                case ShapeType::Polygon:
                    DrawPolygon(rd, color, pos, cosTheta, sinTheta);
                    break;
                }
            }
        }

        void SetDrawAABB(bool enabled) override { m_drawAABB = enabled; }

        const char *GetBackendName() const override { return "SFML"; }

    private:
        PhysicsThread &m_simulation;
        sf::RenderWindow &m_window;
        bool m_drawAABB;
        
        void DrawCircle(const RenderData &rd, const sf::Color &color, const sf::Vector2f &sfmlPos)
        {
            // 与draw.h保持一致，使用实心填充
            sf::CircleShape circle(rd.circleRadius);
            circle.setPosition(sf::Vector2f(sfmlPos.x - rd.circleRadius, sfmlPos.y - rd.circleRadius));
            circle.setFillColor(color);
            m_window.draw(circle);
        }
        
        void DrawBox(const RenderData &rd, const sf::Color &color, const OxygenMathLite::Vec2 &pos, float cosTheta, float sinTheta)
        {
            // 与draw.h保持一致，使用ConvexShape和实心填充
            float halfSize = rd.boxSize * 0.5f;
            std::vector<OxygenMathLite::Vec2> vertices = {
                {-halfSize, -halfSize}, {halfSize, -halfSize}, {halfSize, halfSize}, {-halfSize, halfSize}};
            
            sf::ConvexShape box;
            box.setPointCount(4);
            sf::Vector2u windowSize = m_window.getSize();
            
            for (size_t i = 0; i < vertices.size(); ++i)
            {
                // 应用旋转和位置变换，与draw.h保持一致
                float x = vertices[i].x * cosTheta - vertices[i].y * sinTheta + pos.x;
                float y = vertices[i].x * sinTheta + vertices[i].y * cosTheta + pos.y;
                
                // 转换到SFML坐标系，与draw.h保持一致
                box.setPoint(i, sf::Vector2f(windowSize.x / 2 + x, windowSize.y / 2 - y));
            }
            box.setFillColor(color); // 实心填充，与draw.h保持一致
            m_window.draw(box);
        }
        
        void DrawPolygon(const RenderData &rd, const sf::Color &color, const OxygenMathLite::Vec2 &pos, float cosTheta, float sinTheta)
        {
            // 与draw.h保持一致，使用ConvexShape和实心填充
            sf::ConvexShape polygon;
            polygon.setPointCount(rd.polygonVertices.size());
            sf::Vector2u windowSize = m_window.getSize();
            
            for (size_t i = 0; i < rd.polygonVertices.size(); ++i)
            {
                // 应用旋转和位置变换，与draw.h保持一致
                float x = rd.polygonVertices[i].x * cosTheta - rd.polygonVertices[i].y * sinTheta + pos.x;
                float y = rd.polygonVertices[i].x * sinTheta + rd.polygonVertices[i].y * cosTheta + pos.y;
                
                // 转换到SFML坐标系，与draw.h保持一致
                polygon.setPoint(i, sf::Vector2f(windowSize.x / 2 + x, windowSize.y / 2 - y));
            }
            polygon.setFillColor(color); // 实心填充，与draw.h保持一致
            m_window.draw(polygon);
        }
        
        // 与draw.h保持一致的哈希函数
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
