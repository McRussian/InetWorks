#pragma once

#include "filter.h"
#include <vector>

class EdgeFilter : public Filter {
private:
    std::vector<std::vector<int>> matrix_;  // Матрица 3x3
    int divisor_;  // Сумма элементов матрицы
    float threshold_;  // Порог для бинаризации
    
    // Вспомогательная функция для получения цвета пикселя с учётом границ
    Color GetPixelSafe(const Image& image, int x, int y) const;
    
    // Функция для вычисления яркости пикселя (по формуле из Grayscale)
    uint8_t GetLuminance(const Color& c) const;

public:
    EdgeFilter(float threshold);
    
    void Apply(Image& image) const override;
    const char* GetName() const override;
};