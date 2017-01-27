/* \brief Main file of an example that demonstrates how to draw thick and smooth curves in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 *
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <osg/Camera>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Node>
#include <osg/Object>
#include <osg/PrimitiveSet>
#include <osg/Program>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Transform>
#include <osg/Uniform>
#include <osgViewer/Viewer>

#include <osg/BlendFunc>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>

// user defined callbacks for uniforms
#include "Callbacks.h"
#include "Data.h"

const int OSG_WIDTH = 1280;
const int OSG_HEIGHT = 960;
const osg::Vec4f FOG_COLOR = osg::Vec4f(0.6f, 0.6f, 0.7f, 1.f);

osg::Node* createBezierScene(osg::Camera* camera)
{
    osg::Vec3Array* vertices = createBezierDataPoints();
    osg::Vec4Array* colors = createBezierColorPoints();

    // create geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->addPrimitiveSet(new osg::DrawArrays(GL_LINES_ADJACENCY_EXT, 0, vertices->size()));
    geom->setUseDisplayList(false);
    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_PER_VERTEX);

    // set attributes
    geom->setVertexAttribArray(0, vertices, osg::Array::BIND_PER_VERTEX);
    geom->setVertexAttribArray(1, colors, osg::Array::BIND_PER_VERTEX);

    // create shader
    osg::ref_ptr<osg::Program> program = new osg::Program;

    osg::ref_ptr<osg::Shader> vertShader = new osg::Shader(osg::Shader::VERTEX);
    if (!vertShader->loadShaderSourceFromFile("Shaders/bezier.vert"))
        std::cerr << "Could not read VERTEX shader from file" << std::endl;
    program->addShader(vertShader);

    osg::ref_ptr<osg::Shader> geomShader = new osg::Shader(osg::Shader::GEOMETRY);
        if (!geomShader->loadShaderSourceFromFile("Shaders/bezier.geom"))
            std::cerr << "Could not read GEOMETRY shader from file" << std::endl;
    program->addShader(geomShader);

    osg::ref_ptr<osg::Shader> fragShader = new osg::Shader(osg::Shader::FRAGMENT);
    if (!fragShader->loadShaderSourceFromFile("Shaders/bezier.frag"))
        std::cerr << "Could not read FRAGMENT shader from file" << std::endl;
    program->addShader(fragShader);

    // geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geom.get());

    osg::StateSet* state = geode->getOrCreateStateSet();
    state->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

    // add uniforms
    osg::Uniform* modelViewProjectionMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "ModelViewProjectionMatrix");
    modelViewProjectionMatrix->setUpdateCallback(new ModelViewProjectionMatrixCallback(camera));
    state->addUniform(modelViewProjectionMatrix);

    osg::Uniform* viewportVector = new osg::Uniform(osg::Uniform::FLOAT_VEC2, "Viewport");
    viewportVector->setUpdateCallback(new ViewportCallback(camera));
    state->addUniform(viewportVector);

    osg::Uniform* cameraEye = new osg::Uniform(osg::Uniform::FLOAT_VEC4, "CameraEye");
    cameraEye->setUpdateCallback(new CameraEyeCallback(camera));
    state->addUniform(cameraEye);

    float Thickness = 27.0;
    float miterLimit = 0.75;
    int segments = 30;
    bool isFogged = true;
    state->addUniform(new osg::Uniform("Thickness", Thickness));
    state->addUniform(new osg::Uniform("MiterLimit", miterLimit));
    state->addUniform(new osg::Uniform("Segments", segments));
    state->addUniform(new osg::Uniform("FogColor", FOG_COLOR));
    state->addUniform(new osg::Uniform("IsFogged", isFogged));

    // state settings
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    state->setMode(GL_BLEND, osg::StateAttribute::ON);
    state->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);

    osg::LineWidth* lw = new osg::LineWidth;
    lw->setWidth(10.f);
    state->setAttribute(lw, osg::StateAttribute::ON);
    osg::BlendFunc* blendfunc = new osg::BlendFunc();
    state->setAttributeAndModes(blendfunc, osg::StateAttribute::ON);

    return geode.release();
}

osg::Node* createPolylineScene(osg::Camera* camera)
{
    osg::Vec3Array* vertices = createPolylineDataPoints();
    osg::Vec4Array* colors = createPolylineColorPoints();

    // create geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->addPrimitiveSet(new osg::DrawArrays(GL_LINES_ADJACENCY_EXT, 0, vertices->size()));
    geom->setUseDisplayList(false);
    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_PER_VERTEX);

    geom->setVertexAttribArray(0, vertices, osg::Array::BIND_PER_VERTEX);
    geom->setVertexAttribArray(1, colors, osg::Array::BIND_PER_VERTEX);

    // create shader
    osg::ref_ptr<osg::Program> program = new osg::Program;

    osg::ref_ptr<osg::Shader> vertShader = new osg::Shader(osg::Shader::VERTEX);
    if (!vertShader->loadShaderSourceFromFile("Shaders/polyline.vert"))
        std::cerr << "Could not read VERTEX shader from file" << std::endl;
    program->addShader(vertShader);

    osg::ref_ptr<osg::Shader> geomShader = new osg::Shader(osg::Shader::GEOMETRY);
        if (!geomShader->loadShaderSourceFromFile("Shaders/polyline.geom"))
            std::cerr << "Could not read GEOMETRY shader from file" << std::endl;
    program->addShader(geomShader);

    osg::ref_ptr<osg::Shader> fragShader = new osg::Shader(osg::Shader::FRAGMENT);
    if (!fragShader->loadShaderSourceFromFile("Shaders/polyline.frag"))
        std::cerr << "Could not read FRAGMENT shader from file" << std::endl;
    program->addShader(fragShader);

    // geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geom.get());

    osg::StateSet* state = geode->getOrCreateStateSet();
    state->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

    // add uniforms
    osg::Uniform* modelViewProjectionMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "ModelViewProjectionMatrix");
    modelViewProjectionMatrix->setUpdateCallback(new ModelViewProjectionMatrixCallback(camera));
    state->addUniform(modelViewProjectionMatrix);

    osg::Uniform* viewportVector = new osg::Uniform(osg::Uniform::FLOAT_VEC2, "Viewport");
    viewportVector->setUpdateCallback(new ViewportCallback(camera));
    state->addUniform(viewportVector);

    float Thickness = 12.0;
    float miterLimit = 0.1;
    state->addUniform(new osg::Uniform("Thickness", Thickness));
    state->addUniform(new osg::Uniform("MiterLimit", miterLimit));

    // state settings
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    state->setMode(GL_BLEND, osg::StateAttribute::ON);
    state->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);

    osg::LineWidth* lw = new osg::LineWidth;
    lw->setWidth(10.f);
    state->setAttribute(lw, osg::StateAttribute::ON);
    osg::BlendFunc* blendfunc = new osg::BlendFunc();
    state->setAttributeAndModes(blendfunc, osg::StateAttribute::ON);

    return geode.release();
}

int main(int, char**)
{
#ifdef _WIN32
    ::SetProcessDPIAware(); // if Windows, for HDPI monitors
#endif // _WIN32

    osgViewer::Viewer viewer;
    osg::DisplaySettings::instance()->setNumMultiSamples(4); // multi sampling is on

    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(createBezierScene(viewer.getCamera()));
    root->addChild(createPolylineScene(viewer.getCamera()));
    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(100,100,OSG_WIDTH, OSG_HEIGHT);

    /* depth settings */
    osg::StateSet* state = root->getOrCreateStateSet();
    state->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    osg::Camera* camera = viewer.getCamera();
    camera->setClearColor(FOG_COLOR);
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    viewer.setCameraManipulator( new osgGA::TrackballManipulator);
    viewer.realize();
    while (!viewer.done()){
        camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        viewer.frame();
    }


    return 0;
}
