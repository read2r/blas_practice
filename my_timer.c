#include <stdio.h>
#include "my_timer.h"

void my_timer_start(my_timer_t *mt) {
    mt->start = clock();
}

void my_timer_end(my_timer_t *mt) {
    mt->end = clock();
    mt->time = (double)(mt->end - mt->start) / CLOCKS_PER_SEC;
}

void my_timer_print(my_timer_t *mt) {
    printf("elapsed time : %lf\n", mt->time);
}
