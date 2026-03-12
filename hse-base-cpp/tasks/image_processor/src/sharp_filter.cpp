// src/SharpeningFilter.cpp
#include "sharp_filter.h"
#include <algorithm>
#include <cmath>

SharpeningFilter::SharpeningFilter() {
    // Инициализируем матрицу повышения резкости
    matrix_ = {
        {0, -1, 0},
        {-1,  5, -1},
        {0, -1, 0}
    };
    
    // Сумма элементов матрицы = 1 (для сохранения яркости)
    divisor_ = 1;
}

Color SharpeningFilter::GetPixelSafe(const Image& image, int x, int y) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    // Обработка границ: используем ближайший существующий пиксель
    int safeX = std::max(0, std::min(x, width - 1));
    int safeY = std::max(0, std::min(y, height - 1));
    
    return image.GetPixel(safeX, safeY);
}

void SharpeningFilter::Apply(Image& image) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    // Создаём копию для чтения исходных пикселей
    Image original = image;
    
    // Применяем матрицу к каждому пикселю
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sumR = 0;
            float sumG = 0;
            float sumB = 0;
            
            // Проходим по всем элементам матрицы 3x3
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    // Получаем цвет соседнего пикселя (с учётом границ)
                    Color neighbor = GetPixelSafe(original, x + dx, y + dy);
                    
                    // Вес из матрицы (индексы: dy+1, dx+1)
                    int weight = matrix_[dy + 1][dx + 1];
                    
                    // Добавляем взвешенные значения
                    sumR += weight * neighbor.GetR();
                    sumG += weight * neighbor.GetG();
                    sumB += weight * neighbor.GetB();
                }
            }
            
            // Нормализуем (делим на сумму элементов матрицы)
            // В нашем случае сумма = 1, так что деление не нужно, но оставим для общности
            if (divisor_ != 0) {
                sumR /= divisor_;
                sumG /= divisor_;
                sumB /= divisor_;
            }
            
            // Ограничиваем значения в диапазоне [0, 255]
            int newR = static_cast<int>(sumR + 0.5f);
            int newG = static_cast<int>(sumG + 0.5f);
            int newB = static_cast<int>(sumB + 0.5f);
            
            newR = std::max(0, std::min(255, newR));
            newG = std::max(0, std::min(255, newG));
            newB = std::max(0, std::min(255, newB));
            
            // Устанавливаем новый цвет
            image.SetPixel(x, y, Color(
                static_cast<uint8_t>(newR),
                static_cast<uint8_t>(newG),
                static_cast<uint8_t>(newB)
            ));
        }
    }
}

const char* SharpeningFilter::GetName() const {
    return "Sharpening";
}