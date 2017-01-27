#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <osg/Uniform>
#include <osg/Camera>
#include <osg/NodeVisitor>

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

struct ViewportCallback: public osg::Uniform::Callback
{
    ViewportCallback(osg::Camera* camera) :
        _camera(camera) {
    }

    virtual void operator()(osg::Uniform* uniform, osg::NodeVisitor* /*nv*/) {
        const osg::Viewport* viewport = _camera->getViewport();
        osg::Vec2f viewportVector = osg::Vec2f(viewport->width(), viewport->height());
        uniform->set(viewportVector);
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


#endif // CALLBACKS_H
