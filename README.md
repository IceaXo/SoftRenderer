# Tiny C++ Software Rasterizer (纯 CPU 3D 软光栅渲染引擎)

## 📌 项目简介
本项目是一个脱离任何图形 API（OpenGL/Vulkan/DirectX）和硬件加速（GPU）的纯 C++ 软件渲染器。通过从零手写数学库与渲染流水线，在 CPU 端实现了从 3D 顶点数据到 2D 屏幕像素的完整光栅化过程。

本项目旨在深入理解计算机图形学底层原理、C++ 内存管理以及 CPU 性能优化，是面向游戏引擎底层开发的硬核实战项目。

## 🚀 核心特性 (Core Features)
* **Zero Third-Party Dependencies**：零第三方依赖，纯手写 3D 数学库（向量点乘/叉乘、4x4 矩阵变换）。
* **Standard Rendering Pipeline**：完整实现 MVP（Model-View-Projection）矩阵空间变换与透视除法。
* **Rasterization Algorithms**：基于 Bresenham 的直线绘制，以及基于**重心坐标 (Barycentric Coordinates)** 和包围盒算法的三角形光栅化。
* **Depth Buffering (Z-Buffer)**：徒手维护一维深度缓冲数组，实现精确的 3D 几何遮挡剔除。
* **Perspective-Correct Interpolation**：基于 `1/Z` 的透视校正插值，解决纹理映射与顶点属性在 3D 空间中的透视畸变问题。
* **Back-face Culling**：基于向量叉乘的背面剔除算法，大幅降低多余像素的计算开销。

## 🛠️ 技术栈
* **Language**: Modern C++ (C++17)
* **Build System**: CMake
* **Architecture**: 面向对象的模块化设计 (Math / FrameBuffer / Rasterizer 解耦)

## 编译与运行
```bash
mkdir build && cd build
cmake -S . -B build
cmake --build build
.\SoftRenderer.exe

📅 开发日志 (README Dev Log) 更新
📅 [2026-03-07] 阶段一：图元装配·重心坐标与像素级颜色插值
摘要：对三角形光栅化管线进行了深度重构。摒弃了生硬的单色填充，基于重心坐标（Barycentric Coordinates）成功实现了顶点颜色的平滑插值过渡，并对底层热点循环（Hot Path）进行了极致的物理指令压榨。

🔺 图元着色 - 重心坐标插值 (Barycentric Interpolation)：
* 物理博弈：在包围盒的双层遍历中，复用叉乘面积数据，精确计算出当前像素点对于三个顶点的权重比例（$\alpha, \beta, \gamma$）。
* 像素混合：基于计算出的权重，对顶点颜色的 ARGB 通道进行独立拆解、加权求和与重组，实现像素级的高精度平滑着色。

⚡ 核心优化 - 极客级性能压榨 (Performance Tuning)：
* 浮点降维：由于浮点除法（FPU Division）极其昂贵，将 `1.0f / total_area` 预计算提至双层 for 循环外部，在百万级像素遍历中把除法强制转换为乘法，大幅削减 CPU 时钟周期消耗。
* 循环不变量外提 (Loop Invariant Code Motion)：将与像素坐标 $(x, y)$ 无关的顶点颜色位移解包运算（`>> 16` 与 `& 0xFF`）彻底剥离出内层循环，避免无意义的 ALU 算力浪费。

📅 [2026-03-06] 阶段一：图元装配·底层数学库与光栅化核心
摘要：成功实装了软光栅管线的核心基石。从零构建了 2D 几何数学库，并基于纯 CPU 运算完成了线框与实心面片的光栅化渲染，彻底打通了从数学模型到显存画布的物理映射。

⚔️ 2D 几何兵器库 (Geometry Math Library)：
* 从零手撕轻量级二维向量类 `Vec2i`，利用 `inline` 关键字极限压榨函数调用开销。
* 基于常量引用（`const &`）重载 `operator-`，并实装向量叉乘（Cross Product）核心算法，为后续的空间判定提供数学底座。

📐 图元装配 - 线框光栅化 (Wireframe Rasterization)：
* 实装 Bresenham 画线算法（Zingl 优化变体）。
* 物理降维：通过引入隐式方程误差项 `err`，将 X/Y 轴的步进判定进行代数融合，实现零浮点数、零除法的极速画线。彻底消灭传统算法中针对 8 个象限的 `if-else` 分支，大幅降低 CPU 分支预测失败的惩罚 (Branch Misprediction)。

🔺 图元装配 - 面片光栅化 (Triangle Rasterization)：
* 实装基于边缘方程 (Edge Equation) 的实心三角形渲染。
* 内存防护：计算 AABB 包围盒实现像素级视锥剔除，精准锁死 `[0, width-1]` 边界，从根源上杜绝显存指针越界 (Segfault)。
* 暴力处决：在双层循环中利用“三重叉乘判定”严密界定像素内外关系，成功在 FrameBuffer 中绘制出精确的纯色实心三角形。

🏗️ 架构重构 (Architecture Refactoring)：
* 规范面向对象设计，将 `DrawLine` 与 `DrawTriangle` 统一封装至 `Rasterizer` 光栅化器类的静态方法中，实现渲染逻辑与画布状态的完美解耦。

📅 [2026-03-05] 阶段零：辟地开天·环境肃清与内存画布
摘要：完成了软光栅渲染器的底座构建。攻克了 Windows 开发环境下最隐蔽的“头文件遮蔽”与“编码解析”双重天劫，实装了基础内存画布与 PPM 离屏输出。
内存画布阵法 (FrameBuffer)：

实装 FrameBuffer 类，采用 std::vector<uint32_t> 作为底层物理存贮，支持 A-R-G-B 32位像素格式。

实现 SaveToPPM 算法，通过二进制流将内存像素无损序列化为 .ppm 图像。

构建系统重塑 (Modern CMake)：

废弃传统的脚本编译，引入 CMakeLists.txt 管理多文件项目。

MSVC 兼容性补丁：通过 add_compile_options(/utf-8) 强制编译器识别 UTF-8 源码，彻底消灭了由中文注释引发的“换行符吞噬”乱码错误。

底层 Bug 勘破 (Post-Mortem)：

劫难复盘：排查并解决了由于本地存在 math.h 导致的 Header Shadowing (头文件遮蔽) 问题。该问题导致编译器错误地跳过了系统标准数学库，引发了超过 100 处 cstdlib 编译崩溃。

物理隔离：将项目整体迁移至纯英文路径（D:\SoftRenderer），规避了 Windows 用户名中文字符导致的包工头（CMake）逻辑闪退。