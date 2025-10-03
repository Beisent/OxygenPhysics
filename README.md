
#  OxygenPhysics

**OxygenPhysics** 是一个轻量级、高性能的 **2D 游戏物理引擎**，采用 **ECS（实体-组件-系统）与 OOP（面向对象）混合架构**，专为 2D 游戏场景设计，兼顾开发效率与运行性能。

> ✨ **核心目标**：提供稳定、高效、可扩展的 2D 物理模拟能力，同时保持代码简洁与架构清晰。

---

## 🚀 主要特性

### 🔧 核心物理能力
- **刚体动力学**：支持静态/动态刚体，含位置、速度、质量、惯量等完整物理属性。
- **多种碰撞形状**：圆形（Circle）、矩形（Box）、任意凸多边形（Polygon）。
- **高精度碰撞检测**：
  - 基于 **GJK 算法** 快速判断碰撞。
  - 使用 **EPA 算法** 计算精确的接触点、法线与穿透深度。
- **空间加速结构**：基于 **AABB Tree** 的高效碰撞筛选（开发中）。
- **关节约束系统**：计划支持旋转关节、距离关节、棱柱关节、焊接关节等（开发中）。

### 🏗️ 架构设计亮点
- **ECS + OOP 混合架构**：结合 ECS 的数据局部性优势与 OOP 的逻辑封装清晰性。
- **基于 [entt](https://github.com/skypjack/entt)**：使用业界高性能 ECS 框架管理实体与组件。
- **测试多线程支持**：测试代码中提供物理模拟独立线程，避免阻塞渲染主线程。
- **模块化 & 可扩展**：组件与系统解耦，便于功能扩展与定制。

---

## 📂 项目结构

```text
OxygenPhysics/
├── include/
│   ├── Common/         # 数学工具、常量、辅助函数
│   ├── Components/     # ECS 组件定义（Transform, Velocity, Shape 等）
│   ├── Factories/      # 刚体与形状创建工厂
│   └── Systems/        # 物理系统（碰撞检测、积分器、约束求解等）
├── src/                # 实现文件
├── tests/              # 单元测试与使用示例
└── vendor/             # 第三方依赖（entt, glm 等）
```

---

## 🧩 核心组件概览

| 模块         | 组件/类                          | 说明                               |
| ------------ | -------------------------------- | ---------------------------------- |
| **形状系统** | `ShapeComponent`, `ShapeFactory` | 支持 Circle/Box/Polygon 的统一接口 |
| **刚体系统** | `PhysicsWorld`, `RigidFactory`   | 物理世界管理与刚体创建入口         |
|              | `TransformComponent`             | 位置 + 旋转                        |
|              | `VelocityComponent`              | 线速度 + 角速度                    |
|              | `MassComponent`                  | 质量、转动惯量、是否静态           |
| **碰撞系统** | `AABBComponent`                  | 轴对齐包围盒                       |
|              | `ContactManifold`                | 存储碰撞接触信息                   |
|              | `GeometryTools`                  | GJK/EPA 所需的几何支持函数         |

---

## 🧪 快速开始

### 1. 创建物理世界
```cpp
// 单线程模式
OxyPhysics::PhysicsWorld world;

// 或启用多线程模拟
OxyPhysics::PhysicsThread simulation;
simulation.Start();
auto& world = simulation.GetWorld();
```

### 2. 创建刚体
```cpp
// 圆形刚体
auto circle = world.CreateCircleRigid({0, 0}, 10.0f, 1.0f, false);

// 矩形刚体
auto box = world.CreateBoxRigid({50, 0}, {20, 20}, 1.0f, false);

// 多边形刚体（凸多边形）
auto polygon = world.CreatePolygonRigid(
    {100, 0},
    {{0, 10}, {-10, 0}, {10, 0}},
    1.0f,
    false
);

// 创建刚体
BodyDef Body;
Body.position = OxygenMathLite::Vec2(-50 + i * 10, -50);
Body.velocity = OxygenMathLite::Vec2(0, 50+i*10);
Body.mass = 1.0;
Body.isStatic = false;
        
world.CreateRigid(Body, ShapeFactory::CreateCircle(50.0));
```

### 3. 运行模拟
```cpp
// 单线程：手动推进
world.Step(1.0f / 60.0f);

// 多线程：自动更新，仅需获取渲染数据
std::vector<OxyPhysics::RenderData> renderData;
simulation.GetRenderData(renderData);
```

---

## ⚙️ 技术实现细节（开发中）

| 模块             | 状态       | 说明                                     |
| ---------------- | ---------- | ---------------------------------------- |
| **GJK 碰撞检测** | ✅ 基础实现 | 支持凸体间碰撞判定，含单纯形管理         |
| **EPA 接触生成** | 🚧 进行中   | 计算精确接触信息                         |
| **AABB Tree**    | 🚧 计划中   | 动态构建、高效重叠查询                   |
| **关节约束系统** | 🚧 规划中   | 支持 Revolute/Distance/Prismatic/Weld 等 |

---

## 📦 依赖项

- **[entt](https://github.com/skypjack/entt)**：高性能 ECS 框架（已包含在 `vendor/`）
- **[glm](https://github.com/g-truc/glm)**（可选）：用于 `OxygenRender` 可视化模块
- **OxygenRender**（可选）：配套的调试渲染器（独立项目）

> 所有依赖均为头文件库，无需额外编译。

---

## 🛠️ 构建说明

项目使用 **CMake** 构建，支持 Windows / Linux / macOS。

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

> 更详细的构建选项与 IDE 配置将在后续文档中提供。

---

## 🗺️ 开发路线图

1. ✅ 完成 GJK/EPA 碰撞检测核心
2. 🚧 实现动态 AABB Tree 加速结构
3. 🚧 开发关节约束系统
4. 🚧 优化数值稳定性与性能（如休眠机制、积分器改进）
5. 📚 完善 API 文档与交互式示例

---

## 📄 许可证

本项目采用 **[MIT License](LICENSE)**，欢迎自由使用、修改与分发。

---

> 💡 **欢迎贡献！** 如果你对 2D 物理引擎、ECS 架构或游戏开发感兴趣，欢迎提交 Issue 或 PR！

---
