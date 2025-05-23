WFLAGS := -Wall
CFLAGS := -std=c99

all : test_triangulation test_moravec test_trouve_coin test_detection test_camera_calibration test_reconstruction_mult test_reconstruction test_SVD

menu : menu.c constante.o
	gcc constante.o menu.c -o menu -lncurses

test_triangulation : constante.o triangle.o manipulation_fichier.o matrice.o SVD.o camera_calibration.o reconstruction.o triangle.h test_triangulation.c 
	gcc $(CFLAGS) $(WFLAGS) -g constante.o triangle.o manipulation_fichier.o matrice.o reconstruction.o SVD.o camera_calibration.o  test_triangulation.c -lm -o test_triangulation
  
test_moravec : constante.o matrice.o manipulation_fichier.o moravec.o test_moravec.c moravec.h
	gcc $(CFLAGS) $(WFLAGS) -g constante.o manipulation_fichier.o matrice.o moravec.o test_moravec.c -lm -o test_moravec

test_trouve_coin : constante.o manipulation_fichier.o matrice.o moravec.o trouve_coin.o test_trouve_coin.c
	gcc $(CFLAGS) $(WFLAGS) -g constante.o manipulation_fichier.o matrice.o moravec.o trouve_coin.o test_trouve_coin.c -lm -o test_trouve_coin

test_detection : constante.o ransac.o detection.o manipulation_fichier.o matrice.o moravec.o appariement.o trouve_coin.o  SVD.o camera_calibration.o test_detection.c
	gcc $(CFLAGS) $(WFLAGS) -g constante.o ransac.o detection.o manipulation_fichier.o matrice.o moravec.o appariement.o trouve_coin.o  SVD.o camera_calibration.o test_detection.c -lm -o test_detection

test_detection_arch : constante.o ransac.o detection.o manipulation_fichier.o matrice.o moravec.o appariement.o trouve_coin.o  SVD.o camera_calibration.o test_detection_arch.c
	gcc $(CFLAGS) $(WFLAGS) -g constante.o ransac.o detection.o manipulation_fichier.o matrice.o moravec.o appariement.o trouve_coin.o  SVD.o camera_calibration.o test_detection_arch.c -lm -o test_detection_arch

test_SVD : constante.o manipulation_fichier.o matrice.o SVD.o
	gcc $(CFLAGS) $(WFLAGS) -g constante.o manipulation_fichier.o matrice.o SVD.o test_SVD.c -lm -o test_SVD

test_camera_calibration : constante.o manipulation_fichier.o matrice.o SVD.o camera_calibration.o test_camera_calibration.c
	gcc $(CFLAGS) $(WFLAGS) -g constante.o manipulation_fichier.o matrice.o SVD.o camera_calibration.o  test_camera_calibration.c -lm -o test_camera_calibration

test_reconstruction : constante.o manipulation_fichier.o matrice.o reconstruction.o SVD.o camera_calibration.o test_reconstruction.c
	gcc $(CFLAGS) $(WFLAGS) -g constante.o manipulation_fichier.o matrice.o reconstruction.o SVD.o camera_calibration.o  test_reconstruction.c -lm -o test_reconstruction

test_reconstruction_mult : constante.o manipulation_fichier.o matrice.o reconstruction.o SVD.o camera_calibration.o test_reconstruction_mult.c
	gcc $(CFLAGS) $(WFLAGS) -g constante.o manipulation_fichier.o matrice.o reconstruction.o SVD.o camera_calibration.o  test_reconstruction_mult.c -lm -o test_reconstruction_mult

test_ransac : constante.o ransac.o manipulation_fichier.o matrice.o test_ransac.c
	gcc $(CFLAGS) $(WFLAGS) -g constante.o ransac.o manipulation_fichier.o matrice.o test_ransac.c -lm -o test_ransac


ransac.o :  ransac.c ransac.h
	gcc $(CFLAGS) $(WFLAGS) -g -c ransac.c -lm -o ransac.o


reconstruction.o :  reconstruction.c reconstruction.h
	gcc $(CFLAGS) $(WFLAGS) -g -c reconstruction.c -lm -o reconstruction.o

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
	
detection.o : detection.c detection.h
	gcc $(CFLAGS) $(WFLAGS) -g -c detection.c -lm -o detection.o

constante.o : constante.c constante.h
	gcc $(CFLAGS) $(WFLAGS) -g -c constante.c -lm -o constante.o
clean :
	rm -f *.o
	rm -f *.txt
	rm -f test_triangulation
	rm -f test_detection
	rm -f test_moravec
	rm -f test_camera_calibration
	rm -f test_SVD
	rm -f test_reconstruction
	rm -f test_reconstruction_mult
	rm -f test_triangulation
	rm -f test_trouve_coin
