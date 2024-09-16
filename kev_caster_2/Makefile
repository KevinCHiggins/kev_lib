CC = gcc -std=c11
ifeq ($(OS),Windows_NT)
	libs = -lcomctl32 -lgdi32 -lm -Wl,--subsystem,windows
	targ = multi.exe
else
	libs = -lX11 -lm
	targ = multi
endif

kev_caster_2: kev_caster_2.c ../kev_winter.o ../kev_perf_timer.o ../kev_render.o
	$(CC) ../kev_winter.o ../kev_perf_timer.o ../kev_render.o kev_caster_2.c -I "../" -o kev_caster_2 $(libs)

../kev_winter.o: ../kev_winter.c ../kev_winter.h
	$(CC) ../kev_winter.c -c -o ../kev_winter.o

../kev_render.o: ../kev_render.c ../kev_render.h
	$(CC) ../kev_render.c -c -o ../kev_render.o

../kev_perf_timer.o: ../kev_perf_timer.c ../kev_perf_timer.h
	$(CC) ../kev_perf_timer.c -c -o ../kev_perf_timer.o
