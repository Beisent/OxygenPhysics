#include "OxygenRender.h"
#include "PhysicsThread.h"
#include "Factories/Factories.h"
#include "Common/OxygenMathLite.h"
#include "draw.h"

using namespace OxyRender;
using namespace OxyPhysics;
using namespace OxygenMathLite;

int main()
{
    Window window(800, 600, "Simple2D");
    Renderer renderer(window);
    Graphics2D graphics2D(window, renderer);

    // 创建物理模拟系统
    PhysicsThread simulation;
    
    // 启动物理模拟线程
    simulation.Start();

    // 获取物理世界引用并创建物体
    auto& world = simulation.GetWorld();
    
    // // 创建几个物体
    // BodyDef body;
    // body.position = Vec2(0, 0);
    // body.velocity = Vec2(0, 10);
    // body.mass = 1.0;
    // body.isStatic = false;
    
    // auto circle = ShapeFactory::CreateCircle(100.0);
    // world.CreateRigid(body, circle);

    // 创建更多物体以测试多线程
    for (int i = 0; i < 10; i++) {
        BodyDef otherBody;
        otherBody.position = OxygenMathLite::Vec2(-50 + i * 10, -50);
        otherBody.velocity = OxygenMathLite::Vec2(0, 50+i*10);
        otherBody.mass = 1.0;
        otherBody.isStatic = false;
        
        world.CreateRigid(otherBody, ShapeFactory::CreateCircle(50.0));
    }

    // 创建调试渲染器
    PhysicsDebugDraw debugDraw(simulation, graphics2D);

    auto &timer = Timer::getInstance();
    timer.setTargetFPS(120);

    while (!window.shouldClose())
    {
        timer.update(window);

        // 渲染
        graphics2D.clear();
        graphics2D.begin();

        // 绘制物理世界中的物体
        debugDraw.Draw();

        graphics2D.flush();

        window.update();
    }

    // 停止物理模拟线程
    simulation.Stop();

    return 0;
}