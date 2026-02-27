#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

class BPETokenizer {
public:
    void Train(std::string_view text, int num_merges);
    std::vector<std::string> Encode(std::string_view word) const;
    const std::vector<std::pair<std::string, std::string>>& GetMergeRules() const;

private:
    std::vector<std::pair<std::string, std::string>> merge_rules_;
    // Your code goes here
};
