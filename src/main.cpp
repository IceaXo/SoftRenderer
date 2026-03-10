#include <iostream>
#include "framebuffer.h"
#include "rasterizer.h"
#include "geometry.h"

int main() {
    std::cout << "[系统提示] 师弟，软光栅终极视界走廊准备生成！" << std::endl;
    
    // 1. 初始化画布
    FrameBuffer fb(3840, 2160);
    fb.Clear(0x00000000); // 刷上纯黑的虚空背景

    // ====================================================================
    // 2. 核心法宝：MVP 矩阵绝对不能删！这是整个 3D 宇宙的骨架！
    // ====================================================================
    Matrix4x4 M = Matrix4x4::MakeTranslation(0.0f, 0.0f, 5.0f); // 把整个世界往前推 5 米
    Matrix4x4 V = Matrix4x4::MakeLookAt(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec3f(0.0f, 1.0f, 0.0f));
    Matrix4x4 P = Matrix4x4::MakePerspective(90.0f, 3840.0f/2160.0f, 0.1f, 100.0f);
    Matrix4x4 MVP = P * V * M; // 终极矩阵融合

    // ====================================================================
    // 3. 视口映射法阵 (把 [-1, 1] 的抽象坐标变成真实的屏幕像素)
    // ====================================================================
    auto viewport_transform = [](Vec4f& v) {
        v.x = (v.x + 1.0f) * 0.5f * 3840.0f;
        v.y = (1.0f - v.y) * 0.5f * 2160.0f; // Y轴反转
    };

    // ====================================================================
    // 4. 3D 打印机 (把顶点转换、视口映射、光栅化全部打包成一条流水线)
    // ====================================================================
    auto Draw3DTriangle = [&](Vec3f p1, Vec3f p2, Vec3f p3, Color c1, Color c2, Color c3) {
        // (1) 穿过 MVP 矩阵！
        Vec4f s1 = ApplyTransform(p1, MVP);
        Vec4f s2 = ApplyTransform(p2, MVP);
        Vec4f s3 = ApplyTransform(p3, MVP);
        
        // (2) 拉伸到屏幕像素！
        viewport_transform(s1);
        viewport_transform(s2);
        viewport_transform(s3);
        
        // (3) 画它！
        Rasterizer::DrawTriangle(s1, s2, s3, c1, c2, c3, fb);
    };

    // ====================================================================
    // 5. 见证奇迹的时刻：铺设 3D 场景
    // ====================================================================
    
    // (A) 铺设无限延伸的 3D 棋盘格地板
    for(int x = -5; x < 5; ++x) {
        for(int z = -4; z < 10; ++z) {
            Color color = ((x + z + 100) % 2 == 0) ? 0xFFFFFF : 0x555555; 
            Vec3f a(x, -2.0f, z);
            Vec3f b(x + 1.0f, -2.0f, z);
            Vec3f c(x, -2.0f, z + 1.0f);
            Vec3f d(x + 1.0f, -2.0f, z + 1.0f);
            Draw3DTriangle(a, b, c, color, color, color);
            Draw3DTriangle(c, b, d, color, color, color);
        }
    }

    // (B) 在走廊尽头竖立一堵红墙 (挡在 Z = 10.0f 的地方)
    for(int x = -5; x < 5; ++x) {
        for(int y = -2; y < 3; ++y) {
            Color color = ((x + y + 100) % 2 == 0) ? 0xFF0000 : 0x880000; 
            Vec3f a(x, y, 10.0f);
            Vec3f b(x + 1.0f, y, 10.0f);
            Vec3f c(x, y + 1.0f, 10.0f);
            Vec3f d(x + 1.0f, y + 1.0f, 10.0f);
            Draw3DTriangle(a, b, c, color, color, color);
            Draw3DTriangle(c, b, d, color, color, color);
        }
    }

    // (C) 终极绝杀：贯穿天地的巨剑！
    // 它的上面两个角在空中（离你很近，Z=2）
    Vec3f magic_a(-3.0f,  3.0f,  2.0f); 
    Vec3f magic_b( 3.0f,  3.0f,  2.0f); 
    // 它的底角极其暴躁：
    // Y = -4.0f（刺穿了 Y=-2 的地板！）
    // Z = 15.0f（捅破了 Z=10 的红墙！）
    Vec3f magic_c( 0.0f, -4.0f, 15.0f); 

    Draw3DTriangle(magic_a, magic_b, magic_c, 0xFF0000, 0x00FF00, 0x0000FF);

    // ====================================================================
    // 6. 出图结丹！
    // ====================================================================
    fb.SaveToPPM("test.ppm");
    std::cout << "[系统提示] 大圆满！请查收 test.ppm！" << std::endl;
    
    return 0;
}
// #include <iostream>
// #include "framebuffer.h" // 必须包含画布菜单！
// #include "rasterizer.h"
// #include "geometry.h"


