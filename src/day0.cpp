#include <stdio.h>

#include "darray.hpp"
#include "day.hpp"
#include "parse.hpp"
#include "timer.hpp"

char in[] = {
    #include "../inputs/day0.inc"
};
strview input { in, strlen(in) };

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    u64 start = time_get_counter();

    strview *groups = arr_init<strview>();
    strview *items = arr_init<strview>();
    sv_split(input, sv("\n\n"), &groups);

    size_t len = arr_size(groups);
    for (size_t i = 0; i < len; i++) {
        arr_clear(items);
        sv_split(groups[i], sv("\n"), &items);
        size_t group_len = arr_size(items);
        //printf("item count = %lld\n", group_len);

        for (size_t j = 0; j < group_len; j++) {
            //printf("\t- " SV_FMT "\n", SV_ARG(items[j]));
        }
        //printf("\n");
    }
    arr_free(items);
    arr_free(groups);

    u64 end = time_get_counter();
    f64 secs = time_elapsed_seconds(start, end);
    printf("Time spent: %.3lf ms\n", secs * 1000.0);

    print_res(p1, "%i", 0);
    print_res(p2, "%i", 0);
    return 0;
}
