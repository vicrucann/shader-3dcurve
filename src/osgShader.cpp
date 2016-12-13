#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <osg/Camera>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osg/PrimitiveSet>
#include <osg/Program>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Transform>
#include <osg/Uniform>
#include <osgViewer/Viewer>

#include <osg/BlendFunc>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>

struct ModelViewProjectionMatrixCallback: public osg::Uniform::Callback
{
    ModelViewProjectionMatrixCallback(osg::Camera* camera) :
            _camera(camera) {
    }

    virtual void operator()(osg::Uniform* uniform, osg::NodeVisitor* nv) {
        osg::Matrixd viewMatrix = _camera->getViewMatrix();
        osg::Matrixd modelMatrix = osg::computeLocalToWorld(nv->getNodePath());
        osg::Matrixd modelViewProjectionMatrix = modelMatrix * viewMatrix * _camera->getProjectionMatrix();
        uniform->set(modelViewProjectionMatrix);
    }

    osg::Camera* _camera;
};

struct CameraEyeCallback: public osg::Uniform::Callback
{
    CameraEyeCallback(osg::Camera* camera) :
            _camera(camera) {
    }

    virtual void operator()(osg::Uniform* uniform, osg::NodeVisitor* /*nv*/) {
        osg::Vec3f eye, center, up;
        _camera->getViewMatrixAsLookAt(eye, center, up);
        osg::Vec4f eye_vec = osg::Vec4f(eye.x(), eye.y(), eye.z(), 1);
        uniform->set(eye_vec);
    }
    osg::Camera* _camera;
};

const int OSG_WIDTH = 1280;
const int OSG_HEIGHT = 960;
const osg::Vec4f FOG_COLOR = osg::Vec4f(0.6f, 0.6f, 0.7f, 1.f);

osg::Vec3Array* createDataPoints()
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

    vertices->push_back(osg::Vec3f(0,0,0));
    vertices->push_back(osg::Vec3f(0,0,1));
    vertices->push_back(osg::Vec3f(1,0,1));
    vertices->push_back(osg::Vec3f(1,0,0));

    vertices->push_back(osg::Vec3f(1,1,0));
    vertices->push_back(osg::Vec3f(0.5,5,0));

    return vertices.release();
}

osg::Vec4Array* createColorPoints()
{
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

    colors->push_back(osg::Vec4f(0.1, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.2, 0.1, 0.9, 1));
    colors->push_back(osg::Vec4f(0.7, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));

    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.9, 0.1, 1));

    return colors.release();
}

osg::Node* createBezierScene(osg::Camera* camera)
{
    osg::Vec3Array* vertices = createDataPoints();
    osg::Vec4Array* colors = createColorPoints();

    // create geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, vertices->size()));
    geom->setUseDisplayList(false);

    // defaults
    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_PER_VERTEX);

    // set attributes
    geom->setVertexAttribArray(0, vertices, osg::Array::BIND_PER_VERTEX);
    geom->setVertexAttribArray(1, colors, osg::Array::BIND_PER_VERTEX);

    // create shader
    osg::ref_ptr<osg::Program> program = new osg::Program;

    osg::ref_ptr<osg::Shader> vertShader = new osg::Shader(osg::Shader::VERTEX);
    if (!vertShader->loadShaderSourceFromFile("Shaders/polygon.vert"))
        std::cerr << "Could not read VERTEX shader from file" << std::endl;
    program->addShader(vertShader);

    osg::ref_ptr<osg::Shader> fragShader = new osg::Shader(osg::Shader::FRAGMENT);
    if (!fragShader->loadShaderSourceFromFile("Shaders/polygon.frag"))
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

    osg::Uniform* cameraEye = new osg::Uniform(osg::Uniform::FLOAT_VEC4, "CameraEye");
    cameraEye->setUpdateCallback(new CameraEyeCallback(camera));
    state->addUniform(cameraEye);

    state->addUniform(new osg::Uniform("FogColor", FOG_COLOR));

    // state settings
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    state->setMode(GL_BLEND, osg::StateAttribute::ON);
    state->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);

    osg::BlendFunc* blendfunc = new osg::BlendFunc();
    state->setAttributeAndModes(blendfunc, osg::StateAttribute::ON);

    return geode.release();
}

int main(int, char**)
{
#ifdef _WIN32
    ::SetProcessDPIAware(); // if Windows
#endif // _WIN32

    osgViewer::Viewer viewer;
    osg::DisplaySettings::instance()->setNumMultiSamples(4);

    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(createBezierScene(viewer.getCamera()));
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
