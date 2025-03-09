from PIL import Image
import sys

def convert_jpg_to_ppm(input_path, output_path):
    try:
        # Ouvrir l'image JPEG
        img = Image.open(input_path)
        
        # Convertir en mode RGB (au cas où l'image est en CMYK ou autre)
        img = img.convert("RGB")
        
        # Sauvegarder en format PPM
        img.save(output_path, format="PPM")
        
        print(f"Conversion terminée : {output_path}")
    except Exception as e:
        print(f"Erreur : {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} input.jpg output.ppm")
    else:
        convert_jpg_to_ppm(sys.argv[1], sys.argv[2])
