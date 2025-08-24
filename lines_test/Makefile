CC = gcc -std=c11
ifeq ($(OS),Windows_NT)
	libs = -lcomctl32 -lgdi32 -lm -Wl,--subsystem,windows
	targ = multi.exe
else
	libs = -lX11 -lm
	targ = multi
endif

lines_test: lines_test.c ../kev_winter.o ../kev_render.o ../kev_perf_timer.o
	gcc ../kev_winter.o ../kev_render.o ../kev_perf_timer.o lines_test.c -I "../" -o lines_test $(libs)

../kev_winter.o: ../kev_winter.c ../kev_winter.h
	$(CC) ../kev_winter.c -c -o ../kev_winter.o

../kev_render.o: ..//kev_render.c ../kev_render.h
	gcc ../kev_render.c -c -o ../kev_render.o

../kev_perf_timer.o: ../kev_perf_timer.c ../kev_perf_timer.h
	$(CC) ../kev_perf_timer.c -c -o ../kev_perf_timer.o