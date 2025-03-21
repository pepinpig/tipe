all : test_triangulation test_moravec test_trouve_coin 


test_triangulation : triangle.o triangle.h test_triangulation.c
	gcc -Wall -g triangle.o test_triangulation.c -lm -o test_triangulation
  
test_moravec : manipulation_fichier.o matrice.o moravec.o test_moravec.c
	gcc -Wall -g manipulation_fichier.o matrice.o moravec.o test_moravec.c -lm -o test_moravec

test_trouve_coin : manipulation_fichier.o matrice.o moravec.o trouve_coin.o test_trouve_coin.c
	gcc -Wall -g manipulation_fichier.o matrice.o moravec.o trouve_coin.o test_trouve_coin.c -lm -o test_trouve_coin

test_detection : manipulation_fichier.o matrice.o moravec.o appariement.o camera_calibration.o test_detection.c
	gcc -Wall -g manipulation_fichier.o matrice.o moravec.o appariement.o camera_calibration.o test_detection.c -lm -o test_detection

test_camera_calibration : manipulation_fichier.o matrice.o camera_calibration.o test_detection.c
	gcc -Wall -g manipulation_fichier.o matrice.o camera_calibration.o test_detection.c -lm -o test_detection

appariement.o : appariement.c appariement.h
	gcc -Wall -g -c appariement.c -lm -o appariement.o

moravec.o :  moravec.c moravec.h
	gcc -Wall -g -c moravec.c -lm -o moravec.o

triangle.o : triangle.h triangle.c
	gcc -Wall -g -c triangle.c -lm -o triangle.o

manipulation_fichier.o : manipulation_fichier.h manipulation_fichier.c
	gcc -Wall -g -c manipulation_fichier.c -lm -o manipulation_fichier.o

matrice.o : matrice.c matrice.h
	gcc -Wall -g -c matrice.c -lm -o matrice.o

SVD.o : SVD.c SVD.h
	gcc -Wall -g -c SVD.c -lm -o SVD.o

camera_calibration.o :camera_calibration.c camera_calibration.h
	gcc -Wall -g -c camera_calibration.c -lm -o camera_calibration.o


trouve_coin.o : trouve_coin.c trouve_coin.h
	gcc -Wall -g -c trouve_coin.c -lm -o trouve_coin.o

clean :
	rm -f *.o
	rm -f test_triangulation
	rm -f test_trouve_coin
	rm -f test-moravec



