// src/GaussianBlurFilter.h
#pragma once

#include "filter.h"
#include <vector>
#include <cmath>

class GaussianBlurFilter : public Filter {
private:
    double sigma_;
    std::vector<double> kernel_;  // Одномерное ядро Гаусса
    int kernelSize_;
    
    // Вычисление одномерного ядра Гаусса
    void ComputeKernel();
    
    // Применение одномерной свёртки по горизонтали
    void ApplyHorizontal(const Image& src, Image& dst) const;
    
    // Применение одномерной свёртки по вертикали
    void ApplyVertical(const Image& src, Image& dst) const;
    
    // Получение значения пикселя с учётом границ (отражение)
    Color GetPixelReflect(const Image& image, int x, int y) const;

public:
    GaussianBlurFilter(double sigma);
    
    void Apply(Image& image) const override;
    const char* GetName() const override;
};