# Projet de Reconstruction 3D

## Description
Ce projet implémente une pipeline de reconstruction 3D à partir d’images, incluant le calcul de matrices de transformation, la calibration de caméra et la mise en correspondance de points.

Execution rapide : compiler avec make menu puis executer ./menu
---

## Structure du projet

### Fichier source

| Fichier | Description |
|---------|------------|
| `menu.c` | Gestion interactive des étapes de la reconstruction |
| `matrice.c` | Implémente les opérations sur les matrices. |
| `moravec.c` | Effectue une selction de points d'intérêts avec la méthode de Moravec|
| `trouve_coin.c` | Raffinement des points détecter par Moravec|
| `appariement.c` | calcul du descripteur brief et distance à la droite épipolaire|
| `detection.c` | Effectue la détection, le tris et l'appariement des points d'intérêts pour un couple d'image - avec moravec, brief et eipolar_distance|
| `camera_calibration.c` | Effectue la détection, le tris et l'appariement des points d'intérêts pour un couple d'image |
| `SVD.c` | Plusieurs fonctions pour calculer la décomposition SVD d'une matrice et son application à la résolution de système|
| `reconstruction.c` | Effectue la reconstruction de coordonnées 3D à partir depoints 2D|
| `triangles.c` | Détermine les triangles utilent pour reconstruir l'enveloppe de l'objet|
| `manipulation_fichier.c` | Utilitaire pour lire et ecrire des matrices dans les données|
| `constante.c` |Modification des paramètres de reconstruction|

### 📂 Fichier include

| Fichier | Description |
|---------|------------|
| `calibration.h` | Déclare les fonctions pour la calibration de la caméra. |
| `correspondance.h` | Déclare les fonctions pour l’appariement des points. |
| `epipolaire.h` | Contient les prototypes pour le calcul des droites épipolaires. |
| `rectification.h` | Déclare les fonctions pour la rectification stéréoscopique. |
| `triangulation.h` | Déclare les fonctions pour la reconstruction 3D. |
| `optimisation.h` | Contient les définitions des fonctions d’optimisation. |
| `matrice.h` | Définit les structures et opérations de base sur les matrices. |

### Fichier test

| Fichier |
|---------|
| `test_moravec.c` |  
| `test_trouve_coin.c` | 
| `test_detection.c` |  
| `test_camera_calibration.c` | 
| `test_SVD.c` | 
| `test_reconstruction.c` |
| `test_reconstruction_mult.c` |
| `test_triangulation.c` |
---

### 📂 `points/`
Contient les fichiers d’entrée et de sortie.

| Fichier | Description |
|---------|------------|
| `images/` | Dossier contenant les images utilisées pour la reconstruction. |
| `donnees/` | Résultats de la reconstruction (ex : nuages de points 3D). |

---
## Utilitaire Python 
| `save_points.py` | Selectionnées les points pour le calibrage et les sauvegardés dans un fichier texte (usage : python3 select_deux.py <img1>.jpg <img2>.jpg <c) |
| `jpg_to_txt.py` | Utilitaire pour lire les images par pixel|
| `correspondance.py` | Affichage des droites epipolaires|
| `rassemble_points.py` | |
| `plot_detect_un.py` | affiche les points du fichier arg2 sur l image arg1 |


## Compilation et Exécution

| Commande               | Description | Usage|
|------------------------|-------------|---------|
| `all`                 | Compile tous les exécutables de tests||
| `test_triangulation`  | Test de triangulation||
| `test_moravec`        | Test de la détection de moravec||
| `test_trouve_coin`    | Test la détection de coin ||
| `test_detection`    | Test l'ensemble du processus de detection et d'appariement ||
| `clean`              | Supprime tous les fichiers objets (`.o`) et les exécutables ||



A faire  : 
moravec :  descripteur Brief : 
choisis 256 dans une fenetre et les 
n BRIEF, the test pairs are preselected randomly but remain fixed for all keypoints in all images to ensure consistency and comparability of the descriptors.

## Historique 

### 25/02 : fin implem brief
1ere tentative d appariement : 
 2 critères pour la correspondance : 
    - distance de hamming (entre descripteurs brief)
    - distance à la droite epipolaire
    id 1 : knn avec hamming puis choix du candidat minimisant la distance  la droite
    id 2 : creation d un 
    id 3 : filtrer d abord avec distance à la droite puis comparer distance de hamming


### 07/03 : 
 - moravec sur img 1
 - tri prealable des points  utiles sur img2
 - moravec  sur img 2
 - pour chaque points de img1 , on cherche des points de correspondances parmis les points d interet de img2

### 16/03 : mise au propre du projet

### 26/03 : detection et appariement 
premier test resussi d appariement
teste detecction :  trouve matrice points appariés

### 6-8/05 : assemblage des differents programmes et debug