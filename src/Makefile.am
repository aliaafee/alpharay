
bin_PROGRAMS = alpharay

alpharay_SOURCES =   main.cc \
                     vector.cc \
                     matrix4.cc \
                     xmlobject.cc \
                     map.cc \
                     material.cc \
                     bitmap.cc \
					 object2d.cc \
					 canvas.cc \
                     ray.cc \
                     camera.cc \
                     light.cc \
                     spot-light.cc \
                     area-light.cc \
                     sun-light.cc \
                     sky-light.cc \
                     triangle.cc \
                     object.cc \
                     sphere.cc \
					 cone.cc \
					 group.cc \
                     scene.cc \
                     plane.cc \
                     mesh.cc \
                     renderer.cc \
                     raytracer.cc \
                     pathtracer.cc \
                     project.cc \
                     bbox.cc \
                     octree.cc

alpharay_LDFLAGS =  -ltinyxml

if THREADING
    alpharay_LDFLAGS += -lboost_thread-mt -lboost_system-mt
endif

if OPENGL
    alpharay_SOURCES += gl-image.cc
    alpharay_LDFLAGS += -lgl -lglut -lX11 -lglu
endif

alpharay_LDADD = 

