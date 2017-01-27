#ifndef DATA_H
#define DATA_H

#include <osg/ref_ptr>
#include <osg/Array>

osg::Vec3Array* createBezierDataPoints()
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

    vertices->push_back(osg::Vec3f(0,0,0));
    vertices->push_back(osg::Vec3f(0,0,1));
    vertices->push_back(osg::Vec3f(1,0,1));
    vertices->push_back(osg::Vec3f(1,0,0));
    vertices->push_back(osg::Vec3f(1,0,0));
    vertices->push_back(osg::Vec3f(1,0,-1));
    vertices->push_back(osg::Vec3f(2,0,-1));
    vertices->push_back(osg::Vec3f(2,0,0));

    vertices->push_back(osg::Vec3f(2,0,0));
    vertices->push_back(osg::Vec3f(2,3,0));
    vertices->push_back(osg::Vec3f(2,3,1));
    vertices->push_back(osg::Vec3f(2,3,2));

    return vertices.release();
}

osg::Vec3Array* createPolylineDataPoints()
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

    float y = -0.5f;
    vertices->push_back(osg::Vec3f(0,y,0));
    vertices->push_back(osg::Vec3f(0,y,1));
    vertices->push_back(osg::Vec3f(1,y,1));
    vertices->push_back(osg::Vec3f(1,y,0));

    vertices->push_back(osg::Vec3f(0,y,1));
    vertices->push_back(osg::Vec3f(1,y,1));
    vertices->push_back(osg::Vec3f(1,y,0));
    vertices->push_back(osg::Vec3f(1,y,0));

    vertices->push_back(osg::Vec3f(1,y,1));
    vertices->push_back(osg::Vec3f(1,y,0));
    vertices->push_back(osg::Vec3f(1,y,0));
    vertices->push_back(osg::Vec3f(1,y,-1));

    vertices->push_back(osg::Vec3f(1,y,0));
    vertices->push_back(osg::Vec3f(1,y,0));
    vertices->push_back(osg::Vec3f(1,y,-1));
    vertices->push_back(osg::Vec3f(2,y,-1));

    vertices->push_back(osg::Vec3f(1,y,0));
    vertices->push_back(osg::Vec3f(1,y,-1));
    vertices->push_back(osg::Vec3f(2,y,-1));
    vertices->push_back(osg::Vec3f(2,y,0));

    return vertices.release();
}

osg::Vec4Array* createBezierColorPoints()
{
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

    colors->push_back(osg::Vec4f(0.1, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.2, 0.1, 0.9, 1));
    colors->push_back(osg::Vec4f(0.7, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));

    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.7, 0.1, 0.9, 1));
    colors->push_back(osg::Vec4f(0.2, 0.9, 0.1, 1));

    colors->push_back(osg::Vec4f(0.0, 0.0, 0.0, 1));
    colors->push_back(osg::Vec4f(0.25, 0.25, 0.25, 1));
    colors->push_back(osg::Vec4f(0.5, 0.5, 0.5, 1));
    colors->push_back(osg::Vec4f(1.0, 1.0, 1.0, 1));

    return colors.release();
}

osg::Vec4Array* createPolylineColorPoints()
{
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

    colors->push_back(osg::Vec4f(0.1, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.2, 0.1, 0.9, 1));
    colors->push_back(osg::Vec4f(0.7, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));

    colors->push_back(osg::Vec4f(0.2, 0.1, 0.9, 1));
    colors->push_back(osg::Vec4f(0.7, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));

    colors->push_back(osg::Vec4f(0.7, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.9, 0.1, 1));

    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.7, 0.1, 0.9, 1));

    colors->push_back(osg::Vec4f(0.9, 0.2, 0.9, 1));
    colors->push_back(osg::Vec4f(0.9, 0.9, 0.1, 1));
    colors->push_back(osg::Vec4f(0.7, 0.1, 0.9, 1));
    colors->push_back(osg::Vec4f(0.2, 0.9, 0.1, 1));

    return colors.release();
}

#endif // DATA_H
