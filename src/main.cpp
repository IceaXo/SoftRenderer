#include <iostream>
#include "framebuffer.h" // 必须包含画布菜单！
#include "rasterizer.h"
#include "geometry.h"


int main(){
    std::cout << "[系统提示] 师弟，软光栅护宗大阵已启动！" << std::endl;
    std::cout << "[下一步] 准备构建 FrameBuffer 画布..." << std::endl;

    // 【师兄的留白】：这里马上要填入初始化画布的代码

    FrameBuffer fb(800,600);

    fb.Clear(0x00FF0000);

    for(int i=0; i<10; i++) {
        for(int j=0; j<10; j++) {
            fb.SetPixel(400+i, 300+j, 0x0000FF00); 
        }
    }
    Rasterizer::DrawLine(fb,0,0,800,600,0x000000FF);

    Vec2i a(200,200);
    Vec2i b(400,200);
    Vec2i c(300,400);
    
    Color colorA = 0x00FF0000;
    Color colorB = 0x0000FF00;
    Color colorC = 0x000000FF;

    Rasterizer::DrawTriangle(a,b,c,colorA, colorB, colorC,fb);
    fb.SaveToPPM("test.ppm");
    
    return 0;
}
