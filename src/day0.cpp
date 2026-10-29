#include <stdio.h>

#include "day.hpp"
#include "parse.hpp"
#include "timer.hpp"

char in[] = {
    #include "../inputs/day00.inc"
};
strview input { in, strlen(in) };

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    strview *list = (strview *)arr_init(strview);
    sv_split(input, sv("\n\n"), &list);

    for (i32 i = 0; i < 2; i++) {
        printf(SV_FMT ",\n\n", SV_ARG(list[i]));
    }
    arr_free(list);

    print_res(p1, "%i", 0);
    print_res(p2, "%i", 0);
    return 0;
}
