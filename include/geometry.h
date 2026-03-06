#pragma once

 struct Vec2i{
    int x,y;

    Vec2i():x(0),y(0) {}
    Vec2i(int _x,int _y):x(_x),y(_y){}

    Vec2i operator-(const Vec2i& other) const{
        return Vec2i(x-other.x,y-other.y);
    }
 };

//直接把函数大括号里的代码，当成文本一样【粘贴】到调用它的地方
//消除了所有的函数跳跃开销
 inline int CrossProduct(const Vec2i& v1,const Vec2i& v2){
    return v1.x*v2.y-v1.y*v2.x;
 }