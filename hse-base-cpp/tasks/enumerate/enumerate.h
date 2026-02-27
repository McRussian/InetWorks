#pragma once

#include <iterator>

template <typename Iterator>
class IteratorRange {
private:
    Iterator begin_;
    Iterator end_;

public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    Iterator begin() const {  // NOLINT
        return begin_;
    }

    Iterator end() const {  // NOLINT
        return end_;
    }
};

template <typename Container>
auto Enumerate(Container& container) {
} 