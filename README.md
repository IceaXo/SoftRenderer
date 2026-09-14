# SoftRenderer · C++ CPU 软件光栅器

用 C++17 在 CPU 上将三维顶点转换为二维像素，并输出 PPM 图像。当前程序绘制棋盘地面、墙面和彩色三角形，用于观察透视、颜色插值和遮挡关系。

这是个人图形学实践，工作内容覆盖向量/矩阵、MVP 变换、三角形光栅化、颜色与深度缓冲，以及文件输出。当前没有交互编辑器或完整游戏引擎。

## 已有实现与阅读顺序

| 顺序 | 模块 | 看什么 |
| --- | --- | --- |
| 1 | [src/main.cpp](src/main.cpp) | 场景顶点、MVP、视口映射与 `test.ppm` 输出 |
| 2 | [include/geometry.h](include/geometry.h) | 向量、矩阵、LookAt、透视投影与齐次变换 |
| 3 | [src/rasterizer.cpp](src/rasterizer.cpp) | 直线、三角形覆盖、重心坐标与透视校正颜色插值 |
| 4 | [src/framebuffer.cpp](src/framebuffer.cpp) | 颜色/深度缓冲与 PPM 序列化 |

```mermaid
flowchart LR
    V[三维顶点] --> M[MVP 变换与透视除法]
    M --> S[视口映射]
    S --> R[三角形覆盖与属性插值]
    R --> D[深度测试与写入]
    D --> F[FrameBuffer / PPM]
```

透视校正采用保留的齐次分量处理顶点颜色；没有据此声称已经实现完整纹理系统。Z-Buffer 处理遮挡，不能单凭它保证消除所有 Z-fighting。

## 构建与结果

当前 CMake 使用 MSVC 的 `/utf-8` 选项，以下按 Windows + Visual Studio 2022、CMake 3.10+ 准备。在仓库根目录执行：

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\SoftRenderer.exe
```

程序在当前工作目录写入 `test.ppm`。源码默认画布为 3840×2160；该尺寸不是帧率、吞吐量或抗锯齿性能的测试成绩。

## 当前状态

2026-09-14 完成展示说明与关键代码静态核对，未重新构建或运行。已知 `GetDepth` 越界分支缺少 `return`，需要修复后再完成边界验证；本轮文档更新没有修改源代码。跨编译器选项和渲染边界也待进一步整理。

[历史开发记录](https://github.com/IceaXo/SoftRenderer/blob/c3cdf316d241dc069bbb4db2aa21b528948d4906/README.md) 保留当时表述，当前范围以本页为准。

[更多项目](https://github.com/IceaXo)
