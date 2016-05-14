#ifndef OBJECT_H
#define OBJECT_H

#include <Ray.h>
#include <Vect.h>
#include <Color.h>

/**
 * Parent class for all scene objects.
 */
class Object {
public:

    Object ();

    // method functions
    virtual Color getColor () { return Color(0.0, 0.0, 0.0, 0); }

    virtual Vect getNormalAt(Vect point) {
        return Vect(0, 0, 0);
    }

    // This returns the intersection point as a multiple of the ray.
    // Let return := t, then t*ray is the position of the point of intersection.
    virtual double findIntersection(Ray ray) {
        return 0;
    }

};

Object::Object () {}

#endif
