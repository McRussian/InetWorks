// src/NegativeFilter.cpp
#include "negative_filter.h"

void NegativeFilter::Apply(Image& image) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color c = image.GetPixel(x, y);
            
            // Формула негатива: R' = 255 - R, G' = 255 - G, B' = 255 - B
            // Так как значения от 0 до 255, а в формуле от 0 до 1
            uint8_t r = 255 - c.GetR();
            uint8_t g = 255 - c.GetG();
            uint8_t b = 255 - c.GetB();
            
            image.SetPixel(x, y, Color(r, g, b));
        }
    }
}

const char* NegativeFilter::GetName() const {
    return "Negative";
}