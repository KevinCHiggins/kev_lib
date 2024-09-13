#ifndef _KEV_PERF_TEST_HEADER_
#define _KEV_PERF_TEST_HEADER_
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
typedef struct kev_perf_timing
{
	int64_t calls_amount;
	double total_time_s;
	int64_t last_timestamp_ns;
} kev_perf_timing;

int64_t kev_perf_time_since_last_call_ns(kev_perf_timing *timing);
double kev_perf_time_since_last_call_s(kev_perf_timing *timing);
double kev_perf_rate_since_last_call(kev_perf_timing *timing);
void kev_perf_init();
#endif
