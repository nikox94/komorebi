#ifndef Rotate_H
#define Rotate_H

#include <Ray.h>
#include <Vect.h>
#include <math.h>
#include <Transform.h>

using namespace std;

class Rotate : public Transform {
    Vect axis;
    double rotateAngle; // Rotate angle is in degrees [0, 360)

public:

    Rotate() {
        axis = Vect(0,0,1);
        rotateAngle = 45;
    }

    Rotate(Vect vector, double angle) {
        axis = vector.normalise();
        rotateAngle = angle;
    }

    Rotate(double x, double y, double z, double angle) {
        axis = Vect(x, y, z).normalise();
        rotateAngle = angle;
    }

    // method functions
    virtual Vect apply (Vect vector) {
        double sine = sin(rotateAngle * M_PI / 180.0);
        double cosine = cos(rotateAngle * M_PI / 180.0);
        Vect result = vector.mult(cosine);
        result = result.add(axis.cross(vector).mult(sine));
        result = result.add(axis.mult(axis.dot(vector)*(1-cosine)));
        return result;
    }

    virtual Ray apply (Ray ray) {
        Vect ray_origin = ray.getRayOrigin();
        Vect ray_direction = ray.getRayDirection();
        return Ray(apply(ray_origin), apply(ray_direction));
    }

    virtual Transform* inverse() {
        return new Rotate(axis, -rotateAngle);
    }

};

#endif
