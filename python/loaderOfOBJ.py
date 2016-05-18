#classe qui est retournée aprés la lecture du fichier et qui contient toutes
#les informations sur les objects présents
class Informations:
    def __init__(self):
        self.vertices = []
        self.triangles = []
        self.triangles_uvs = []
        self.triangles_normals = []
        self.tex_coords = []
        self.normals = []
#dans un fichier obj les réferences aux sommets existent de deux maniéres différentes:
#   soit c'est le n° sommet
#   soit c'est le n° dernier ajouté
def getRealOffset(index, offset, len_total):
    return index - 1 - offset if index > 0 else len_total + index
# charge une face dans le mesh mesh
# v1 est la ligne du premier vertex, v2 du second...
# on gére les cas ou v1 contient uniquement l'indice du sommet, ou l'indice d'une 
# coordonnée uv, ou d'une normale...
def add_triangle(mesh, v1, v2, v3, offsetc, offsetu, offsetn):
    v1_infos = [int(i) if i else None for i in v1.split("/")]
    v2_infos = [int(i) if i else None for i in v2.split("/")]
    v3_infos = [int(i) if i else None for i in v3.split("/")]
    """
    if not v1_infos[0] is None:
        temp_tup = (getRealOffset(v1_infos[0], offsetc, len(mesh.vertices)), \
                    getRealOffset(v2_infos[0], offsetc, len(mesh.vertices)), \
                    getRealOffset(v3_infos[0], offsetc, len(mesh.vertices)))
        mesh.triangles.append(temp_tup)
    if len(v1_infos) > 1 and not v1_infos[1] is None:
        temp_tup = (getRealOffset(v1_infos[1], offsetu, len(mesh.text_coords)), \
                    getRealOffset(v2_infos[1], offsetu, len(mesh.text_coords)), \
                    getRealOffset(v3_infos[1], offsetu, len(mesh.text_coords)))
        mesh.triangles_uvs.append(temp_tup)
    if len(v1_infos) > 2 and not v1_infos[2] is None:
        temp_tup = (getRealOffset(v1_infos[2], offsetn, len(mesh.normals)), \
                    getRealOffset(v2_infos[2], offsetn, len(mesh.normals)), \
                    getRealOffset(v3_infos[2], offsetn, len(mesh.normals)))
        mesh.triangles_normals.append(temp_tup)
    """
    if not v1_infos[0] is None:
        temp_tup = (v1_infos[0]-1-offsetc if v1_infos[0] > 0 else len(mesh.vertices) + v1_infos[0], \
                    v2_infos[0]-1-offsetc if v2_infos[0] > 0 else len(mesh.vertices) + v2_infos[0], \
                    v3_infos[0]-1-offsetc if v3_infos[0] > 0 else len(mesh.vertices) + v3_infos[0])
        mesh.triangles.append(temp_tup)
    if len(v1_infos) > 1 and not v1_infos[1] is None:
        temp_tup = (v1_infos[1]-1-offsetu if v1_infos[1] > 0 else len(mesh.text_coords) + v1_infos[1], \
                    v2_infos[1]-1-offsetu if v2_infos[1] > 0 else len(mesh.text_coords) + v2_infos[1], \
                    v3_infos[1]-1-offsetu if v3_infos[1] > 0 else len(mesh.text_coords) + v3_infos[1])
        mesh.triangles_uvs.append(temp_tup)
    if len(v1_infos) > 2 and not v1_infos[2] is None:
        temp_tup = (v1_infos[2]-1-offsetn if v1_infos[2] > 0 else len(mesh.normals) + v1_infos[2], \
                    v2_infos[2]-1-offsetn if v2_infos[2] > 0 else len(mesh.normals) + v2_infos[2], \
                    v3_infos[2]-1-offsetn if v3_infos[2] > 0 else len(mesh.normals) + v3_infos[2])
        mesh.triangles_normals.append(temp_tup)
    
#charge un fichier .obj
def load(path):
    file = open(path)
    mesh = Informations()
    meshes = {}
    current_name = None
    offsetc, offsetu, offsetn = 0, 0, 0
    for line in file.readlines():
        tokens = line.split(" ")
        if tokens[0] == "vt":
            uvs = list(map(float, tokens[1:]))
            meshes[current_name].tex_coords.append((uvs[0], uvs[1], 0))
        elif tokens[0] == "v":
            meshes[current_name].vertices.append(tuple(map(float, tokens[1:])))
        elif tokens[0] == "vn":
            meshes[current_name].normals.append(tuple(map(float, tokens[1:])))
        elif tokens[0] == "f":
            #la boucle est là car une ligne peut déclarer plusieurs triangles
            for i in range(2, len(tokens)-1):
                add_triangle(meshes[current_name], tokens[1], tokens[i], tokens[i+1], offsetc, offsetu, offsetn)
        elif tokens[0] == "o":
            if not current_name is None:
                offsetc += len(meshes[current_name].vertices)
                offsetu += len(meshes[current_name].tex_coords)
                offsetn += len(meshes[current_name].normals)
            current_name = line[1:].lstrip().rstrip()
            meshes[current_name] = Informations()
    return meshes


 
