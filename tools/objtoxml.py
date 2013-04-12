'''
scale = 50.0
filename = "../examples/bunny-sn.obj"
trans = (0.0, 0.0, -1.8)
'''

scale = 1.0
filename = "../examples/sphere.obj"
trans = (0.0, 0.0, 0.0)


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
                        if p == "":
                            p = "0"
                        pa[i] = int(p)
                        i += 1
                        
                    f.append(pa)
                faces.append(f)
    return verts, norms, faces

verts, norms, faces = loadOBJ(filename)

#print "verts {0}, norms {1}, faces {2}".format(len(verts), len(norms), len(faces))
#exit()

print '<?xml version="1.0" ?>'
print '<mesh>'
print ' <verts>'
i = 0
for vert in verts:
    print '  <vert i="{0}" p="{1}, {3}, {2}" n="{4}, {6}, {5}" />'.format(
            i+1, 
            (float(vert[0]) * scale) + trans[0], (float(vert[1]) * scale) + trans[2], (float(vert[2]) * scale) + trans[1], 
            norms[i][0], norms[i][1], norms[i][2] );
    i += 1
print ' </verts>'

print ' <uvps>'
print '  <uvp i="0" p="0,0" />'
print ' </uvps>'

i = 0
print " <trigs>"
for face in faces:
    i += 1
    nx = (float(norms[face[0][0]-1][0]) + float(norms[face[1][0]-1][0]) + float(norms[face[2][0]-1][0])) / 3.0
    ny = (float(norms[face[0][0]-1][2]) + float(norms[face[1][0]-1][2]) + float(norms[face[2][0]-1][2])) / 3.0
    nz = (float(norms[face[0][0]-1][1]) + float(norms[face[1][0]-1][1]) + float(norms[face[2][0]-1][1])) / 3.0

    print '  <trig i="{0}" n="{7},{8},{9}" v0="{1}" v1="{2}" v2="{3}" m0="{4}" m1="{5}" m2="{6}" />'.format(
                        i, 
                        face[0][0], face[1][0], face[2][0],
                        face[0][1], face[1][1], face[2][1],
                        nx, ny, nz
                        );

print " </trigs>"

print "</mesh>"


