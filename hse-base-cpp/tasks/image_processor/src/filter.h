#pragma once
#include <string>
#include <vector>

#include "image.h"

struct FilterParams {
    std::string name;
    std::vector<std::string> args;
};

std::vector<FilterParams> ParseFilters(int argc, char* argv[]); 

class Filter {
public:
    virtual ~Filter() = default;
    
    // Виртуальная функция применения фильтра
    virtual void Apply(Image& image) const = 0;
    
    // Виртуальная функция для получения имени фильтра
    virtual const char* GetName() const = 0;
};