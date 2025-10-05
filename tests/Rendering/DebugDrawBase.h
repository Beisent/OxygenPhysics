#pragma once
#include "RenderData.h"
#include <vector>

namespace OxyPhysics
{
    class DebugDrawBase
    {
    public:
        virtual ~DebugDrawBase() = default;

        // 渲染物理世界的静态接口
        virtual void DrawFromSimulation() = 0;

        // 可直接绘制任意 RenderData 集合
        virtual void Draw(const std::vector<RenderData> &renderData) = 0;

        // 是否绘制调试AABB
        virtual void SetDrawAABB(bool enabled) = 0;

        // 获取当前后端类型
        virtual const char *GetBackendName() const = 0;
    };
}
