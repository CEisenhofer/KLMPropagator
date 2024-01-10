#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <z3++.h>

#define EXP_THEORIES

enum Logic : unsigned char {
    C = 0,
    CL = 1,
    CM = 2,
    P = 3,
    M = 4,
};

using namespace z3;

inline bool starts_with(std::string const& s, std::string const& v) {
    if (v.size() > s.size())
        return false;
    return std::equal(v.begin(), v.end(), s.begin());
}

inline bool ends_with(std::string const& s, std::string const& v) {
    if (v.size() > s.size())
        return false;
    return std::equal(v.rbegin(), v.rend(), s.rbegin());
}

template<typename T>
class pointer_hash {
public:
    std::size_t operator()(T* const& p) const {
        return p->operator()();
    }
};

template<typename T>
class pointer_eq {
public:
    bool operator()(T* const& lhs, T* const& rhs) const {
        return *lhs == *rhs;
    }
};

namespace std {
    template<>
    struct hash<expr> {
        size_t operator()(const expr& e) const {
            return e.hash();
        }
    };

    template<>
    struct equal_to<expr> {
        bool operator()(const expr& lhs, const expr& rhs) const {
            return eq(lhs, rhs);
        }
    };
}