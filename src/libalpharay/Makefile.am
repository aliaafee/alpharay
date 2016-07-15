ACLOCAL_AMFLAGS = -I m4
noinst_LTLIBRARIES = libalpharay.la

libalpharay_la_SOURCES = vector.cc \
                            matrix4.cc \
                            xmlobject.cc \
					        xmlobject-named.cc \
                            map.cc \
					        sky-map.cc \
                            material.cc \
					        image.cc \
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

libalpharay_la_CXXFLAGS = -std=c++11 -Wc++11-extensions 
libalpharay_la_LIBDADD = -ltinyxml 
libalpharay_la_LIBDADD += -lboost_thread-mt -lboost_system-mt
