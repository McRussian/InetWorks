#include "bpe_tokenizer.h"

void BPETokenizer::Train(std::string_view text, int num_merges) {
}

std::vector<std::string> BPETokenizer::Encode(std::string_view word) const {
    return {};
}

const std::vector<std::pair<std::string, std::string>>& BPETokenizer::GetMergeRules() const {
    return merge_rules_;
}
