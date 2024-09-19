CC = gcc -std=c11
ifeq ($(OS),Windows_NT)
	libs = -lcomctl32 -lgdi32 -lm -Wl,--subsystem,windows
	targ = multi.exe
else
	libs = -lX11 -lm
	targ = multi
endif

kev_render_test: kev_render_test.c ../kev_test.o ../kev_render.o ../kev_winter.o
	$(CC) ../kev_test.o ../kev_render.o ../kev_winter.o kev_render_test.c -I "../" -o kev_render_test $(libs)

../kev_test.o: ../kev_test.c ../kev_test.h
	$(CC) ../kev_test.c -c -o ../kev_test.o

../kev_render.o: ../kev_render.c ../kev_render.h
	$(CC) ../kev_render.c -c -o ../kev_render.o
	

../kev_winter.o: ../kev_winter.c ../kev_winter.h
	$(CC) ../kev_winter.c -c -o ../kev_winter.o

