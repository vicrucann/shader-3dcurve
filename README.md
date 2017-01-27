# GLSL shader for 3D Bezier curves with added fog effect (using OpenSceneGraph)

This is an example project that demonstrates how to draw thick and smooth lines / curves in 3D. [It is known](http://vicrucann.github.io/tutorials/osg-shader-3dlines/) that the native `GL_LINE_STRIP_ADJACENCY` creates gaps when drawing a polyline. Another restriction of the above mode is that thickness cannot surpass certain threshold (e.g., `10.f`, depending on the machine). This code demonstrates how a GLSL shaders can help to solve the above problems. 

It is possible to draw two types of geometries with the provided shaders:

1. Polylines
2. Bezier curves

Both of the geometries can be placed in a 3D space with any line thickness. 

As a bonus, I added a simplest fog effect inside the fragment shader for a Bezier curve. 

## 3D polylines

The polyline shaders can be found as `Shaders/polyline.*` files. The vertex and fragment shaders are simple pass-through, while the geometry shader provides the functionality to turn the input geometry into a thich triangular strip which is always turned towards the screen.

## 3D Bezier curves

Refer to the `Shaders/bezier.*` set of files. The main principle is the same as for the polyline shaders. The vertex shader is a pass-through. The fragment shader demostrates a simple fog effect on the geometry, and the geometrical shader treats the input geometry as Bezier control points, breaks the input curve into the provided number of segments and draws each segment in the same manner as a polyline segment. I.e., it emits a rectangular strip which is always turned towards the camera.


