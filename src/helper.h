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

class expr_hash {
public:
    std::size_t operator()(expr const& p) const {
        return p.hash();
    }
};

class expr_eq {
public:
    bool operator()(expr const& lhs, expr const& rhs) const {
        return eq(lhs, rhs);
    }
};
