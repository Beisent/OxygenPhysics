#include "OxygenRender.h"
#include <entt/entt.hpp>
#include "Components.h"
#include "Factories.h"
#include "math.h"
#include <random>

using namespace OxyRender;
using namespace OxyPhysics;

struct Color
{
    float r, g, b, a;
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

int main()
{
    Window window(800, 600, "ECS Physics Test");
    Renderer renderer(window);
    Graphics2D graphics2D(window, renderer);

    graphics2D.setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    auto &camera = graphics2D.getCamera();
    camera.setZoom(3.0f);

    entt::registry registry;

    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> posDistX(-400.0f, 400.0f);
    std::uniform_real_distribution<float> posDistY(-300.0f, 300.0f);
    std::uniform_real_distribution<float> velDist(-100.0f, 100.0f);
    std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
    std::uniform_real_distribution<float> sizeDist(20.0f, 50.0f);

    // ----------------- 创建多个实体 -----------------
    for(int i=0; i<10; ++i)
    {
        auto entity = registry.create();

        Vec2 pos(posDistX(rng), posDistY(rng));
        registry.emplace<TransformComponent>(entity, TransformComponent{pos, 0.0});
        registry.emplace<VelocityComponent>(entity, VelocityComponent{Vec2(velDist(rng), velDist(rng)), 0.0});
        registry.emplace<Color>(entity, colorDist(rng), colorDist(rng), colorDist(rng), 1.0f);

        int shapeType = i % 3; // 0: Circle, 1: Box, 2: Polygon
        if(shapeType == 0)
        {
            ShapeComponent circle = ShapeFactory::CreateCircle(sizeDist(rng));
            registry.emplace<ShapeComponent>(entity, circle);
        }
        else if(shapeType == 1)
        {
            ShapeComponent box = ShapeFactory::CreateBox(sizeDist(rng));
            registry.emplace<ShapeComponent>(entity, box);
        }
        else
        {
            std::vector<Vec2> verts = {
                { -sizeDist(rng)/2, -sizeDist(rng)/2 },
                { sizeDist(rng)/2, -sizeDist(rng)/2 },
                { sizeDist(rng)/2, sizeDist(rng)/2 },
                { -sizeDist(rng)/2, sizeDist(rng)/2 }
            };
            ShapeComponent poly = ShapeFactory::CreatePolygon(verts);
            registry.emplace<ShapeComponent>(entity, poly);
        }
    }

    // ----------------- 时间控制 -----------------
    const float TARGET_FPS = 60.0f;
    const float TIME_STEP = 1.0f / TARGET_FPS;
    float accumulator = 0.0f;

    while(!window.shouldClose())
    {
        Timer::getInstance().update(window);
        float dt = Timer::getInstance().deltaTime();
        if(dt > 0.25f) dt = 0.25f;
        accumulator += dt;

        graphics2D.clear();
        graphics2D.begin();
        graphics2D.drawAxis();

        // ----------------- 更新物理 -----------------
        while(accumulator >= TIME_STEP)
        {
            auto view = registry.view<TransformComponent, VelocityComponent>();
            view.each([&](auto &transform, auto &velocity)
            {
                transform.position.x += velocity.linearVelocity.x * TIME_STEP;
                transform.position.y += velocity.linearVelocity.y * TIME_STEP;
                transform.rotation += velocity.angularVelocity * TIME_STEP;
            });
            accumulator -= TIME_STEP;
        }

        // ----------------- 渲染 -----------------
        auto view2 = registry.view<TransformComponent, Color, ShapeComponent>();
        view2.each([&](auto &transform, auto &color, auto &shape)
        {
            Vec2 posOffset = transform.position + shape.localPosition;

            if(shape.type == ShapeType::Circle)
            {
                graphics2D.drawCircle(posOffset.x, posOffset.y, shape.circle.radius,
                                      {color.r, color.g, color.b, color.a});
            }
            else if(shape.type == ShapeType::Box)
            {
                double halfSize = shape.box.size / 2.0;
                graphics2D.drawRect(posOffset.x - halfSize, posOffset.y + halfSize,
                                    shape.box.size, -shape.box.size,
                                    {color.r, color.g, color.b, color.a});
            }
            else if(shape.type == ShapeType::Polygon)
            {
                std::vector<glm::vec2> points;
                points.reserve(shape.polygon.vertices.size());
                for(auto &v : shape.polygon.vertices)
                    points.emplace_back(posOffset.x + v.x, posOffset.y + v.y);

                graphics2D.drawPolygon(points, {color.r, color.g, color.b, color.a});
            }
        });

        graphics2D.flush();
        window.update();
    }

    return 0;
}
