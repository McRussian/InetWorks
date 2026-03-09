#include "crop_filter.h"
#include <algorithm>

CropFilter::CropFilter(int width, int height) 
    : width_(width), height_(height) {}

void CropFilter::Apply(Image& image) const {
    // Получаем текущие размеры
    int oldWidth = image.GetWidth();
    int oldHeight = image.GetHeight();
    
    // Вычисляем новые размеры (не больше исходных)
    int newWidth = std::min(width_, oldWidth);
    int newHeight = std::min(height_, oldHeight);
    
    // Если размеры не меняются - ничего не делаем
    if (newWidth == oldWidth && newHeight == oldHeight) {
        return;
    }
    
    // Создаём новое изображение нужного размера
    Image result(newWidth, newHeight);
    
    // Копируем пиксели из левого верхнего угла
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            Color c = image.GetPixel(x, y);
            result.SetPixel(x, y, c);
        }
    }
    
    // Заменяем изображение
    image = result;
}

const char* CropFilter::GetName() const {
    return "Crop";
}