#pragma once

#include <string>
#include <string_view>
#include <vector>

class SearchIndex {
public:
    void Add(std::string_view document);

    std::vector<std::string_view> Query(std::string_view query, size_t max_results) const;

private:
    // Your code goes here
};
