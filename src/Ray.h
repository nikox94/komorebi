#ifndef RAY_H
#define RAY_H

#include "math.h"
#include "Vect.h"

class Ray {
    Vect origin, direction;

public:

    Ray ();

    Ray (Vect, Vect);

    // method functions
    // TODO: remove annoying 'Ray' part of name
    Vect getRayOrigin () { return origin; }
    Vect getRayDirection() { return direction; }

};

Ray::Ray () {
    Vect O(0,0,0);
    Ray(O, O);
}

Ray::Ray (Vect origin, Vect direction) {
    this->origin = origin;
    this->direction = direction;
}

#endif