// int main(){
//     std::cout << "[系统提示] 师弟，软光栅护宗大阵已启动！" << std::endl;
//     std::cout << "[下一步] 准备构建 FrameBuffer 画布..." << std::endl;

//     FrameBuffer fb(1280,720);

//     fb.Clear(0x00000000);

    
//     Color colorA = 0x00FF0000;
//     Color colorB = 0x0000FF00;
//     Color colorC = 0x000000FF;

//     // Rasterizer::DrawTriangle(a,b,c,colorA, colorB, colorC,fb);

//     // ⚡ 绿色的背景大三角形 (Z = 10.0f，离眼睛很远)
//     Vec3f bg_a(-1.0f, -1.0f, 0.0f);
//     Vec3f bg_b(1.0f, -1.0f, 0.0f);
//     Vec3f bg_c(0.0f,  1.0f, 3.0f);

//         //T * R * S 先缩放 (Scale)再旋转 (Rotate)最后平移 (Translate)！
//     //从右往左算
//     Matrix4x4 M = Matrix4x4::MakeTranslation(0.0f, 0.0f, 5.0f)*Matrix4x4::MakeScale(2.0f,2.0f,2.0f);

//     // 2. 摄像机矩阵（你在哪）
//     Matrix4x4 V = Matrix4x4::MakeLookAt(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec3f(0.0f, 1.0f, 0.0f));

//     // 3. 透视液压机（你用什么眼睛看）
//     Matrix4x4 P = Matrix4x4::MakePerspective(90.0f, 1280.0f/720.0f, 0.1f, 100.0f);

//     // 4. 终极融合：P * V * M （注意顺序，从右往左乘！）
//     Matrix4x4 MVP = P * V * M;

//     Vec4f a_screen = ApplyTransform(bg_a, MVP);
//     Vec4f b_screen = ApplyTransform(bg_b, MVP);
//     Vec4f c_screen = ApplyTransform(bg_c, MVP);

//     Rasterizer::DrawTriangle(a_screen, b_screen, c_screen, colorA,colorB,colorC, fb);
    


//     // // ⚡ 红色的前景小三角形 (Z = 5.0f，离眼睛很近)
//     // // ⚠️ 注意：红色是【后画】的，在没有 Z-Buffer 之前，后画的肯定覆盖先画的。
//     // // 但是有了 Z-Buffer，红色的 Z (5.0) 小于绿色的 Z (10.0)，所以它能画出来。
//     // Vec3f fg_a(300.0f, 200.0f, 5.0f);
//     // Vec3f fg_b(500.0f, 200.0f, 5.0f);
//     // Vec3f fg_c(400.0f, 400.0f, 5.0f);
//     // Rasterizer::DrawTriangle(fg_a, fg_b, fg_c, 0x00FF0000, 0x00FF0000, 0x00FF0000, fb);

//     // // ⚡ 终极刁难：蓝色的测试小三角形 (Z = 20.0f，比绿色还远)
//     // // ⚠️ 这是最后画的！如果没有 Z-Buffer，蓝色会直接糊在红色和绿色上面。
//     // // 但是现在，它离得最远，Z-Buffer 阵法绝对会把它无情弹开，你在画面上根本看不见它！
//     // Vec3f test_a(350.0f, 250.0f, 20.0f);
//     // Vec3f test_b(450.0f, 250.0f, 20.0f);
//     // Vec3f test_c(400.0f, 350.0f, 20.0f);
//     // Rasterizer::DrawTriangle(test_a, test_b, test_c, 0x000000FF, 0x000000FF, 0x000000FF, fb);

//     auto viewport_transform = [](Vec4f& v){
//         v.x = (v.x+1.0f)*0.5f*1280.0f;
//         v.y = (1.0f-v.y)*0.5f*720.0f;
//     };
//     viewport_transform(a_screen);
//     viewport_transform(b_screen);
//     viewport_transform(c_screen);

    
    
//     fb.SaveToPPM("test.ppm");
//     return 0;
// }
