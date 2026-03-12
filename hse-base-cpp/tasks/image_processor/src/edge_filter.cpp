#include "edge_filter.h"
#include <algorithm>
#include <cmath>

EdgeFilter::EdgeFilter(float threshold) : threshold_(threshold) {
    // Инициализируем матрицу для выделения границ
    // Если в матрице не указаны элементы, они равны 0
    matrix_ = {
        {0, -1, 0},
        {-1,  4, -1},
        {0, -1, 0}
    };
    
    // Сумма элементов матрицы = 0 (для выделения границ)
    divisor_ = 1;  // Не делим, так как сумма = 0
}

Color EdgeFilter::GetPixelSafe(const Image& image, int x, int y) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    // Обработка границ: используем ближайший существующий пиксель
    int safeX = std::max(0, std::min(x, width - 1));
    int safeY = std::max(0, std::min(y, height - 1));
    
    return image.GetPixel(safeX, safeY);
}

uint8_t EdgeFilter::GetLuminance(const Color& c) const {
    // Формула яркости из Grayscale: 0.299*R + 0.587*G + 0.114*B
    float gray = 0.299f * c.GetR() + 0.587f * c.GetG() + 0.114f * c.GetB();
    return static_cast<uint8_t>(gray + 0.5f);
}

void EdgeFilter::Apply(Image& image) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    // Шаг 1: Переводим в оттенки серого
    Image grayscale(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t lum = GetLuminance(image.GetPixel(x, y));
            grayscale.SetPixel(x, y, Color(lum, lum, lum));
        }
    }
    
    // Шаг 2: Создаём изображение для результата
    Image result(width, height);
    
    // Шаг 3: Применяем матрицу для выделения границ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sum = 0;
            
            // Проходим по всем элементам матрицы 3x3
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    // Получаем яркость соседнего пикселя
                    Color neighbor = GetPixelSafe(grayscale, x + dx, y + dy);
                    uint8_t lum = neighbor.GetR();  // В grayscale все каналы равны
                    
                    // Вес из матрицы (индексы: dy+1, dx+1)
                    int weight = matrix_[dy + 1][dx + 1];
                    
                    // Добавляем взвешенное значение
                    sum += weight * lum;
                }
            }
            
            // Применяем порог
            uint8_t newValue;
            if (sum > threshold_) {
                newValue = 255;  // Белый - граница
            } else {
                newValue = 0;    // Чёрный - не граница
            }
            
            result.SetPixel(x, y, Color(newValue, newValue, newValue));
        }
    }
    
    // Заменяем исходное изображение результатом
    image = result;
}

const char* EdgeFilter::GetName() const {
    return "Edge Detection";
}