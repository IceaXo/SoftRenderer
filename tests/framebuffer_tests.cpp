#include "framebuffer.h"
#include <iostream>
#include <limits>

int main() {
    FrameBuffer buffer(2, 2);
    const float empty = std::numeric_limits<float>::max();
    buffer.SetDepth(0, 0.25f);
    buffer.SetDepth(3, 0.75f);
    buffer.SetDepth(-1, 7.0f);
    buffer.SetDepth(4, 7.0f);
    if (buffer.GetDepth(-1) != empty || buffer.GetDepth(4) != empty ||
        buffer.GetDepth(0) != 0.25f || buffer.GetDepth(3) != 0.75f) {
        std::cerr << "Depth bounds or valid values failed\n";
        return 1;
    }
    buffer.Clear(0);
    for (int i = 0; i < 4; ++i) {
        if (buffer.GetDepth(i) != empty) return 2;
    }
    FrameBuffer zero(0, 0);
    if (zero.GetDepth(0) != empty) return 3;
    return 0;
}
