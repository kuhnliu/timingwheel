#include "timingwheel.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "CppUTest/TestHarness.h"
struct test_arg {
    long start_msec;
    long timeout_msec;
    int fired;
};

static void timeout_callback(void *arg)
{
    struct timeval cur_time;
    struct test_arg *_arg = (struct test_arg *)arg;

    gettimeofday(&cur_time, NULL);
    printf("current time %ld msec, timer start at %ld msec, timer %ld msec\n",
            cur_time.tv_sec*1000 + cur_time.tv_usec/1000, _arg->start_msec, _arg->timeout_msec);
    _arg->fired = 1;
}

TEST_GROUP(TimingWheel)
{
	void setup()
	{
        timeout_init(1024*1024);
	}

	void teardown()
	{
	}
};

TEST(TimingWheel, timeout_success)
{
    int i = 0;
    wtimer_t mytimer;
    struct test_arg arg;
    struct timeval cur_time;

    gettimeofday(&cur_time, NULL);
    arg.start_msec = cur_time.tv_sec*1000 + cur_time.tv_usec/1000;
    arg.timeout_msec = 10 + arg.start_msec;
    arg.fired = 0;

    init_timer(&mytimer, timeout_callback, &arg);
    start_timer(&mytimer, 10);

    while (i++ < 10) {
        usleep(1000);
        timeout_timer();
    }
    LONGS_EQUAL(1, arg.fired);
}

TEST(TimingWheel, two_timeout_success)
{
    int i = 0;
    wtimer_t mytimer1;
    wtimer_t mytimer2;
    struct test_arg arg1;
    struct test_arg arg2;
    struct timeval cur_time;

    gettimeofday(&cur_time, NULL);
    arg1.start_msec = cur_time.tv_sec*1000 + cur_time.tv_usec/1000;
    arg1.timeout_msec = 10 + arg1.start_msec;
    arg1.fired = 0;
    arg2.start_msec = cur_time.tv_sec*1000 + cur_time.tv_usec/1000;
    arg2.timeout_msec = 10 + arg2.start_msec;
    arg2.fired = 0;

    init_timer(&mytimer1, timeout_callback, &arg1);
    init_timer(&mytimer2, timeout_callback, &arg2);
    start_timer(&mytimer1, 10);
    start_timer(&mytimer2, 10);

    while (i++ < 10) {
        usleep(1000);
        timeout_timer();
    }
    LONGS_EQUAL(1, arg1.fired);
    LONGS_EQUAL(1, arg2.fired);
}

TEST(TimingWheel, timeout_failure)
{
    int i = 0;
    wtimer_t mytimer;
    struct test_arg arg;
    struct timeval cur_time;

    gettimeofday(&cur_time, NULL);
    arg.start_msec = cur_time.tv_sec*1000 + cur_time.tv_usec/1000;
    arg.timeout_msec = 10 + arg.start_msec;
    arg.fired = 0;

    init_timer(&mytimer, timeout_callback, &arg);
    start_timer(&mytimer, 10);

    while (i++ < 9) {
        usleep(1000);
        timeout_timer();
    }
    LONGS_EQUAL(0, arg.fired);
}

TEST(TimingWheel, longtime_timeout_success)
{
    int i = 0;
    int longtime = 1024*1024*3+1024;
    wtimer_t mytimer;
    struct test_arg arg;
    struct timeval cur_time;

    gettimeofday(&cur_time, NULL);
    arg.start_msec = cur_time.tv_sec*1000 + cur_time.tv_usec/1000;
    arg.timeout_msec = longtime + arg.start_msec;
    arg.fired = 0;

    init_timer(&mytimer, timeout_callback, &arg);
    start_timer(&mytimer, longtime);

    while (i++ < longtime) {
        timeout_timer();
    }
    LONGS_EQUAL(1, arg.fired);
}

