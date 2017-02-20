#include <stdlib.h>
#include <string.h>
#include "timingwheel.h"

#define LIST_FIRST(head) ((head)->lh_first)
#define TIMEOUT_ONE_TICK(tick) ((tick) = (((tick) + 1) % (g_timing_wheel->slot_size)))

LIST_HEAD(timerhead, wtimer);
typedef struct timer_slot {
    struct timerhead head;
} timer_slot_t;

typedef struct timing_wheel {
    long tick;
    long slot_size;
    timer_slot_t slots[1];
} timing_wheel_t;

static timing_wheel_t *g_timing_wheel = NULL;

int
timeout_init(long slot_size)
{
    int i = 0;
    timing_wheel_t *wheel = NULL;
    size_t memsize = (size_t)(sizeof(timing_wheel_t) + (size_t)slot_size * (sizeof(timer_slot_t)));

    wheel = (timing_wheel_t *)malloc(memsize);
    if (wheel == NULL) {
        return  -1; 
    }

    memset(wheel, (int)memsize, 0);
    wheel->slot_size = slot_size;

    for (i = 0; i < slot_size; i++) {
        LIST_INIT(&(wheel->slots[i].head));
    }

    g_timing_wheel = wheel;

    return 0;
}

void
init_timer(wtimer_t *timer, wtimer_cb func, void *data)
{
    timer->func = func;
    timer->data = data;
}

void
start_timer(wtimer_t *timer, long msec)
{
    long slot = 0;
    timer_slot_t *head = NULL;

    timer->cycle = msec / g_timing_wheel->slot_size;
    slot = (msec + g_timing_wheel->tick) % g_timing_wheel->slot_size; 
    head = &(g_timing_wheel->slots[slot]);
    LIST_INSERT_HEAD(&head->head, timer, entry);
}

void
stop_timer(wtimer_t *timer)
{
    LIST_REMOVE(timer, entry);
}

void
timeout_timer(void)
{
    wtimer_t *timer = NULL, *next = NULL;
    timing_wheel_t *wheel = g_timing_wheel;
    long slot = TIMEOUT_ONE_TICK(wheel->tick);
    timer_slot_t *head = &(wheel->slots[slot]);

    next = LIST_FIRST(&head->head);
    while((timer = next) != NULL ) {
        next = LIST_NEXT(timer, entry);
        if (timer->cycle-- == 0) {
            LIST_REMOVE(timer, entry);
            timer->func(timer->data);
        }
    }
}
