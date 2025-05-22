from pathlib import Path

def generate_latex_frames(filename: str, lines_per_frame: int = 50) -> str:
    filepath = Path(filename)
    if not filepath.exists():
        return f"% Erreur : le fichier {filename} n'existe pas."

    total_lines = sum(1 for _ in filepath.open(encoding='utf-8'))
    base_name = filepath.name
    title_name = base_name.replace('_', '\\_')  # <-- échappé pour le titre seulement

    latex_output = ""
    frame_count = 0

    for start in range(1, total_lines + 1, lines_per_frame):
        frame_count += 1
        end = min(start + lines_per_frame - 1, total_lines)

        latex_output += f"""\\begin{{frame}}[fragile, noframenumbering]{{{title_name} (partie {frame_count})}}
\\vspace{{-1em}}
\\inputminted[
  firstline={start},
  lastline={end},
  breaklines=true,
  fontsize=\\tiny,
  frame=lines,
  framesep=1mm
]{{c}}{{../{base_name}}}
\\end{{frame}}\n\n"""

    return latex_output

# Utilisation
if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print("Usage: python gen_latex_code.py nom_fichier [nb_lignes_par_frame] [fichier_sortie]")
    else:
        nom_fichier = sys.argv[1]
        lignes = int(sys.argv[2]) if len(sys.argv) > 2 else 50
        sortie = sys.argv[3] if len(sys.argv) > 3 else None

        output = generate_latex_frames(nom_fichier, lignes)

        if sortie:
            with open(sortie, "w", encoding="utf-8") as f:
                f.write(output)
            print(f"✅ LaTeX écrit dans {sortie}")
        else:
            print(output)