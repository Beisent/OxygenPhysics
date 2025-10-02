#pragma once
#include "Systems/World.h"
#include "RenderData.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <memory>
#include <iostream>

namespace OxyPhysics
{

    // 测试模块专用的多线程物理模拟类
    class PhysicsThread
    {
    public:
        PhysicsThread()
        {
            m_world = std::make_unique<PhysicsWorld>();
        }

        ~PhysicsThread()
        {
            Stop();
        }

        // 启动物理模拟线程
        void Start()
        {
            if (!m_running)
            {
                m_running = true;
                m_simulationThread = std::thread(&PhysicsThread::SimulationThread, this);
            }
        }

        // 停止物理模拟线程
        void Stop()
        {
            if (m_running)
            {
                m_running = false;
                m_condition.notify_one();
                if (m_simulationThread.joinable())
                {
                    m_simulationThread.join();
                }
            }
        }

        // 获取物理世界（线程安全）
        PhysicsWorld &GetWorld()
        {
            return *m_world;
        }

        // 获取渲染所需的数据副本（线程安全）
        void GetRenderData(std::vector<RenderData> &renderData)
        {
            std::lock_guard<std::mutex> lock(m_renderDataMutex);
            renderData = m_renderDataCache;
        }

        // 设置物理模拟的时间步长
        void SetTimeStep(float dt)
        {
            m_timeStep = dt;
        }

        // 检查是否已准备好渲染数据
        bool IsReadyForRender() const
        {
            return m_hasRenderData;
        }

        // 手动更新物理模拟（单线程模式）
        void Update(float dt)
        {
            if (!m_running)
            {
                // 只有在线程未运行时才手动更新
                m_world->Step(dt);
                UpdateRenderData();
            }
        }

        // 启用/禁用多线程模式
        void SetMultithreaded(bool enable)
        {
            if (enable && !m_running)
            {
                Start();
            }
            else if (!enable && m_running)
            {
                Stop();
            }
        }

    private:
        // 物理模拟线程函数
        void SimulationThread()
        {
            try
            {
                // 确保初始时至少收集一次渲染数据
                UpdateRenderData();

                // 线程主循环
                while (m_running)
                {
                    // 执行物理模拟
                    {
                        // 计算时间步长
                        float dt = m_timeStep;
                        m_world->Step(dt);
                    }

                    // 更新渲染数据
                    UpdateRenderData();

                    // 控制模拟频率
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_timeStep * 1000)));
                }
            }
            catch (const std::exception &e)
            {
                // 异常处理，防止程序崩溃
                std::cerr << "Physics simulation thread exception: " << e.what() << std::endl;
                m_running = false;
            }
        }

        // 更新渲染数据缓存
        void UpdateRenderData()
        {
            std::lock_guard<std::mutex> renderLock(m_renderDataMutex);
            m_renderDataCache.clear();

            // 收集所有需要渲染的物体数据
            auto view = m_world->registry.view<TransformComponent, ShapeComponent, AABBComponent>();
            view.each([this](auto entity, auto &transform, auto &shape, auto &aabb)
                      {
            RenderData data;
            data.position = transform.position;
            data.rotation = transform.rotation;
            data.shapeType = shape.type;
            data.localPosition = shape.localPosition;
            data.localRotation = shape.localRotation;
            data.aabb = aabb;
            data.entityId = static_cast<uint32_t>(entt::to_integral(entity));
            
            // 根据形状类型存储相应的数据
            switch (shape.type) {
            case ShapeType::Circle:
                data.circleRadius = shape.circle.radius;
                break;
            case ShapeType::Box:
                data.boxSize = shape.box.size;
                break;
            case ShapeType::Polygon:
                data.polygonVertices = shape.polygon.vertices;
                break;
            }
            
            m_renderDataCache.push_back(data); });

            // 设置渲染数据准备就绪标志
            m_hasRenderData = !m_renderDataCache.empty();
        }

        // 物理世界
        std::unique_ptr<PhysicsWorld> m_world;

        // 线程相关
        std::thread m_simulationThread;
        std::mutex m_renderDataMutex;
        std::condition_variable m_condition;
        std::atomic<bool> m_running{false};

        // 时间步长相关
        float m_timeStep = 1.0f / 60.0f; // 默认60Hz

        // 渲染数据缓存
        std::vector<RenderData> m_renderDataCache;
        std::atomic<bool> m_hasRenderData{false};
    };

} // namespace OxyPhysics