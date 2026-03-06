#include "framebuffer.h"
#include <algorithm>
#include <iostream>
#include <fstream>  // 引入文件流法术，用于读写硬盘

FrameBuffer::FrameBuffer(int w,int h):width(w),height(h){
    color_buffer.resize(width*height,0x00000000);
}

void FrameBuffer::Clear(Color c){
    std::fill(color_buffer.begin(),color_buffer.end(),c);
}

void FrameBuffer::SetPixel(int x, int y, Color c){
    if (x<0||x>=width||y<0||y>=height) return;

    int index = y*width+x;
    color_buffer[index] = c;
}

void FrameBuffer::SaveToPPM(const std::string& filename){
    // ofstream 意思是 Output File Stream（输出文件流）。
    // 物理映射：你向操作系统申请修建一条通往 filename（比如 "test.ppm"）的专属管道。
    // ios::binary 是什么？这是“绝对保真协议”！
    // 在 Windows 系统里，如果你不加 binary，系统会自作聪明地把你发过去的换行符 '\n' 偷偷换成 '\r\n'。
    std::ofstream file(filename,std::ios::binary);

    // 如果硬盘满了，或者没有管理员权限，管道修建就会失败。
    if(!file.is_open()){
        std::cerr<<"文件打开失败"<<std::endl;
        return;
    }

    // 你的图片数据就是一堆 0 和 1。看图软件怎么知道这堆 0 和 1 是多大的图片？
    // 所以咱们必须在快递箱的最上面，贴一张“面单”。
    // "P6" 就是暗号，代表“这是一张彩色 PPM 图片”。
    // 接着写上 width（宽）和 height（高），最后写个 255（告诉看图软件，最亮的颜色值是 255）。
    file <<"P6\n"<<width<<" "<<height<<"\n255\n";

    // 把工厂里的几十万个像素，一个接一个地扔进管道里！
    for (int y=0;y<height;++y){
        for (int x=0;x<width;++x){
            // 拿到当前座位的颜色（32位大包裹）
            Color c = color_buffer[y*width+x];

            // 假设现在的颜色是 c = [00][FF][AA][11] (A=00, R=FF, G=AA, B=11)

            // 提取红色 (RR)：
            // 1. 把整体往右推 16 位：变成了 [00][00][00][FF]
            // 2. 用 & 0xFF 过滤：只留下最后的 [FF]！
            unsigned char r = (c >> 16) & 0xFF; 

            // 提取绿色 (GG)：
            // 1. 把整体往右推 8 位：变成了 [00][00][FF][AA]
            // 2. 用 & 0xFF 过滤：[FF] 被杀掉，只留下最后的 [AA]！
            unsigned char g = (c >> 8) & 0xFF; 

            // 提取蓝色 (BB)：
            // 它本来就在最右边，连推都不用推！
            // 直接用 & 0xFF 过滤：前面的全杀掉，只留下最后的 [11]！
            unsigned char b = c & 0xFF;
            // 把这三个小零件扔进管道 (file << ...) ，顺着管道流进硬盘里！
            file<<r<<g<<b;
        }    
    }
    // 极其重要！发完货必须关闭管道！
    // 如果你不调用 close，留在管道半路上的数据可能永远掉不进硬盘里，你的图片文件只有一半！
    file.close();
    std::cout<<"[FrameBuffer]已生成"<<filename<<std::endl;
}

int FrameBuffer::GetWidth(){
    return width;
}

int FrameBuffer::GetHeight(){
    return height;
}