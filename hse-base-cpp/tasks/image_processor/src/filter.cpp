#include "filter.h"

std::vector<FilterParams> ParseFilters(int argc, char* argv[]) {
    std::vector<FilterParams> filters;
    
    for (int i = 3; i < argc; i++) {
        std::string arg = argv[i];
        
        // Если это название фильтра (начинается с -)
        if (arg[0] == '-') {
            FilterParams filter;
            filter.name = arg.substr(1);  // убираем '-'
            
            // Собираем аргументы до следующего фильтра
            int j = i + 1;
            while (j < argc && argv[j][0] != '-') {
                filter.args.push_back(argv[j]);
                j++;
            }
            
            filters.push_back(filter);
            i = j - 1;  // обновляем счётчик
        }
    }
    
    return filters;
}
