kev_caster_test: kev_caster_test.c ../kev_caster.o ../kev_test.o
	gcc ../kev_caster.o ../kev_test.o kev_caster_test.c -I "../" -o kev_caster_test

../kev_caster.o: ..//kev_caster.c ../kev_caster.h
	gcc ../kev_caster.c -c -o ../kev_caster.o

../kev_test.o: ..//kev_test.c ../kev_test.h
	gcc ../kev_test.c -c -o ../kev_test.o
