#ifndef _BOUNDING_SHAPE_H_
#define _BOUNDING_SHAPE_H_

#include "entity.h"

#include <vector>

DEF_ENTITY
(
    BoundingShape,
    Entity,
    CONSTRUCTOR(),
    PUBLIC
    (
        virtual void DebugRender() {}
        virtual bool RayTest(const ray& r, vec3f& point) { return false; }
    ),
    PROTECTED()
);

DEF_ENTITY
(
    AABBox,
    BoundingShape,
    CONSTRUCTOR
    (
        global_matrixmodel = GFXGlobal<mat4f>::Get("MatrixModel0");
    ),
    PUBLIC
    (
        void Size(float x, float y, float z)
        {
            size = vec3f(x, y, z);
        }

        void DebugRender()
        {
            global_matrixmodel = node->GetTransform();
            std::vector<vec3f> points;
            points.push_back(vec3f(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f));
            points.push_back(vec3f(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
            line.Render(points);
        }
    ),
    PROTECTED
    (
        vec3f size;
        GFXGlobal<mat4f> global_matrixmodel;
        GFXLine line;
    )
);

DEF_ENTITY
(
    BBox,
    BoundingShape,
    CONSTRUCTOR
    (
        global_matrixmodel = GFXGlobal<mat4f>::Get("MatrixModel0");
    ),
    PUBLIC
    (
        void Size(float x, float y, float z)
        {
            box.a = vec3f(x * -0.5f, y * -0.5f, z * -0.5f);
            box.b = vec3f(x * 0.5f, y * 0.5f, z * 0.5f);
        }

        bool RayTest(const ray& r, vec3f& point)
        {
            ray tray = r;

            tray = node->GetTransform() * r;
            
            if (intersect(tray, box, point))
            {
                point = node->GetTransform() * vec4f(point.x, point.y, point.z, 1.0f);
                return true;
            }
            else
            {
                point = vec3f(0, 0, 0);
                return false;
            }
        }

        void DebugRender()
        {
            global_matrixmodel = node->GetTransform();
            std::vector<vec3f> points;
            points.push_back(vec3f(box.a.x, box.a.y, box.a.z));
            points.push_back(vec3f(box.b.x, box.a.y, box.a.z));
            points.push_back(vec3f(box.b.x, box.a.y, box.a.z));
            points.push_back(vec3f(box.b.x, box.b.y, box.a.z));
            points.push_back(vec3f(box.b.x, box.b.y, box.a.z));
            points.push_back(vec3f(box.a.x, box.b.y, box.a.z));
            points.push_back(vec3f(box.a.x, box.b.y, box.a.z));
            points.push_back(vec3f(box.a.x, box.a.y, box.a.z));
            points.push_back(vec3f(box.a.x, box.a.y, box.b.z));
            points.push_back(vec3f(box.b.x, box.a.y, box.b.z));
            points.push_back(vec3f(box.b.x, box.a.y, box.b.z));
            points.push_back(vec3f(box.b.x, box.b.y, box.b.z));
            points.push_back(vec3f(box.b.x, box.b.y, box.b.z));
            points.push_back(vec3f(box.a.x, box.b.y, box.b.z));
            points.push_back(vec3f(box.a.x, box.b.y, box.b.z));
            points.push_back(vec3f(box.a.x, box.a.y, box.b.z));
            points.push_back(vec3f(box.a.x, box.a.y, box.a.z));
            points.push_back(vec3f(box.a.x, box.a.y, box.b.z));
            points.push_back(vec3f(box.b.x, box.a.y, box.a.z));
            points.push_back(vec3f(box.b.x, box.a.y, box.b.z));
            points.push_back(vec3f(box.b.x, box.b.y, box.a.z));
            points.push_back(vec3f(box.b.x, box.b.y, box.b.z));
            points.push_back(vec3f(box.a.x, box.b.y, box.a.z));
            points.push_back(vec3f(box.a.x, box.b.y, box.b.z));
            line.Render(points);
        }
    ),
    PROTECTED
    (
        aabb box;
        GFXGlobal<mat4f> global_matrixmodel;
        GFXLine line;
    )
);

DEF_ENTITY
(
    BSphere,
    BoundingShape,
    CONSTRUCTOR
    (
        global_matrixmodel = GFXGlobal<mat4f>::Get("MatrixModel0");
    ),
    PUBLIC
    (
        void Radius(float r) 
        {
            _sphere.radius = r;
            circle.clear();
            for (float i = 0; i < PI * 2; i += 0.1f)
            {
                circle.push_back(vec2f(sinf(i) * _sphere.radius, cosf(i) * _sphere.radius));
                circle.push_back(vec2f(sinf(i + 0.1f) * _sphere.radius, cosf(i + 0.1f) * _sphere.radius));
            }
        }

        bool RayTest(const ray& r, vec3f& point)
        {
            ray tray = r;

            tray = node->GetTransform() * r;
            
            if (intersect(tray, _sphere, point))
            {
                point = node->GetTransform() * vec4f(point.x, point.y, point.z, 1.0f);
                return true;
            }
            else
            {
                point = vec3f(0, 0, 0);
                return false;
            }
        }
        
        void DebugRender()
        {
            global_matrixmodel = node->GetTransform();
            std::vector<vec3f> points;
            for (unsigned int i = 0; i < circle.size(); ++i)
                points.push_back(vec3f(circle[i].x, circle[i].y, 0.0f));
            for (unsigned int i = 0; i < circle.size(); ++i)
                points.push_back(vec3f(circle[i].x, 0.0f, circle[i].y));
            for (unsigned int i = 0; i < circle.size(); ++i)
                points.push_back(vec3f(0.0f, circle[i].x, circle[i].y));
            line.Render(points);
        }
    ),
    PROTECTED
    (
        sphere _sphere;
        GFXGlobal<mat4f> global_matrixmodel;
        GFXLine line;
        std::vector<vec2f> circle;
    )
);

#endif