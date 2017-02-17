#ifndef _TIMING_WHEEL_H_
#define _TIMING_WHEEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/queue.h>

typedef void (*wtimer_cb)(void *);

typedef struct wtimer {
    wtimer_cb func;
    void *data;
    long cycle;
    LIST_ENTRY(wtimer) entry;
} wtimer_t;

extern int timeout_init(long slot_size);
extern void timeout_timer(void);
extern void init_timer(wtimer_t *timer, wtimer_cb func, void *data);
extern void start_timer(wtimer_t *timer, long msec);
extern void stop_timer(wtimer_t *timer);

#ifdef __cpluscplus
}
#endif

#endif
