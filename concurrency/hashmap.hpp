#ifndef HASHMAP_HPP_
#define HASHMAP_HPP_
#include <murmurhash.hpp>
#include <mutex>
#include <memory>
#include <vector>
#include <unordered_map>
// golang-like implemention of hashmap

namespace cpputil {
    template<typename KeyT, typename ValueT>
    class HashMap {
        private:
        mutable std::mutex mu_;
        std::unordered_map<KeyT,ValueT> dirty_;
    };
}
#endif