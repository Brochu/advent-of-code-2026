#include "day.hpp"
#include "parse.hpp"
#include "timer.hpp"

char in[] = {
    #include "../inputs/day00.inc"
};
strview input { in, strlen(in) };

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    print_res(p1, "%i", 0);
    print_res(p2, "%i", 0);
    return 0;
}
