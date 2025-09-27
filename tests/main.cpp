#include "OxygenRender.h"
#include <entt/entt.hpp>
#include "Components.h"
#include "Factories.h"
#include "OxyMathLite.h"

using namespace OxyRender;
using namespace OxyPhysics;
using namespace OxygenMathLite;
int main()
{
    Window window(800, 600, "Simple2D");
    Renderer renderer(window);
    Graphics2D graphics2D(window, renderer);

    auto &timer = Timer::getInstance();
    timer.setTargetFPS(120);
    while (!window.shouldClose())
    {
        timer.update(window);
        double dt = timer.deltaTime();

        graphics2D.clear();
        graphics2D.begin();

        graphics2D.drawRect(-50, 50, 100, -100, {1, 0, 0, 0.5f});

        graphics2D.flush();

        window.update();
    }
    return 0;
}
