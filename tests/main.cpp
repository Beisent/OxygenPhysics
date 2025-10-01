#include "OxygenRender.h"
#include <entt/entt.hpp>
#include "Components.h"
#include "Factories.h"
#include "World.h"
#include "OxyMathLite.h"
#include "draw.h"

using namespace OxyRender;
using namespace OxyPhysics;
using namespace OxygenMathLite;

int main()
{
    Window window(800, 600, "Simple2D");
    Renderer renderer(window);
    Graphics2D graphics2D(window, renderer);

    // 创建物理世界
    PhysicsWorld world;

    // 创建几个物体
    BodyDef body;
    body.position = Vec2(0, 0);
    body.velocity = Vec2(0, 10);
    body.mass = 1.0;
    body.isStatic = false;
    
    auto circle = ShapeFactory::CreateCircle(10.0);
    world.CreateRigid(body, circle);


    // 创建调试渲染器
    PhysicsDebugDraw debugDraw(world, graphics2D);

    auto &timer = Timer::getInstance();
    timer.setTargetFPS(120);

    while (!window.shouldClose())
    {
        timer.update(window);
        double dt = timer.deltaTime();

        // 更新物理
        world.Step(static_cast<real>(dt));

        // 渲染
        graphics2D.clear();
        graphics2D.begin();

        // 绘制物理世界中的物体
        debugDraw.Draw();

        graphics2D.flush();

        window.update();
    }

    return 0;
}
