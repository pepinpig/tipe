from PIL import Image
import sys

def convert_jpg_to_pgm(input_path, output_path):
    try:
        # Ouvrir l'image JPEG
        img = Image.open(input_path)
        
        # Convertir en niveaux de gris
        img = img.convert("L")  # "L" signifie Grayscale (0-255)
        
        # Sauvegarder en format PGM (P5, binaire)
        img.save(output_path, format="PPM")

        # Modifier le fichier pour convertir PPM → PGM (P5)
        with open(output_path, "rb") as f:
            data = f.read()

        # Modifier l'en-tête pour passer de P6 (PPM) à P5 (PGM)
        data = data.replace(b"P6", b"P5", 1)

        # Réécrire le fichier avec l'en-tête correct
        with open(output_path, "wb") as f:
            f.write(data)

        print(f"Conversion terminée : {output_path}")

    except Exception as e:
        print(f"Erreur : {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} input.jpg output.pgm")
    else:
        convert_jpg_to_pgm(sys.argv[1], sys.argv[2])
