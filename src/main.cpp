#include <iostream>
#include "framebuffer.h" // 必须包含画布菜单！
#include "rasterizer.h"
#include "geometry.h"


int main(){
    std::cout << "[系统提示] 师弟，软光栅护宗大阵已启动！" << std::endl;
    std::cout << "[下一步] 准备构建 FrameBuffer 画布..." << std::endl;

    // 【师兄的留白】：这里马上要填入初始化画布的代码

    FrameBuffer fb(800,600);

    fb.Clear(0x00000000);

    // for(int i=0; i<10; i++) {
    //     for(int j=0; j<10; j++) {
    //         int index = i*fb.GetWidth()+j;
    //         fb.SetPixel(index,0x0000FF00); 
    //     }
    // }
    // Rasterizer::DrawLine(fb,0,0,800,600,0x000000FF);

    // Vec3f a(200.0f,200.0f,10.0f);
    // Vec3f b(400.0f,200.0f,10.0f);
    // Vec3f c(300.0f,400.0f,5.0f);
    
    // Color colorA = 0x00FF0000;
    // Color colorB = 0x0000FF00;
    // Color colorC = 0x000000FF;

    // Rasterizer::DrawTriangle(a,b,c,colorA, colorB, colorC,fb);

    // ⚡ 绿色的背景大三角形 (Z = 10.0f，离眼睛很远)
    Vec3f bg_a(100.0f, 100.0f, 10.0f);
    Vec3f bg_b(700.0f, 100.0f, 10.0f);
    Vec3f bg_c(400.0f, 500.0f, 10.0f);

        //T * R * S 先缩放 (Scale)再旋转 (Rotate)最后平移 (Translate)！
    //从右往左算
    Matrix4x4 M = Matrix4x4::MakeTranslation(200.0f,0.0f,0.0f) * Matrix4x4::MakeScale(0.5f, 0.5f, 0.5f);

    Vec3f a_screen = ApplyTransform(bg_a, M);
    Vec3f b_screen = ApplyTransform(bg_b, M);
    Vec3f c_screen = ApplyTransform(bg_c, M);

    Rasterizer::DrawTriangle(a_screen, b_screen, c_screen, 0x0000FF00, 0x0000FF00, 0x0000FF00, fb);

    // ⚡ 红色的前景小三角形 (Z = 5.0f，离眼睛很近)
    // ⚠️ 注意：红色是【后画】的，在没有 Z-Buffer 之前，后画的肯定覆盖先画的。
    // 但是有了 Z-Buffer，红色的 Z (5.0) 小于绿色的 Z (10.0)，所以它能画出来。
    Vec3f fg_a(300.0f, 200.0f, 5.0f);
    Vec3f fg_b(500.0f, 200.0f, 5.0f);
    Vec3f fg_c(400.0f, 400.0f, 5.0f);
    Rasterizer::DrawTriangle(fg_a, fg_b, fg_c, 0x00FF0000, 0x00FF0000, 0x00FF0000, fb);

    // ⚡ 终极刁难：蓝色的测试小三角形 (Z = 20.0f，比绿色还远)
    // ⚠️ 这是最后画的！如果没有 Z-Buffer，蓝色会直接糊在红色和绿色上面。
    // 但是现在，它离得最远，Z-Buffer 阵法绝对会把它无情弹开，你在画面上根本看不见它！
    Vec3f test_a(350.0f, 250.0f, 20.0f);
    Vec3f test_b(450.0f, 250.0f, 20.0f);
    Vec3f test_c(400.0f, 350.0f, 20.0f);
    Rasterizer::DrawTriangle(test_a, test_b, test_c, 0x000000FF, 0x000000FF, 0x000000FF, fb);

    fb.SaveToPPM("test.ppm");
    
    return 0;
}
