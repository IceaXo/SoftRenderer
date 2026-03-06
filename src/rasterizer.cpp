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

void Rasterizer::DrawTriangle(const Vec2i& a,const Vec2i& b,const Vec2i& c,uint32_t color,FrameBuffer& fb){
    int min_x = std::max(0,std::min({a.x,b.x,c.x}));

    int max_x = std::min(fb.GetWidth()-1,std::max({a.x,b.x,c.x}));

    int min_y = std::max(0,std::min({a.y,b.y,c.y}));

    int max_y = std::min(fb.GetHeight()-1,std::max({a.y,b.y,c.y}));
    
    for (int y=min_y;y<=max_y;++y){
        for (int x=min_x;x<=max_x;++x){
            Vec2i p(x,y);

            Vec2i ab = b-a;
            Vec2i bc = c-b;
            Vec2i ca = a-c;

            Vec2i ap = p-a;
            Vec2i bp = p-b;
            Vec2i cp = p-c;

            int z1 = CrossProduct(ab,ap);
            int z2 = CrossProduct(bc,bp);
            int z3 = CrossProduct(ca,cp);

            if ((z1>=0&&z2>=0&&z3>=0)||(z1<=0&&z2<=0&&z3<=0)){
                fb.SetPixel(x,y,color);
            }
        }
    }
}