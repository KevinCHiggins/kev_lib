kev_matrix_test: kev_matrix_test.c ../kev_matrix.o ../kev_test.o
	gcc ../kev_matrix.o ../kev_test.o kev_matrix_test.c -I "../" -o kev_matrix_test

../kev_matrix.o: ..//kev_matrix.c ../kev_matrix.h
	gcc ../kev_matrix.c -c -o ../kev_matrix.o

../kev_test.o: ..//kev_test.c ../kev_test.h
	gcc ../kev_test.c -c -o ../kev_test.o
