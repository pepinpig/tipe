import bpy
import bmesh
from math import radians

# Nettoyer la scène
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)

# Fonction pour créer un matériau coloré
def create_material(name, color):
    mat = bpy.data.materials.new(name)
    mat.diffuse_color = (*color, 1.0)
    return mat

# Créer le cube
bpy.ops.mesh.primitive_cube_add(size=2, location=(0, 0, 0))
cube = bpy.context.object
cube.name = "ColoredCube"

# Ajouter 6 matériaux et assigner à chaque face
colors_cube = [(1,0,0), (0,1,0), (0,0,1), (1,1,0), (1,0,1), (0,1,1)]
for i, color in enumerate(colors_cube):
    mat = create_material(f"CubeMat{i}", color)
    cube.data.materials.append(mat)

bpy.ops.object.mode_set(mode='EDIT')
bm = bmesh.from_edit_mesh(cube.data)
for i, face in enumerate(bm.faces):
    face.material_index = i
bmesh.update_edit_mesh(cube.data)
bpy.ops.object.mode_set(mode='OBJECT')

# Activer add-on extra objects (si non activé)
bpy.ops.preferences.addon_enable(module="add_mesh_extra_objects")

# Ajouter dodécaèdre
bpy.ops.mesh.primitive_regular_solid_add(solid_type='Dodecahedron', radius=1)
dodeca = bpy.context.object
dodeca.name = "ColoredDodecahedron"
dodeca.location = (0, 0, 1.5)

# Ajouter 12 matériaux et assigner à chaque face
colors_dodeca = [
    (1,0.5,0), (0.5,1,0), (0,1,0.5), (0,0.5,1),
    (0.5,0,1), (1,0,0.5), (0.5,0.5,0), (0,0.5,0.5),
    (0.5,0,0.5), (0.8,0.3,0), (0.3,0.8,0), (0.8,0,0.3)
]
for i, color in enumerate(colors_dodeca):
    mat = create_material(f"DodecaMat{i}", color)
    dodeca.data.materials.append(mat)

bpy.ops.object.mode_set(mode='EDIT')
bm = bmesh.from_edit_mesh(dodeca.data)
for i, face in enumerate(bm.faces):
    face.material_index = i
bmesh.update_edit_mesh(dodeca.data)
bpy.ops.object.mode_set(mode='OBJECT')

# Ajouter lumière
bpy.ops.object.light_add(type='AREA', location=(5, -5, 5))
light = bpy.context.object
light.data.energy = 1000

# Ajouter caméra
bpy.ops.object.camera_add(location=(6, -6, 4), rotation=(radians(65), 0, radians(45)))
bpy.context.scene.camera = bpy.context.object