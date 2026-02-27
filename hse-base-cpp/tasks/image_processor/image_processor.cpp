// image_processor.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <stdexcept>

#pragma pack(push, 1)

// Заголовок файла BMP (BITMAPFILEHEADER)
struct BMPFileHeader {
    uint16_t bfType;      // Сигнатура (должна быть 'BM')
    uint32_t bfSize;      // Размер файла в байтах
    uint16_t bfReserved1; // Зарезервировано (0)
    uint16_t bfReserved2; // Зарезервировано (0)
    uint32_t bfOffBits;   // Смещение до пиксельных данных
};

// Информационный заголовок (BITMAPINFOHEADER)
struct BMPInfoHeader {
    uint32_t biSize;          // Размер этого заголовка (40 байт)
    int32_t  biWidth;         // Ширина изображения в пикселях
    int32_t  biHeight;        // Высота изображения в пикселях
    uint16_t biPlanes;        // Количество плоскостей (должно быть 1)
    uint16_t biBitCount;      // Бит на пиксель (24 для нашего случая)
    uint32_t biCompression;   // Тип сжатия (0 - без сжатия)
    uint32_t biSizeImage;     // Размер изображения в байтах
    int32_t  biXPelsPerMeter; // Горизонтальное разрешение
    int32_t  biYPelsPerMeter; // Вертикальное разрешение
    uint32_t biClrUsed;       // Количество используемых цветов
    uint32_t biClrImportant;  // Важные цвета
};

#pragma pack(pop)

// Структура пикселя (24 бита RGB)
struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

// Основная структура изображения
struct Image {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    std::vector<std::vector<Pixel>> pixels;
    
    int32_t GetWidth() const { return infoHeader.biWidth; }
    int32_t GetHeight() const { return std::abs(infoHeader.biHeight); }
    bool IsTopDown() const { return infoHeader.biHeight < 0; }
};

// Структура для хранения результатов парсинга
struct ParsedArgs {
    std::string inputFile;
    std::string outputFile;
    bool showHelp;
};

// Функция парсинга аргументов командной строки
ParsedArgs ParseArguments(int argc, char* argv[]) {
    ParsedArgs result;
    result.showHelp = false;
    
    // Если аргументов нет или запрошена помощь
    if (argc < 2) {
        result.showHelp = true;
        return result;
    }
    
    std::string firstArg = argv[1];
    if (firstArg == "-h" || firstArg == "--help") {
        result.showHelp = true;
        return result;
    }
    
    // Проверка наличия входного и выходного файлов
    if (argc < 3) {
        throw std::runtime_error("Input and output files are required");
    }
    
    result.inputFile = argv[1];
    result.outputFile = argv[2];
    
    return result;
}

// Функция вывода справки
void PrintHelp() {
    std::cout << "========================================\n";
    std::cout << "     IMAGE PROCESSOR - BMP FILTERS     \n";
    std::cout << "========================================\n\n";
    
    std::cout << "DESCRIPTION:\n";
    std::cout << "  Console application for applying filters to 24-bit BMP images\n\n";
    
    std::cout << "USAGE:\n";
    std::cout << "  image_processor <input.bmp> <output.bmp> [filters...]\n\n";
    
    std::cout << "FILTERS:\n";
    std::cout << "  (no filters implemented yet - this is a template)\n\n";
    
    std::cout << "EXAMPLES:\n";
    std::cout << "  image_processor input.bmp output.bmp\n";
    std::cout << "  image_processor input.bmp output.bmp -crop 800 600 -gs\n";
    std::cout << "  image_processor --help\n\n";
    
    std::cout << "NOTES:\n";
    std::cout << "  * Input file must be 24-bit uncompressed BMP\n";
    std::cout << "  * Output will be saved in the same format\n";
    std::cout << "  * Filters are applied in the order they appear\n";
    std::cout << "========================================\n";
}

