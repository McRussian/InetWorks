#include <iostream>
#include <string>

using namespace std::literals;

std::string_view GetSubstring(const std::string& str, const std::string& query) {
    size_t query_index = str.find(query);
    if (query_index == std::string::npos) {
        return std::string_view();
    }

    return std::string_view(str.data() + query_index, query.size());
}

int main() {
    std::string str = "Hello world!"s;
    std::string_view str_view = GetSubstring(str, "world"s);

    std::cout << str_view << std::endl;

    std::cout << static_cast<const void*>(str.data()) << " " << static_cast<const void*>(str.data() + str.size()) << std::endl;
    std::cout << static_cast<const void*>(str_view.data()) << " " << static_cast<const void*>(str_view.data() + str_view.size()) << std::endl;

    return 0;
}

