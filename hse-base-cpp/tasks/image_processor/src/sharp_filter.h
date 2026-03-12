#pragma once

#include "filter.h"
#include <vector>

class SharpeningFilter : public Filter {
private:
    std::vector<std::vector<int>> matrix_;  // Матрица 3x3
    int divisor_;  // Сумма элементов матрицы (для нормализации)
    
    // Вспомогательная функция для получения цвета пикселя с учётом границ
    Color GetPixelSafe(const Image& image, int x, int y) const;

public:
    SharpeningFilter();
    
    void Apply(Image& image) const override;
    const char* GetName() const override;
};