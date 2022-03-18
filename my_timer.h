#ifndef __MY_TIMER_H__
#define __MY_TIMER_H__

#include "time.h"

typedef struct _my_timer {
    clock_t start;
    clock_t end;
    double time;
} my_timer_t;

void my_timer_start(my_timer_t *mt);
void my_timer_end(my_timer_t *mt);
void my_timer_print(my_timer_t *mt);

#endif
