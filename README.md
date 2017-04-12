# GLSL shader for 3D Bezier curves with added fog effect (using OpenSceneGraph)

![GLSL shader for lines and curves](https://github.com/vicrucann/shader-3dcurve/blob/master/images/curve-lines.gif)

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

## Requirement

* Compiler that supports C++, e.g., GCC
* OpenSceneGraph library (>= 3.4.0)
* Graphics card supporting OpenGL (>=3.3) - make sure your drivers are updated
* CMake (>=2.8.11)

## Troubleshoot

On slightly older machines, it is possible to encounter the `Error C6033 : Hardware limitations reached, can only emit ... vertices of this size.`. In this case, modify the number of maximum emitting vertices within the shader file. 

In `Stroke.geom` file, seek for `SegmentsMax` and `max_vertices` variables, and, depending on the maximum number of vertices your hardware supports, change their values. E.g., if the number of maximum vertices is 102 on your machine (based on the error message), set: 

```
const int SegmentsMax = 24; // max_vertices = (SegmentsMax+1)*4;
// ...
layout(triangle_strip, max_vertices = 100) out;
```

## Build

Use `CMakeLists.txt` to do the build, then run `shader-3dcurve`.
