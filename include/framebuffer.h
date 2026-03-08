#pragma once
#include <vector>
#include <cstdint>
#include <string>

// 用一个 32 位的无符号整数表示颜色 (A-R-G-B)
// 比如 0x00FF0000 就是纯红
using Color = uint32_t;

class FrameBuffer{
private:
    int width;
    int height;

    std::vector<Color> color_buffer;

    std::vector<float> depth_buffer;

public:
    FrameBuffer(int w,int h);
    ~FrameBuffer() = default;
    
    void Clear(Color c);
    void SetPixel(int index,Color c);
    void SetPixel(int x,int y,Color c);
    void SaveToPPM(const std::string& filename);
    int GetWidth();
    int GetHeight();

    void SetDepth(int index, float z);
    float GetDepth(int index);
};