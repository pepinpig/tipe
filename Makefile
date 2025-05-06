WFLAGS := -Wall
CFLAGS := -std=c99

all : test_triangulation test_moravec test_trouve_coin test_detection


test_triangulation : triangle.o triangle.h test_triangulation.c
	gcc $(CFLAGS) $(WFLAGS) -g triangle.o test_triangulation.c -lm -o test_triangulation
  
test_moravec : manipulation_fichier.o matrice.o moravec.o test_moravec.c
	gcc $(CFLAGS) $(WFLAGS) -g manipulation_fichier.o matrice.o moravec.o test_moravec.c -lm -o test_moravec

test_trouve_coin : manipulation_fichier.o matrice.o moravec.o trouve_coin.o test_trouve_coin.c
	gcc $(CFLAGS) $(WFLAGS) -g manipulation_fichier.o matrice.o moravec.o trouve_coin.o test_trouve_coin.c -lm -o test_trouve_coin

test_detection : manipulation_fichier.o matrice.o moravec.o appariement.o trouve_coin.o  SVD.o camera_calibration.o test_detection.c
	gcc $(CFLAGS) $(WFLAGS) -g manipulation_fichier.o matrice.o moravec.o appariement.o trouve_coin.o  SVD.o camera_calibration.o test_detection.c -lm -o test_detection

test_SVD : manipulation_fichier.o matrice.o SVD.o
	gcc $(CFLAGS) $(WFLAGS) -g manipulation_fichier.o matrice.o SVD.o test_SVD.c -lm -o test_SVD


test_camera_calibration : manipulation_fichier.o matrice.o SVD.o camera_calibration.o test_camera_calibration.c
	gcc $(CFLAGS) $(WFLAGS) -g manipulation_fichier.o matrice.o SVD.o camera_calibration.o  test_camera_calibration.c -lm -o test_camera_calibration

test_reconstruction : manipulation_fichier.o matrice.o SVD.o camera_calibration.o test_reconstruction.c
	gcc $(CFLAGS) $(WFLAGS) -g manipulation_fichier.o matrice.o SVD.o camera_calibration.o  test_reconstruction.c -lm -o test_reconstruction


moravec.o :  moravec.c moravec.h
	gcc $(CFLAGS) $(WFLAGS) -g -c moravec.c -lm -o moravec.o

triangle.o : triangle.h triangle.c
	gcc $(CFLAGS) $(WFLAGS) -g -c triangle.c -lm -o triangle.o

manipulation_fichier.o : manipulation_fichier.h manipulation_fichier.c
	gcc $(CFLAGS) $(WFLAGS) -g -c manipulation_fichier.c -lm -o manipulation_fichier.o

matrice.o : matrice.c matrice.h
	gcc $(CFLAGS) $(WFLAGS) -g -c matrice.c -lm -o matrice.o

SVD.o : SVD.c SVD.h
	gcc $(CFLAGS) $(WFLAGS) -g -c SVD.c -lm -o SVD.o

trouve_coin.o : trouve_coin.c trouve_coin.h
	gcc $(CFLAGS) $(WFLAGS) -g -c trouve_coin.c -lm -o trouve_coin.o

clean :
	rm -f *.o
	rm -f test_triangulation
	rm -f test_trouve_coin
	rm -f test-moravec
