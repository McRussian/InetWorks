// src/GrayscaleFilter.cpp
#include "grayscale_filter.h"

void GrayscaleFilter::Apply(Image& image) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color c = image.GetPixel(x, y);
            
            // Формула яркости: Y = 0.299*R + 0.587*G + 0.114*B
            uint8_t r = c.GetR();
            uint8_t g = c.GetG();
            uint8_t b = c.GetB();
            
            // Вычисляем яркость (используем float для точности)
            float gray = 0.299f * r + 0.587f * g + 0.114f * b;
            
            // Округляем до целого
            uint8_t grayByte = static_cast<uint8_t>(gray + 0.5f);
            
            // Устанавливаем все каналы одинаковыми
            image.SetPixel(x, y, Color(grayByte, grayByte, grayByte));
        }
    }
}

const char* GrayscaleFilter::GetName() const {
    return "Grayscale";
}