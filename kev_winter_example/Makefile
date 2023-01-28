CC = gcc
ifeq ($(OS),Windows_NT)
	libs = -lcomctl32 -lgdi32 -lm -Wl,--subsystem,windows
	targ = multi.exe
else
	libs = -lX11 -lm
	targ = multi
endif

kev_winter_example: kev_winter_example.c ../kev_winter.o
	$(CC) ../kev_winter.o kev_winter_example.c -I "../" -o kev_winter_example $(libs)

../kev_winter.o: ../kev_winter.c ../kev_winter.h
	$(CC) ../kev_winter.c -c -o ../kev_winter.o
