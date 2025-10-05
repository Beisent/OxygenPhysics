#pragma once
#include "DebugDrawBase.h"
#include "PhysicsThread.h"
#include "Backend/SFMLDebugDraw.h"
#include <memory>

namespace OxyPhysics
{
    enum class RenderBackendType
    {
        SFML,
        ImGui,
        OpenGL,
        OxygenRender
    };

    class RenderBackendFactory
    {
    public:
        static std::unique_ptr<DebugDrawBase> Create(RenderBackendType type, PhysicsThread &simulation, void *context = nullptr)
        {
            {
                switch (type)
                {
                case RenderBackendType::SFML:
                    return std::make_unique<SFMLDebugDraw>(simulation, *static_cast<sf::RenderWindow *>(context));
                default:
                    return nullptr;
                }
            }
        }
    };
}
