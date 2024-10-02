kev_img_test: kev_img_test.c ../kev_img.o ../kev_test.o
	gcc ../kev_img.o ../kev_test.o kev_img_test.c -I "../" -o kev_img_test

../kev_img.o: ..//kev_img.c ../kev_img.h
	gcc ../kev_img.c -c -o ../kev_img.o

../kev_test.o: ..//kev_test.c ../kev_test.h
	gcc ../kev_test.c -c -o ../kev_test.o
