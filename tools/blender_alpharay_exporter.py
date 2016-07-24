bl_info = {
    "name": "Export Alpharay Scene (.xml)",
    "author": "Ali Aafee",
    "version": (1, 0),
    "blender": (2, 7, 7),
    "api": 35622,
    "location": "File > Export",
    "description": "Export Scene to Alpharay xml file",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

'''
Usage Notes:


'''

import bpy
from bpy.props import *
import mathutils, math, struct
import os
import os.path
from os import remove
import time
import bpy_extras
from bpy_extras.io_utils import ExportHelper 
import time
import shutil
import bpy
import mathutils
import bmesh

def vecToStr(vector):
    return "{0},{1},{2}".format(vector.x, vector.y, vector.z)

def cameraXml(camera):
    position = vecToStr(camera.location);
    rotation = vecToStr(camera.rotation_euler);
    target = "0,0,0";
    fov = camera.data.angle;
    tilt = "0";
    lens = camera.data.lens;

    return '<camera position="{0}" rotation="{1}" target="{2}" fov="{3}" tilt="{4}" lens="" />'.format(position, rotation, target, fov, tilt, lens)


def lampXml(lamp):
    name = lamp.name
    position = vecToStr(lamp.location)
    color = "{0},{1},{2}".format(
                    lamp.data.color.r,
                    lamp.data.color.g,
                    lamp.data.color.b);

    if lamp.data.type == "POINT":
        return '<pointlight name="{0}" position="{1}" energy="{2}" color="{3}" />'.format(name, position, lamp.data.energy, color)
    else:
        return '<pointlight name="{0}" position="{1}" energy="{2}" color="{3}" />'.format(name, position, lamp.data.energy, color)


def uniqueFileName(folderpath, filename, extension):
    filename = filename.lower()
    filename_ext = "{0}{1}".format(filename, extension)
    filepath = os.path.join(folderpath, filename_ext)
    if os.path.isfile(filepath) or os.path.isdir(filepath):
        i = 0
        while os.path.isfile(filepath) or os.path.isdir(filepath):
            i += 1
            filename_ext = "{0}-{1}{2}".format(filename, i, extension)
            filepath = os.path.join(folderpath, filename_ext)
    return filepath

'''
def exportObjFile(obj, filepathxml):
    bpy.ops.object.mode_set(mode = 'OBJECT')
    bpy.ops.object.select_all(action='DESELECT')
    bpy.ops.object.select_pattern(pattern=obj.name)

    folderpath = os.path.dirname(filepathxml)

    filepath = uniqueFileName(folderpath, obj.name, ".obj") #os.path.join(folderpath, "{0}.obj".format(obj.name))

    bpy.ops.export_scene.obj(
        filepath=filepath, 
        axis_up='Z',
        axis_forward="Y",
        use_selection=True,
        use_mesh_modifiers=True,
        use_normals=True,
        use_uvs=True,
        use_materials=False,
        use_triangles=True)

    filename = os.path.basename(filepath)

    return '<file type="obj" filename="{0}" />'.format(filename)

def exportPlyFile(obj, filepathxml):
    bpy.ops.object.mode_set(mode = 'OBJECT')
    bpy.ops.object.select_all(action='DESELECT')
    bpy.ops.object.select_pattern(pattern=obj.name)

    folderpath = os.path.dirname(filepathxml)

    filepath = uniqueFileName(folderpath, obj.name, ".ply")
    
    bpy.ops.export_mesh.ply(
        filepath = filepath,
        check_existing=False,
        use_normals=True,
        use_uv_coords=True,
        use_colors=False)
    
    filename = os.path.basename(filepath)

    return '<file type="ply" filename="{0}" />'.format(filename)

'''


def exportWavefrontObj(obj, filepathxml):
    mesh = obj.to_mesh( scene = bpy.context.scene,
                        apply_modifiers = True,
                        settings = "RENDER",
                        calc_tessface=False)

    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()

    folderpath = os.path.dirname(filepathxml)

    filepath = uniqueFileName(folderpath, obj.name, ".obj")

    with open(filepath, "w", encoding="utf8", newline="\n") as f:
        
        hasUV = True if mesh.uv_layers.active is not None else False

        f.write("o {0}\n".format(obj.name))

        strVertices = ""
        strNormals = ""
        countUVs = 0
        strUVs = ""
        strFaces = ""

        for vertex in mesh.vertices:
            strVertices += "v {0} {1} {2}\n".format( vertex.co.x,
                                                     vertex.co.y,
                                                     vertex.co.z)
            strNormals += "vn {0} {1} {2}\n".format( vertex.normal.x,
                                                     vertex.normal.y,
                                                     vertex.normal.z)

        hasUV = True if mesh.uv_layers.active is not None else False

        for face in mesh.polygons:
            strFaceVertices = []
            for vi, li in zip(face.vertices, face.loop_indices):
                vertexIndex = mesh.vertices[vi].index + 1
                normalIndex = vertexIndex
                uvIndex = ""
                if hasUV:
                    countUVs += 1
                    uvIndex = str(countUVs)
                    uv = mesh.uv_layers.active.data[li].uv
                    strUVs += "vt {0} {1}\n".format(uv.x, uv.y)

                strFaceVertices.append("{0}/{1}/{2}".format(vertexIndex, uvIndex, normalIndex))

            strFaces += "f {0}\n".format(" ".join(strFaceVertices));

        f.write(strVertices)
        f.write(strNormals)
        f.write(strUVs)
        f.write(strFaces)

    filename = os.path.basename(filepath)

    return '<file type="obj" filename="{0}" />'.format(filename)


def meshXml(obj, filepath):
    name = obj.name
    position = vecToStr(obj.location)
    rotation = vecToStr(obj.rotation_euler)
    scale = vecToStr(obj.scale)
    
    result = '<mesh name="{0}" position="{1}" rotation="{2}" scale="{3}">\n'.format(name, position, rotation, scale)
    
    result += '\t{0}\n'.format(exportWavefrontObj(obj, filepath))

    result += '</mesh>'

    return result


def lightXml(light):
    position = vecToStr(light.position);
    
        
def do_export(context, props, filepath): 
    #apply_modifiers = props.apply_modifiers
    
    with open(filepath, "w", encoding="utf8", newline="\n") as f:
        cameras = []
        lamps = []
        meshes = []

        for obj in context.scene.objects:
            if obj.type == 'MESH':
                meshes.append(meshXml(obj, filepath))
            if obj.type == "FONT":
                meshes.append(meshXml(obj, filepath))
            if obj.type == 'LAMP':
                lamps.append(lampXml(obj))
            if obj.type == 'CAMERA':
                cameras.append(cameraXml(obj))
                

        xml  = '<?xml version="1.0" ?>\n'
        xml += '<project>\n\n'
        xml += '<scene>\n\n'
        xml += '\n'.join(cameras) + '\n\n'
        xml += '<lights>\n'
        xml += '\n'.join(lamps) + '\n'
        xml += '</lights>\n\n'
        xml += '<objects>\n'
        xml += '\n'.join(meshes) + '\n'
        xml += '</objects>\n\n'
        xml += '</scene>\n\n'
        xml += '</project>\n\n'
        
        f.write(xml)

        f.flush()
        
    return True


###### EXPORT OPERATOR #######
class Export_objc(bpy.types.Operator, ExportHelper):
    '''Exports the scene to Alpharay xml file.'''
    bl_idname = "export_scene.alpharay"
    bl_label = "Export Alpharay Scene (.xml)"

    filename_ext = ".xml"
    
    #apply_modifiers = BoolProperty(name="Apply Modifiers",
    #                        description="Applies the Modifiers",
    #                        default=True)

    #rot_x90 = True    
    #world_space = True
    
    #@classmethod
    #def poll(cls, context):
    #    return context.active_object.type in ['MESH', 'CURVE', 'SURFACE', 'FONT']

    def execute(self, context):
        start_time = time.time()
        print('\n_____START_____')
        props = self.properties
        filepath = self.filepath
        filepath = bpy.path.ensure_ext(filepath, self.filename_ext)

        exported = do_export(context, props, filepath)
        
        if exported:
            print('finished export in %s seconds' %((time.time() - start_time)))
            print(filepath)
            
        return {'FINISHED'}

    def invoke(self, context, event):
        wm = context.window_manager

        if True:
            # File selector
            wm.fileselect_add(self) # will run self.execute()
            return {'RUNNING_MODAL'}
        elif True:
            # search the enum
            wm.invoke_search_popup(self)
            return {'RUNNING_MODAL'}
        elif False:
            # Redo popup
            return wm.invoke_props_popup(self, event) #
        elif False:
            return self.execute(context)


### REGISTER ###

def menu_func(self, context):
    self.layout.operator(Export_objc.bl_idname, text="Alpharay Scene (.xml)")

def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()

