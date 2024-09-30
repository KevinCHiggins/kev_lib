#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "kev_perf_timer.h"
#include "kev_test.h"

void test_first_call_zero_s();
void test_first_call_zero_ns();
void test_first_call_zero_rate();
void test_second_call_nonzero_s();
void test_second_call_nonzero_ns();
void test_second_call_nonzero_rate();
void test_longer_sleep_causes_longer_s();
void test_longer_sleep_causes_longer_ns();
void test_longer_sleep_causes_lower_rate();

int main(int argc, char *argv)
{
    kev_perf_init();
    kev_test_run("First call returns zero (seconds).", test_first_call_zero_s);

    kev_test_run("First call returns zero (nanoseconds).", test_first_call_zero_ns);

    kev_test_run("First call returns zero (rate).", test_first_call_zero_rate);

    kev_test_run("Second call returns non-zero (seconds).", test_second_call_nonzero_s);

    kev_test_run("Second call returns non-zero (nanoseconds).", test_second_call_nonzero_ns);

    kev_test_run("Second call returns non-zero (rate).", test_second_call_nonzero_rate);

    kev_test_run("Longer sleep lengthens time between calls (seconds).", test_longer_sleep_causes_longer_s);

    kev_test_run("Longer sleep lengthens time between calls (nanoseconds).", test_longer_sleep_causes_longer_ns);

    kev_test_run("Longer sleep lowers rate.", test_longer_sleep_causes_lower_rate);


    puts(kev_test_get_report());

    return 0;
}

void test_first_call_zero_s()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	kev_test_assert_double_equal(kev_perf_time_since_last_call_ns(&timing), 0.0);
	
}

void test_first_call_zero_ns()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	kev_test_assert_int_equal(kev_perf_time_since_last_call_s(&timing), 0.0);
	
}
void test_first_call_zero_rate()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	kev_test_assert_double_equal(kev_perf_rate_since_last_call(&timing), 0.0);
	
}
void test_second_call_nonzero_s()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	kev_perf_time_since_last_call_s(&timing);	
	usleep(50); // sleep necessary on Windows due to low perf counter frequency (10000000 on my system)
	double elapsed = kev_perf_time_since_last_call_s(&timing);
	kev_test_assert_double_not_equal(elapsed, 0.0);

	
}
void test_second_call_nonzero_ns()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	kev_perf_time_since_last_call_ns(&timing);
	usleep(50);
	int64_t elapsed = kev_perf_time_since_last_call_ns(&timing);
	kev_test_assert_int_not_equal(elapsed, 0);

}

void test_second_call_nonzero_rate()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	kev_perf_rate_since_last_call(&timing);
	usleep(50);
	double rate = kev_perf_rate_since_last_call(&timing);
	kev_test_assert_double_not_equal(rate, 0.0);
	
}
void test_longer_sleep_causes_longer_s()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	for (int i = 0; i < 100; i++)
	{
		kev_perf_time_since_last_call_s(&timing); // in here so we don't worry about setup time of for loop
		usleep(50);
		double elapsed = kev_perf_time_since_last_call_s(&timing);
		usleep(2000); // Large difference between sleeps necessary for difference to reliably register on Windows
		double elapsed_after_sleep = kev_perf_time_since_last_call_s(&timing);
		kev_test_assert_true(elapsed_after_sleep > elapsed);
	}
}
void test_longer_sleep_causes_longer_ns()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	for (int i = 0; i < 100; i++)
	{
		kev_perf_time_since_last_call_ns(&timing); // in here so we don't worry about setup time of for loop
		usleep(50);
		int64_t elapsed = kev_perf_time_since_last_call_ns(&timing);
		usleep(2000);
		int64_t elapsed_after_sleep = kev_perf_time_since_last_call_ns(&timing);
		kev_test_assert_true(elapsed_after_sleep > elapsed);
	}
}
void test_longer_sleep_causes_lower_rate()
{
	kev_perf_timing timing;
	memset(&timing, 0, sizeof(kev_perf_timing));
	for (int i = 0; i < 100; i++)
	{
		kev_perf_time_since_last_call_s(&timing); // in here so we don't worry about setup time of for loop
		usleep(50);
		double rate = kev_perf_rate_since_last_call(&timing);
		usleep(2000);
		double rate_after_sleep = kev_perf_rate_since_last_call(&timing);
		kev_test_assert_true(rate_after_sleep < rate);
	}
}

