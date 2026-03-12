// src/GaussianBlurFilter.cpp
#include "blur_filter.h"
#include <algorithm>
#include <vector>

GaussianBlurFilter::GaussianBlurFilter(double sigma) : sigma_(sigma) {
    // Вычисляем размер ядра (обычно берут 3*sigma с каждой стороны)
    kernelSize_ = static_cast<int>(std::ceil(sigma_ * 3)) * 2 + 1;
    if (kernelSize_ < 3) kernelSize_ = 3;  // Минимальный размер
    if (kernelSize_ % 2 == 0) kernelSize_++;  // Нечётный размер
    
    ComputeKernel();
}

void GaussianBlurFilter::ComputeKernel() {
    kernel_.resize(kernelSize_);
    int center = kernelSize_ / 2;
    double sum = 0.0;
    
    // Вычисляем значения ядра по формуле Гаусса
    for (int i = 0; i < kernelSize_; i++) {
        int x = i - center;
        kernel_[i] = std::exp(-(x * x) / (2 * sigma_ * sigma_));
        sum += kernel_[i];
    }
    
    // Нормализуем ядро (сумма = 1)
    for (int i = 0; i < kernelSize_; i++) {
        kernel_[i] /= sum;
    }
}

Color GaussianBlurFilter::GetPixelReflect(const Image& image, int x, int y) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    // Отражение от границ (как зеркало)
    if (x < 0) x = -x - 1;
    if (x >= width) x = 2 * width - x - 1;
    if (y < 0) y = -y - 1;
    if (y >= height) y = 2 * height - y - 1;
    
    // На всякий случай проверяем, что координаты корректны
    x = std::max(0, std::min(x, width - 1));
    y = std::max(0, std::min(y, height - 1));
    
    return image.GetPixel(x, y);
}

void GaussianBlurFilter::ApplyHorizontal(const Image& src, Image& dst) const {
    int width = src.GetWidth();
    int height = src.GetHeight();
    int radius = kernelSize_ / 2;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sumR = 0.0;
            double sumG = 0.0;
            double sumB = 0.0;
            
            // Свёртка по горизонтали
            for (int i = 0; i < kernelSize_; i++) {
                int srcX = x + (i - radius);
                Color c = GetPixelReflect(src, srcX, y);
                
                sumR += c.GetR() * kernel_[i];
                sumG += c.GetG() * kernel_[i];
                sumB += c.GetB() * kernel_[i];
            }
            
            // Округляем и ограничиваем
            int newR = static_cast<int>(sumR + 0.5);
            int newG = static_cast<int>(sumG + 0.5);
            int newB = static_cast<int>(sumB + 0.5);
            
            newR = std::max(0, std::min(255, newR));
            newG = std::max(0, std::min(255, newG));
            newB = std::max(0, std::min(255, newB));
            
            dst.SetPixel(x, y, Color(
                static_cast<uint8_t>(newR),
                static_cast<uint8_t>(newG),
                static_cast<uint8_t>(newB)
            ));
        }
    }
}

void GaussianBlurFilter::ApplyVertical(const Image& src, Image& dst) const {
    int width = src.GetWidth();
    int height = src.GetHeight();
    int radius = kernelSize_ / 2;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sumR = 0.0;
            double sumG = 0.0;
            double sumB = 0.0;
            
            // Свёртка по вертикали
            for (int i = 0; i < kernelSize_; i++) {
                int srcY = y + (i - radius);
                Color c = GetPixelReflect(src, x, srcY);
                
                sumR += c.GetR() * kernel_[i];
                sumG += c.GetG() * kernel_[i];
                sumB += c.GetB() * kernel_[i];
            }
            
            // Округляем и ограничиваем
            int newR = static_cast<int>(sumR + 0.5);
            int newG = static_cast<int>(sumG + 0.5);
            int newB = static_cast<int>(sumB + 0.5);
            
            newR = std::max(0, std::min(255, newR));
            newG = std::max(0, std::min(255, newG));
            newB = std::max(0, std::min(255, newB));
            
            dst.SetPixel(x, y, Color(
                static_cast<uint8_t>(newR),
                static_cast<uint8_t>(newG),
                static_cast<uint8_t>(newB)
            ));
        }
    }
}

void GaussianBlurFilter::Apply(Image& image) const {
    int width = image.GetWidth();
    int height = image.GetHeight();
    
    // Промежуточное изображение после горизонтального размытия
    Image temp(width, height);
    
    // Применяем горизонтальное размытие
    ApplyHorizontal(image, temp);
    
    // Применяем вертикальное размытие к результату
    ApplyVertical(temp, image);
}

const char* GaussianBlurFilter::GetName() const {
    return "Gaussian Blur";
}