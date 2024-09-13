#define _POSIX_C_SOURCE 199309L
#include "kev_perf_timer.h"

#ifdef _WIN32
#include <windows.h>
int64_t performance_freq;
LARGE_INTEGER count;
void kev_perf_init()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	performance_freq = freq.QuadPart;
}
int64_t kev_perf_time_since_last_call_ns(kev_perf_timing *timing)
{
	if (timing->calls_amount == 0) return 0.0;
	timing->calls_amount += 1;
	QueryPerformanceCounter(&count);
	int64_t current_ns = count.QuadPart * 1000000000 / performance_freq;
	int64_t delta_ns = current_ns - timing->last_timestamp_ns;
	timing->last_timestamp_ns = current_ns;
	timing->total_time_s += delta_ns / 1000000000.0;
	return delta_ns;
}

#endif
#ifdef __linux__

void kev_perf_init()
{
}
int64_t kev_perf_time_since_last_call_ns(kev_perf_timing *timing)
{
	timing->calls_amount += 1;
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	float current_ns = ts.tv_nsec; // leaves small chance delta measurement will straddle second boundary
	int64_t delta_ns = current_ns - timing->last_timestamp_ns;
	timing->last_timestamp_ns = current_ns;
	timing->total_time_s += delta_ns / 1000000000.0;
	if (timing->calls_amount == 1) return 0.0;
	return delta_ns;
}

#endif

double kev_perf_time_since_last_call_s(kev_perf_timing *timing)
{
	return kev_perf_time_since_last_call_ns(timing) / 1000000000.0;
}
double kev_perf_rate_since_last_call(kev_perf_timing *timing)
{
	double seconds_since_last_call = kev_perf_time_since_last_call_s(timing);
	if (seconds_since_last_call) return 1.0 / seconds_since_last_call; else return 0.0;
}

