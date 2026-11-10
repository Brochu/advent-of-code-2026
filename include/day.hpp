#pragma once

#include "types.hpp"

#define ANS_SIZE 256

#define SV_FMT "%.*s"
#define SV_ARG(sv) (int)(sv).len, (sv).ptr
#define print_res(out, fmt, ...) sprintf_s(out, ANS_SIZE, fmt, __VA_ARGS__)

i32 solve(char p1[ANS_SIZE], char p2[ANS_SIZE]);
