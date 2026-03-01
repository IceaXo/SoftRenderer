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
cmake ..
make
./SoftRenderer