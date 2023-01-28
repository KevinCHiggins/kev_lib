kev_dict_test: kev_dict_test.c ../kev_dict.o ../kev_test.o
	gcc ../kev_dict.o ../kev_test.o kev_dict_test.c -I "../" -o kev_dict_test

../kev_dict.o: ..//kev_dict.c ../kev_dict.h
	gcc ../kev_dict.c -c -o ../kev_dict.o

../kev_test.o: ..//kev_test.c ../kev_test.h
	gcc ../kev_test.c -c -o ../kev_test.o
