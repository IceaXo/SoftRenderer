#pragma once
#include "framebuffer.h"
#include "geometry.h"

class Rasterizer {
public:
    //不需要实例化对象
    static void DrawLine(FrameBuffer& fb,int x0,int y0,int x1,int y1,Color c);

    static void DrawTriangle(const Vec2i& a,const Vec2i& b,const Vec2i& c,uint32_t color,FrameBuffer& fb);
};

