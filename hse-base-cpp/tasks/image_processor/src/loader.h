// BMPLoader.h
#pragma once

#include <string>
#include "image.h"

class BMPLoader {
private:
    std::string lastError_;
    int CalculatePadding(int width) const;

public:
    BMPLoader() = default;
    
    bool Load(const std::string& filename, Image& outImage);
    bool Save(const std::string& filename, const Image& image);
    std::string GetLastError() const;
};