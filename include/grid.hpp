#pragma once

#include <math.h>
#include "types.hpp"

struct vec2 {
    i32 x, y;
    bool operator==(const vec2 &other) const = default;
};

inline vec2 operator+(vec2 a, vec2 b) {
    return { a.x + b.x, a.y + b.y };
}
inline vec2 operator-(vec2 a, vec2 b) {
    return { a.x - b.x, a.y - b.y };
}
inline vec2 operator*(vec2 a, i32 k) {
    return { a.x * k, a.y * k };
}

inline i32 manhattan(vec2 a, vec2 b) {
    return abs(b.x - a.x) + abs(b.y - a.y);
}

extern const vec2 DIRS4[4];
extern const vec2 DIRS8[8];