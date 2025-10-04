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
