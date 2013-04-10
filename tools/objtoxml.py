def loadOBJ(filename):
    numVerts = 0
    verts = []
    norms = []
    faces = []
    for line in open(filename, "r"):
        vals = line.split()
        if len(vals) > 1:
            if vals[0] == "v":
                v = map(float, vals[1:4])
                verts.append(v)
            if vals[0] == "vn":
                n = map(float, vals[1:4])
                norms.append(n)
            if vals[0] == "f":
                f = []
                for val in vals[1:]:
                    ps = val.split("/")
                    pa = [0,0,0]
                    i = 0
                    for p in ps:
                        pa[i] = p
                        i += 1
                        
                    f.append(pa)
                #f = map(int, vals[1:4])
                faces.append(f)
    return verts, norms, faces



verts, norms, faces = loadOBJ("bunny-sn.obj")

print """
<?xml version="1.0" ?>
<project>
    <raytracer reflectiondepth="5" subsamplesx="1" subsamplesy="1" exposure="-1" />
    <preview width="320"  height="240" />
    <final   width="1024" height="768" />
    <scene>
        <camera position="2,13,4" target="0,0,3" fov="1" tilt="0" />
        <lights>
            <pointlight
                name="arey2" 
                position="-10,0,10" 
                intensity="100,100,100"
                shadow="true"
                target="0,0,0" 
                angle="0.628319" 
                anglefalloff="0.628319" 
                lightradius="0.1" 
                samples="30" />
            <pointlight
                name="arey3" 
                position="-10,10,10" 
                intensity="100,100,100"
                shadow="true"
                target="0,0,0" 
                angle="0.628319" 
                anglefalloff="0.628319" 
                lightradius="0.1" 
                samples="30" />
            <pointlight
                name="arey3" 
                position="10,10,5" 
                intensity="60,60,60"
                shadow="false"
                target="0,0,0" 
                angle="0.628319" 
                anglefalloff="0.628319" 
                lightradius="0.1" 
                samples="30" />

        </lights>
        <images>
            <checker2d 
                name="check" 
                color1="1.0,0.0,0.0" 
                color2="0.0,1.0,0.0" />
        </images>
        <maps>
            <map2dplane 
                name="checkmap" 
                position="0,0,0" 
                rotation="0,0,0" 
                scale="1.1,1.1,1.1" 
                imagescale="1,1" 
                image="check" />
        </maps>
        <materials>
            <material 
                name="floomat" 
                diffusecolor="0,0,1" 
                highlightcolor="1,1,1" 
                reflectivity="0.0" 
                dielectric="false" 
                scatterfactor="0.0" 
                scattersamples="1" 
                ka="0.0" 
                kd="0.8" 
                ks="1.0" 
                alpha="15" 
                diffusemap="checkmap" 
                normalmap="" />

            <material 
                name="meshmat" 
                diffusecolor="0,0,1" 
                highlightcolor="1,1,1" 
                reflectivity="0.0" 
                dielectric="false" 
                scatterfactor="0.0" 
                scattersamples="1" 
                ka="0.0" 
                kd="0.8" 
                ks="1.0" 
                alpha="15" 
                diffusemap="" 
                normalmap="" />
        </materials>
        <objects>
            <plane
                name="floor" 
                position="0,0,-0.9" 
                rotation="0,0,0" 
                scale="1,1,1" 
                material="floomat" />

"""



print '<mesh name="cube" position="0,0,-2.6" rotation="0,0,0" scale="1,1,1" material="meshmat">'
print '<verts>'
scale = 50.0
i = 0
for vert in verts:
    print '<vert i="{0}" p="{1}, {3}, {2}" n="{4}, {6}, {5}" />'.format(i+1, vert[0] * scale, vert[1] * scale, vert[2] * scale, norms[i][0], norms[i][1], norms[i][2] );
    i += 1
print '</verts>'

print '<uvps> <uvp i="1" p="0,0" /> </uvps>'

i = 0
print "<trigs>"
for face in faces:
    i += 1
    print '<trig i="{0}" n="0,0,0" v0="{1}" v1="{2}" v2="{3}" m0="1" m1="1" m2="1" />'.format(i, face[0][0], face[1][0], face[2][0]);

print "</trigs>"

print "</mesh>"


print """
    </objects>

    </scene>
</project>
"""


