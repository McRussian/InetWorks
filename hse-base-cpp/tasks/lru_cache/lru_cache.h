#pragma once

#include <cstddef>
#include <optional>
#include <string>

class LRUCache {
public:
    explicit LRUCache(size_t capacity);

    size_t Size() const;
    size_t Capacity() const;

    void Clear() noexcept;

    std::optional<int> Get(const std::string& key);
    bool Put(const std::string& key, int value);
    bool Erase(const std::string& key);

    bool Pin(const std::string& key);
    bool Unpin(const std::string& key);

    void Merge(LRUCache& other);

private:
    size_t capacity_;
    // Your code goes here
};
