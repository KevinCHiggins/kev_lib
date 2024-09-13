kev_test_perf_timer: kev_perf_timer_test.c ../kev_perf_timer.o ../kev_test.o
	gcc kev_perf_timer_test.c ../kev_test.o ../kev_perf_timer.o -I "../" -o kev_perf_timer_test

../kev_perf_timer.o: ../kev_perf_timer.c ../kev_perf_timer.h
	gcc ../kev_perf_timer.c -c -o ../kev_perf_timer.o

../kev_test.o: ../kev_test.c ../kev_test.h
	gcc ../kev_test.c -c -o ../kev_test.o

