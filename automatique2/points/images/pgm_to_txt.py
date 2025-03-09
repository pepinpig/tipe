def pgm_to_txt(input_pgm, output_txt):
    try:
        with open(input_pgm, "rb") as f:
            # Lire l'en-tête
            magic_number = f.readline().strip()  # "P5"
            dimensions = f.readline().strip()  # Largeur et hauteur
            max_val = f.readline().strip()  # Valeur max (généralement 255)

            width, height = map(int, dimensions.split())

            # Lire les données binaires des pixels
            pixel_data = f.read()

        # Convertir les pixels en texte
        with open(output_txt, "w") as f:
            for i in range(height):
                row = pixel_data[i * width:(i + 1) * width]  # Extraire la ligne
                f.write(" ".join(map(str, row)) + "\n")  # Écrire la ligne en texte

        print(f"Conversion terminée : {output_txt}")

    except Exception as e:
        print(f"Erreur : {e}")

# Exemple d'utilisation
if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} input.pgm output.txt")
    else:
        pgm_to_txt(sys.argv[1], sys.argv[2])
