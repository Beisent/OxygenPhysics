#include "PhysicsThread.h"
#include "Factories/Factories.h"
#include "Common/OxygenMathLite.h"
#include "draw.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace OxyPhysics;
using namespace OxygenMathLite;

int main()
{
    // 创建SFML窗口 - 开启抗锯齿
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8; // 设置抗锯齿级别
    
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "OxygenPhysics with SFML", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(120);

    // 创建物理模拟系统
    PhysicsThread simulation;

    // 启动物理模拟线程
    simulation.Start();

    // 获取物理世界引用并创建物体
    auto &world = simulation.GetWorld();

    BodyDef Body;
    Body.position = OxygenMathLite::Vec2(-350 , -50);
    Body.velocity = OxygenMathLite::Vec2(50, 0);
    Body.angle = 30.0;
    Body.mass = 1.0;
    Body.isStatic = false;

    world.CreateRigid(Body, ShapeFactory::CreatePolygon({{0,50},{-50,-50},{50,-50}}));
    // 创建物体以测试多线程
    for (int i = 0; i < 5; i++)
    {
        BodyDef Body;
        Body.position = OxygenMathLite::Vec2(-300 + i * 105, -50);
        Body.velocity = OxygenMathLite::Vec2(0, 0);
        Body.mass = 1.0;
        Body.isStatic = false;

        world.CreateRigid(Body, ShapeFactory::CreateCircle(50.0));
    }

    // 创建调试渲染器 - 使用SFML窗口
    PhysicsDebugDraw debugDraw(simulation, window);

    // 主循环
    sf::Clock clock;
    while (window.isOpen())
    {
        // 处理事件
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // 清除窗口
        window.clear(sf::Color::Black);

        // 绘制物理世界中的物体
        debugDraw.Draw();

        // 显示窗口内容
        window.display();
    }

    // 停止物理模拟线程
    simulation.Stop();

    return 0;
}