// Функция чтения BMP
Image ReadBMP(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    Image image;
    
    // Чтение заголовка файла
    file.read(reinterpret_cast<char*>(&image.fileHeader), sizeof(BMPFileHeader));
    
    // Проверка сигнатуры
    if (image.fileHeader.bfType != 0x4D42) {
        throw std::runtime_error("Invalid BMP file format (signature not BM)");
    }
    
    // Чтение информационного заголовка
    file.read(reinterpret_cast<char*>(&image.infoHeader), sizeof(BMPInfoHeader));
    
    // Проверка формата
    if (image.infoHeader.biBitCount != 24) {
        throw std::runtime_error("Only 24-bit BMP files are supported");
    }
    if (image.infoHeader.biCompression != 0) {
        throw std::runtime_error("Only uncompressed BMP files are supported");
    }
    
    // Вычисление параметров
    int32_t width = image.GetWidth();
    int32_t height = image.GetHeight();
    int32_t bytesPerPixel = 3;
    int32_t rowStride = (width * bytesPerPixel + 3) & ~3;
    
    // Переход к пиксельным данным
    file.seekg(image.fileHeader.bfOffBits, std::ios::beg);
    
    // Чтение пикселей
    image.pixels.resize(height, std::vector<Pixel>(width));
    std::vector<uint8_t> rowData(rowStride);
    
    for (int32_t y = 0; y < height; ++y) {
        file.read(reinterpret_cast<char*>(rowData.data()), rowStride);
        
        for (int32_t x = 0; x < width; ++x) {
            int32_t offset = x * 3;
            image.pixels[y][x].blue = rowData[offset];
            image.pixels[y][x].green = rowData[offset + 1];
            image.pixels[y][x].red = rowData[offset + 2];
        }
    }
    
    // Переворот если изображение хранится снизу вверх
    if (!image.IsTopDown()) {
        std::reverse(image.pixels.begin(), image.pixels.end());
    }
    
    std::cout << "  ✓ Loaded: " << width << "x" << height << " pixels\n";
    return image;
}

// Функция записи BMP
void WriteBMP(const Image& image, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    
    // Подготовка данных
    int32_t width = image.GetWidth();
    int32_t height = image.GetHeight();
    int32_t bytesPerPixel = 3;
    int32_t rowStride = (width * bytesPerPixel + 3) & ~3;
    int32_t pixelDataSize = rowStride * height;
    
    // Создаем копию заголовков с обновленными значениями
    BMPFileHeader fileHeader = image.fileHeader;
    BMPInfoHeader infoHeader = image.infoHeader;
    
    fileHeader.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + pixelDataSize;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    infoHeader.biSizeImage = pixelDataSize;
    
    // Запись заголовков
    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BMPInfoHeader));
    
    // Подготовка пикселей к записи
    std::vector<std::vector<Pixel>> pixelsToWrite = image.pixels;
    if (!image.IsTopDown()) {
        std::reverse(pixelsToWrite.begin(), pixelsToWrite.end());
    }
    
    // Запись пиксельных данных
    std::vector<uint8_t> rowData(rowStride, 0);
    
    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            int32_t offset = x * 3;
            rowData[offset] = pixelsToWrite[y][x].blue;
            rowData[offset + 1] = pixelsToWrite[y][x].green;
            rowData[offset + 2] = pixelsToWrite[y][x].red;
        }
        file.write(reinterpret_cast<const char*>(rowData.data()), rowStride);
    }
    
    std::cout << "  ✓ Saved: " << filename << "\n";
}

int main(int argc, char* argv[]) {
    std::cout << "\n=== IMAGE PROCESSOR ===\n\n";
    
    try {
        // Парсинг аргументов
        ParsedArgs args = ParseArguments(argc, argv);
        
        // Проверка на запрос помощи
        if (args.showHelp) {
            PrintHelp();
            return 0;
        }
        
        // Вывод информации о запуске
        std::cout << "Input file:  " << args.inputFile << "\n";
        std::cout << "Output file: " << args.outputFile << "\n";
        std::cout << "\n";
        
        // Чтение изображения
        std::cout << "Reading BMP file...\n";
        Image image = ReadBMP(args.inputFile);
        
        // Сохранение результата
        std::cout << "\nWriting BMP file...\n";
        WriteBMP(image, args.outputFile);
        
        std::cout << "\nProcessing completed successfully!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "\nERROR: " << e.what() << "\n\n";
        std::cerr << "Use --help for usage information.\n";
        return 1;
    }
    
    return 0;
}