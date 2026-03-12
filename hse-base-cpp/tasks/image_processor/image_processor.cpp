// main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "src/image.h"
#include "src/loader.h"
#include "src/crop_filter.h"
#include "src/grayscale_filter.h"
#include "src/negative_filter.h"
#include "src/sharp_filter.h"

using namespace std;

void ShowHelp() {
    cout << "=== BMP Image Processor ===" << endl;
    cout << "Usage: image_processor input.bmp output.bmp [filters...]" << endl;
    cout << endl;
    cout << "Filters:" << endl;
    cout << "  -crop <width> <height>  - обрезает изображение до указанных размеров" << endl;
    cout << "                            (левая верхняя часть)" << endl;
    cout << "  -gs                      - преобразует в оттенки серого" << endl;
    cout << "                            по формуле: 0.299R + 0.587G + 0.114B" << endl;
    cout << "  -neg                     - преобразует в негатив" << endl;
    cout << "                            по формуле: R' = 255 - R, G' = 255 - G, B' = 255 - B" << endl;
    cout << "  -sharp                   - повышение резкости" << endl;
    cout << "                            с использованием матрицы:" << endl;
    cout << "                            [-1 -1 -1]" << endl;
    cout << "                            [-1  9 -1]" << endl;
    cout << "                            [-1 -1 -1]" << endl;
    cout << endl;
    cout << "Examples:" << endl;
    cout << "  image_processor foto.bmp result.bmp -crop 800 600" << endl;
    cout << "  image_processor foto.bmp result.bmp -gs" << endl;
    cout << "  image_processor foto.bmp result.bmp -neg" << endl;
    cout << "  image_processor foto.bmp result.bmp -sharp" << endl;
    cout << "  image_processor foto.bmp result.bmp -crop 800 600 -gs -neg -sharp" << endl;
}

Filter* CreateFilter(const string& name, const vector<string>& args) {
    if (name == "crop") {
        if (args.size() != 2) {
            cout << "Ошибка: фильтр crop требует 2 параметра" << endl;
            return nullptr;
        }
        
        int width = stoi(args[0]);
        int height = stoi(args[1]);
        
        return new CropFilter(width, height);
    }
    else if (name == "gs") {
        if (!args.empty()) {
            cout << "Ошибка: фильтр gs не принимает параметров" << endl;
            return nullptr;
        }
        
        return new GrayscaleFilter();
    }
    else if (name == "neg") {  // Фильтр Negative
        if (!args.empty()) {
            cout << "Ошибка: фильтр neg не принимает параметров" << endl;
            return nullptr;
        }
        
        return new NegativeFilter();
    }
    else if (name == "sharp") {  // Фильтр Sharpening
        if (!args.empty()) {
            cout << "Ошибка: фильтр sharp не принимает параметров" << endl;
            return nullptr;
        }
        
        return new SharpeningFilter();
    }
    // Если фильтр не найден
    cout << "Ошибка: неизвестный фильтр '" << name << "'" << endl;
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || string(argv[1]) == "--help") {
        ShowHelp();
        return 0;
    }
    
    if (argc < 3) {
        cout << "Ошибка: нужно указать входной и выходной файл" << endl;
        return 1;
    }
    
    string inputFile = argv[1];
    string outputFile = argv[2];
    
    cout << "=== IMAGE PROCESSOR ===" << endl;
    cout << "Входной файл:  " << inputFile << endl;
    cout << "Выходной файл: " << outputFile << endl;
    cout << endl;
    
    // Парсим фильтры
    vector<FilterParams> filterParams = ParseFilters(argc, argv);
    
    if (!filterParams.empty()) {
        cout << "Фильтры:" << endl;
        for (const auto& f : filterParams) {
            cout << "  -" << f.name;
            for (const auto& arg : f.args) {
                cout << " " << arg;
            }
            cout << endl;
        }
        cout << endl;
    }
    
    // Загружаем изображение
    cout << "Загрузка..." << endl;
    BMPLoader loader;
    Image img;
    
    if (!loader.Load(inputFile, img)) {
        cout << "Ошибка загрузки: " << loader.GetLastError() << endl;
        return 1;
    }
    
    cout << "  Размер: " << img.GetWidth() << "x" << img.GetHeight() << endl;
    cout << endl;
    
    // Применяем фильтры
    if (!filterParams.empty()) {
        cout << "Применение фильтров:" << endl;
        
        for (const auto& params : filterParams) {
            // Создаём фильтр
            Filter* filter = CreateFilter(params.name, params.args);
            
            if (filter == nullptr) {
                return 1;
            }
            
            cout << "  - " << filter->GetName() << "... ";
            
            // Применяем
            filter->Apply(img);
            
            cout << "готово" << endl;
            
            delete filter;  // не забываем удалить
        }
        cout << endl;
    }
    
    // Сохраняем результат
    cout << "Сохранение..." << endl;
    if (!loader.Save(outputFile, img)) {
        cout << "Ошибка сохранения: " << loader.GetLastError() << endl;
        return 1;
    }
    
    cout << "Готово! Размер результата: " << img.GetWidth() << "x" << img.GetHeight() << endl;
    return 0;
}