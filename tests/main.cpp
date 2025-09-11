#include "OxygenRender.h"
#include <entt/entt.hpp>
#include "math.h"

using namespace OxyRender;
using namespace OxyPhysics;

struct Position
{
    Vec2 pos;
    Position(Vec2 p) : pos(p) {}
    Position(float x, float y) : pos(x, y) {} 
};

struct Velocity
{
    Vec2 ve;
    Velocity(Vec2 v) : ve(v) {}
    Velocity(float x, float y) : ve(x, y) {} 
};

struct Color
{
    float r, g, b, a;
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

int main()
{
    Window window(800, 600, "Simple2D");
    Renderer renderer(window);
    Graphics2D graphics2D(window, renderer);
    entt::registry registry;

    auto entity2 = registry.create();
    registry.emplace<Position>(entity2, -400.0f, 200.0f);
    registry.emplace<Velocity>(entity2, 80.0f, 0.0f);
    registry.emplace<Color>(entity2, 0.0f, 1.0f, 0.0f, 1.0f);
    
    const float TARGET_FPS = 60.0f;
    const float TIME_STEP = 1.0f / TARGET_FPS;
    float accumulator = 0.0f;

    while (!window.shouldClose())
    {
        Timer::getInstance().update(window);
        float dt = Timer::getInstance().deltaTime();

        if (dt > 0.25f)
            dt = 0.25f;

        accumulator += dt;

        graphics2D.clear();
        graphics2D.begin();

        while (accumulator >= TIME_STEP)
        {
            auto view = registry.view<Position, Velocity>();
            view.each([&](auto &pos, auto &vel)
                      { pos.pos += vel.ve * TIME_STEP; });
            accumulator -= TIME_STEP;
        }

        
        auto view2 = registry.view<Position, Color>();
        view2.each([&](auto &pos, auto &color)
                   { graphics2D.drawRect(pos.pos.x, pos.pos.y, 50, -50, {color.r, color.g, color.b, color.a}); });

        graphics2D.flush();
        window.update();
    }

    return 0;
}