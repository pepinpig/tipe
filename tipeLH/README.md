# Projet de Reconstruction 3D

## Description
Ce projet implémente une pipeline de reconstruction 3D à partir d’images, incluant le calcul de matrices de transformation, la calibration de caméra et la mise en correspondance de points.
---

## Structure du projet

### Fichier source

| Fichier | Description |
|---------|------------|
| `main.c` | Programme principal qui orchestre la reconstruction 3D. |
| `matrice.c` | Implémente les opérations sur les matrices. |
| `save_points.py` | Selectionnées les points pour le calibrage et les sauvegardés dans un fichier texte (usage : python3 save_points.py <img1>.jpg <img2>.jpg <calibration?>) |
| `moravec.c` | Effectue une selction de points d'intérêts avec la méthode de Moravec|
| `appariement.c` | calcul du descripteur briefet  calcul de la distance à la droite épipolaire|
| `detection.c` | Effectue la détection, le tris et l'appariement des points d'intérêts pour un couple d'image - avec moravec, brief et eipolar_distance|
| `detection.c` | Effectue la détection, le tris et l'appariement des points d'intérêts pour un couple d'image |
| `SVD.c` | Plusieurs fonctions pour calculer la décomposition SVD d'une matrice et son application à la résolution de système|
| `triangles.c` | Détermine les triangles utilent pour reconstruir l'enveloppe de l'objet|
| `manipulation_fichier.c` | Utilitaire pour lire et ecrire des matrices dans les données|
| `save_points.py` | Utilitaire pour selctionner des points sur une image et enregistrer leur coordonnées|
| `jpg_to_txt.py` | Utilitaire pour lire les images par pixel|
| `correspondance.py` | Affichage des droites epipolaires|
| `rassemble_points.py` | |

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

---

### 📂 `points/`
Contient les fichiers d’entrée et de sortie.

| Fichier | Description |
|---------|------------|
| `images/` | Dossier contenant les images utilisées pour la reconstruction. |
| `donnees/` | Résultats de la reconstruction (ex : nuages de points 3D). |

---

## Compilation et Exécution

| Commande               | Description |
|------------------------|-------------|
| `all`                 | Compile tous les exécutables de tests|
| `test_triangulation`  | Test de triangulation|
| `test_moravec`        | Test de la détection de moravec|
| `test_trouve_coin`    | Test la détection de coin |
| `test_detection`    | Test l'ensemble du processus de detection et d'appariement |
| `clean`              | Supprime tous les fichiers objets (`.o`) et les exécutables |



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