#include "rasterizer.h"
#include <cmath>
#include <algorithm>

void Rasterizer::DrawLine(FrameBuffer& fb,int x0,int y0,int x1,int y1,Color c){
    // 1. 计算总跨度：dx 是横向距离，dy 是纵向距离。
    int dx = std::abs(x1-x0);
    int dy = -std::abs(y1-y0);

    //sx 和 sy 决定了我们是往上走还是往下走，往左走还是往右走（1 或 -1）
    int sx = x0<x1?1:-1;
    int sy = y0<y1?1:-1;

    int err = dx+dy;
    int e2;

    while (true) {
        // 第一件事：在当前坐标点下像素！
        fb.SetPixel(x0, y0, c);

        // 如果走到终点，直接打破循环（大圆满！）
        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err; 
        // [Core] Bresenham's Line Algorithm (Alois Zingl Variant)
        // 物理意义：放弃了直观的斜率累加 (m = dy/dx) 逻辑。
        // 工程优化：
        // 1. 全程避免浮点数计算，纯整数 ALU 加减法与移位操作。
        // 2. 引入隐式方程误差项 err = dx + dy，巧妙合并了 X 轴与 Y 轴的步进判定。
        // 3. 彻底干掉了传统画线算法中针对 8 个象限的 if-else 分支，极大地降低了 CPU 分支预测失败的惩罚 (Branch Misprediction)。
        if (e2>=dy) {
            err+=dy;
            x0+=sx;
        }
        if (e2<=dx){
            err+=dx;
            y0+=sy;
        }
    }
}

void Rasterizer::DrawTriangle(const Vec3f& a,const Vec3f& b,const Vec3f& c,
    Color colorA,Color colorB, Color colorC,FrameBuffer& fb){
    
    //总面积
    Vec3f ab = b-a;
    Vec3f ac = c-a;
    Vec3f bc = c-b;
    Vec3f ca = a-c;
    float total_area = static_cast<float>(std::abs(CrossProduct2D(ab,ac)));
    float inv_area = 1.0f / total_area; //循环内做浮点数乘法

    if(total_area == 0.0f) return; //三点一线

    //2.提取颜色
    int rA = (colorA>>16)&0xFF;
    int gA = (colorA>>8)&0xFF;
    int bA = colorA&0xFF;

    int rB = (colorB>>16)&0xFF;
    int gB = (colorB>>8)&0xFF;
    int bB = colorB&0xFF;
                
    int rC = (colorC>>16)&0xFF;
    int gC = (colorC>>8)&0xFF;
    int bC = colorC&0xFF;

    //AABB包围盒
    int min_x = std::max(0,static_cast<int>(std::min({a.x,b.x,c.x})));
    int max_x = std::min(fb.GetWidth()-1,static_cast<int>(std::max({a.x,b.x,c.x})));
    int min_y = std::max(0,static_cast<int>(std::min({a.y,b.y,c.y})));
    int max_y = std::min(fb.GetHeight()-1,static_cast<int>(std::max({a.y,b.y,c.y})));
    
    for (int y=min_y;y<=max_y;++y){
        for (int x=min_x;x<=max_x;++x){
            Vec3f p(static_cast<float>(x),static_cast<float>(y),0.0f);
            Vec3f ap = p-a;
            Vec3f bp = p-b;
            Vec3f cp = p-c;

            float z1 = CrossProduct2D(ab,ap);
            float z2 = CrossProduct2D(bc,bp);
            float z3 = CrossProduct2D(ca,cp);

            if ((z1>=0&&z2>=0&&z3>=0)||(z1<=0&&z2<=0&&z3<=0)){
                //1.计算权重
                float alpha = std::abs(z2)*inv_area;
                float beta = std::abs(z3)*inv_area;
                float gamma = std::abs(z1)*inv_area;

                //新功能 1：通过三个顶点的 Z，插值算出当前像素点真实的物理深度
                float current_z = a.z*alpha+b.z*beta+c.z*gamma;
                int index = y*fb.GetWidth()+x;

                if (current_z < fb.GetDepth(index)){
                    //2.按权重混合
                    int r_new = static_cast<int>(rA*alpha+rB*beta+rC*gamma);
                    int g_new = static_cast<int>(gA*alpha+gB*beta+gC*gamma);
                    int b_new = static_cast<int>(bA*alpha+bB*beta+bC*gamma);

                    //4.打包为Color
                    Color final_color = (r_new<<16)|(g_new<<8)|b_new;
                    
                    //画像素
                    fb.SetPixel(index,final_color);
                    fb.SetDepth(index,current_z);
                }
            }
        }
    }
}