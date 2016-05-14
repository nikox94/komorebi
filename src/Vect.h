#ifndef VECT_H
#define VECT_H

#include "math.h"

class Vect {
    double x, y, z;

public:

    Vect ();

    Vect (double, double, double);

    // method functions
    double getVectX() { return x; }
    double getVectY() { return y; }
    double getVectZ() { return z; }

    // define operator== for comparisons
    bool operator==(const Vect& v) const
    {
        // TODO: "Extract" the epsilon to a global var
        double epsilon = 1E-6;
        double x_diff = fabs(this->x - v.x);
        double y_diff = fabs(this->y - v.y);
        double z_diff = fabs(this->z - v.z);
        return (x_diff < epsilon) && (y_diff < epsilon) && (z_diff < epsilon);
    }

    // TODO:Rename to modulus
    double magnitude() {
        return sqrt(x*x+y*y+z*z);
    }

    Vect normalise() {
        double mag = magnitude();
        return Vect(x/mag, y/mag, z/mag);
    }

    Vect negative () {
        return Vect(-x,-y,-z);
    }

    double dot (Vect v) {
        return x*v.x + y*v.y + z*v.z;
    }

    Vect cross (Vect v) {
        return Vect(y*v.z-v.y*z, z*v.x - v.z*x, x*v.y - v.x*y);
    }

    Vect add (Vect v) {
        return Vect(x+v.x, y+v.y, z+v.z);
    }

    Vect mult (double s) {
        return Vect(s*x, s*y, s*z);
    }

    // get the Vect between this one and B, where this and B are point Vectors from the origin, i.e. OA and OB to get AB.
    Vect getVectBetweenTwoPoints(Vect B) {
        return B.add(this->negative());
    }
};

Vect::Vect () {
    Vect(0, 0, 0);
}

Vect::Vect (double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

#endif
