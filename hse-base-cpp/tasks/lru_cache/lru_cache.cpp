#include "lru_cache.h"

// Your code goes here

LRUCache::LRUCache(size_t capacity) {
}

size_t LRUCache::Size() const {
    return 0;
}

size_t LRUCache::Capacity() const {
    return 0;
}

void LRUCache::Clear() noexcept {

}

std::optional<int> LRUCache::Get(const std::string& key) {
    return std::nullopt;
}

bool LRUCache::Put(const std::string& key, int value) {
    return false;
}

bool LRUCache::Erase(const std::string& key) {
    return false;
}

bool LRUCache::Pin(const std::string& key) {
    return false;
}

bool LRUCache::Unpin(const std::string& key) {
    return false;
}

void LRUCache::Merge(LRUCache& other) {
}
