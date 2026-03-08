#pragma once
#include "framebuffer.h"
#include "geometry.h"

class Rasterizer {
public:
    //不需要实例化对象
    static void DrawLine(FrameBuffer& fb,int x0,int y0,int x1,int y1,Color c);

    static void DrawTriangle(const Vec3f& a,const Vec3f& b,const Vec3f& c,
        Color colorA,Color colorB, Color colorC,FrameBuffer& fb);
};

