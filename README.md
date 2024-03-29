# alpharay

A raytracer, using an XML scene description language, and optional wxWidgets UI.


## Basic Installation

alpharay uses the standard procedure for compilation and installation:

    ./configure
    make
    make install

To generate a "./configure" file run "autoreconf", this requires
autoconf and automake.


## Required Dependencies

tinyxml is the only required dependency.

Install it in the standard manner for your system or get it from:
	
	http://sourceforge.net/projects/tinyxml/
	

## Optional Dependencies

The following optional dependencies are supported:
* CImg for handling more image file types, natively supports portable bitmap(pmb) format.
* boost for multi threading
* wxWidgets for UI


## Usage

To render the project examples/hello.xml and save the render result
to hello.pmb:

    alpharay -o hello.pmb examples/hello.xml
