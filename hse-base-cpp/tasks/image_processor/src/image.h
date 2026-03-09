#pragma once

#include <vector>
#include "color.h"

class Image {
private:
    int width_;
    int height_;
    std::vector<std::vector<Color>> pixels_;  // [y][x]

public:
    Image();
    Image(int width, int height);
    
    int GetWidth() const;
    int GetHeight() const;
    bool IsEmpty() const;
    
    Color GetPixel(int x, int y) const;
    void SetPixel(int x, int y, const Color& c);
    
    void Resize(int newWidth, int newHeight);
    void Fill(const Color& c);
};