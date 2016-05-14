#ifndef Transform_H
#define Transform_H

#include <Ray.h>
#include <Vect.h>

/**
 * Parent class for all scene Transforms.
 */
class Transform {
public:

    Transform ();

    // method functions
    virtual Vect apply (Vect vector) { return Vect(0.0, 0.0, 0.0); }

    virtual Ray apply (Ray ray) {return Ray(); }

    virtual Transform* inverse () {
        return new Transform();
    }

};

Transform::Transform () {}

#endif
