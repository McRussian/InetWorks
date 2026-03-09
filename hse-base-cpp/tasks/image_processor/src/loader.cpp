#include <fstream>
#include <algorithm>

#include "loader.h"

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

struct BMPInfo {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerM;
    int32_t yPixelsPerM;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

int BMPLoader::CalculatePadding(int width) const {
    return (4 - (width * 3) % 4) % 4;
}

bool BMPLoader::Load(const std::string& filename, Image& outImage) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        lastError_ = "Cannot open file";
        return false;
    }
    
    BMPHeader header;
    BMPInfo info;
    
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&info), sizeof(info));
    
    // Проверки
    if (header.type != 0x4D42) {
        lastError_ = "Not a BMP file";
        return false;
    }
    
    if (info.bitCount != 24) {
        lastError_ = "Only 24-bit BMP supported";
        return false;
    }
    
    if (info.compression != 0) {
        lastError_ = "Compressed BMP not supported";
        return false;
    }
    
    int width = info.width;
    int height = abs(info.height);
    bool topDown = info.height < 0;
    int padding = CalculatePadding(width);
    
    // Создаем изображение
    outImage.Resize(width, height);
    
    // Читаем пиксели
    file.seekg(header.offset);
    
    std::vector<uint8_t> row(width * 3 + padding);
    
    for (int y = 0; y < height; y++) {
        file.read(reinterpret_cast<char*>(row.data()), row.size());
        
        for (int x = 0; x < width; x++) {
            // BMP хранит BGR
            uint8_t blue = row[x * 3];
            uint8_t green = row[x * 3 + 1];
            uint8_t red = row[x * 3 + 2];
            
            outImage.SetPixel(x, y, Color(red, green, blue));
        }
    }
    
    // Переворачиваем если нужно
    if (!topDown) {
        for (int y = 0; y < height / 2; y++) {
            for (int x = 0; x < width; x++) {
                Color temp = outImage.GetPixel(x, y);
                outImage.SetPixel(x, y, outImage.GetPixel(x, height - 1 - y));
                outImage.SetPixel(x, height - 1 - y, temp);
            }
        }
    }
    
    file.close();
    lastError_ = "Success";
    return true;
}

bool BMPLoader::Save(const std::string& filename, const Image& image) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        lastError_ = "Cannot create file";
        return false;
    }
    
    int width = image.GetWidth();
    int height = image.GetHeight();
    int padding = CalculatePadding(width);
    int dataSize = (width * 3 + padding) * height;
    
    BMPHeader header;
    BMPInfo info;
    
    header.type = 0x4D42;
    header.size = sizeof(header) + sizeof(info) + dataSize;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = sizeof(header) + sizeof(info);
    
    info.size = sizeof(info);
    info.width = width;
    info.height = height;
    info.planes = 1;
    info.bitCount = 24;
    info.compression = 0;
    info.imageSize = dataSize;
    info.xPixelsPerM = 2835;
    info.yPixelsPerM = 2835;
    info.colorsUsed = 0;
    info.colorsImportant = 0;
    
    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    file.write(reinterpret_cast<char*>(&info), sizeof(info));
    
    // Сохраняем пиксели (BMP ожидает снизу вверх)
    std::vector<uint8_t> row(width * 3 + padding, 0);
    
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Color c = image.GetPixel(x, y);
            // Конвертируем RGB -> BGR
            row[x * 3] = c.GetB();
            row[x * 3 + 1] = c.GetG();
            row[x * 3 + 2] = c.GetR();
        }
        file.write(reinterpret_cast<char*>(row.data()), row.size());
    }
    
    file.close();
    lastError_ = "Success";
    return true;
}

std::string BMPLoader::GetLastError() const {
    return lastError_;
}