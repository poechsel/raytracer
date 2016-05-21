import json
import bpy
def blenderMatrixToStr(matrix):
    return [[matrix[i][j] for j in range(4)] for i in range(4)]
def blenderToggleSelection(value, objects):
    for o in objects:
        o.select = value
class Camera:
    def __init__(self):
        self.matrix = None
        self.ratio = None
        self.fov = None
    def json(self):
        return {"matrix": blenderMatrixToStr(self.matrix), \
                "fov" : self.fov, \
                "ratio": self.ratio}
        
class Scene:
    def __init__(self):
        self.width = None
        self.height = None
    def json(self):
        return {"width": self.width, \
                "height": self.height}
class Objects:
    def __init__(self):
        self.path = None
    def json(self):
        return {"path": self.path}



bl_info = {
    "name": "Json exporter",
    "author": "Pierre Oechsel",
    "version": (1, 0, 0),
    "blender": (2, 58, 0),
    "location": "File > Export",
    "description": "Export blender scene to my custom raytracer "
                   "materials and textures",
    "category": "Export"}

import bpy
from bpy_extras.io_utils import ( ExportHelper,
                                 path_reference_mode
                                 )

from bpy.props import (BoolProperty,
                       FloatProperty,
                       StringProperty,
                       EnumProperty,
                       )

class Exporter(bpy.types.Operator, ExportHelper):
    """Load a Wavefront OBJ File"""
    bl_idname = "scene.json"
    bl_label = "export to json"
    bl_options = {'PRESET', 'UNDO'}

    filename_ext = ".json"
    filter_glob = StringProperty(
            default="*.json",
            options={'HIDDEN'},
            )

    check_extension = True

    def execute(self, context):        
        EXPORT = {}
        path = self.properties.filepath
        print(path)


        ###the camera
        camera = bpy.context.scene.camera
        export_camera = Camera()
        export_camera.matrix = camera.matrix_world
        export_camera.fov = camera.data.angle
        export_camera.ratio = bpy.context.scene.render.resolution_x / bpy.context.scene.render.resolution_y
        EXPORT["CAMERA"] = export_camera.json()

        ###scene presets
        export_scene = Scene()
        export_scene.width = bpy.context.scene.render.resolution_x
        export_scene.height = bpy.context.scene.render.resolution_y
        EXPORT["SCENE_PRESETS"] = export_scene.json()


        ###the objects (we assume objects to be in the first layer)
        for ob in bpy.data.objects:
            print (ob.type, ob.layers[0], ob.layers[1])
        objects = [o for o in bpy.data.objects if o.type == "MESH" and o.layers[0]]
        print (objects)
        print(bpy.context.selected_objects)
        temp = bpy.context.selected_objects[:]
        blenderToggleSelection(False, bpy.context.selected_objects)
        blenderToggleSelection(True, objects)
        export_objects = Objects()
        export_objects.path = ".".join(path.split(".")[:-1]) + "_obj.obj"
        bpy.ops.export_scene.obj(filepath = export_objects.path, \
                                use_selection = True, \
                                use_triangles = True, \
                                use_edges = False, \
                                use_materials = False, \
                                check_existing = False, \
                                axis_forward = 'Y',
                                axis_up = 'Z')   
        EXPORT["OBJECTS"] = export_objects.json()
        blenderToggleSelection(False, bpy.context.selected_objects)
        blenderToggleSelection(True, temp)


        ###now save everything to json
        file = open(path, "w+")
        json.dump(EXPORT, file, indent = 4)
        file.close()
        print(EXPORT)

        return {"FINISHED"}


def menu_func_export(self, context):
    self.layout.operator(Exporter.bl_idname, text="Custom (.json)")



def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()
