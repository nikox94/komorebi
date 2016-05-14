#ifndef Translate_H
#define Translate_H

#include <Ray.h>
#include <Vect.h>
#include <Transform.h>

class Translate : public Transform {
    double translateX, translateY, translateZ;

public:

    Translate() {
        Translate(0.0, 0.0, 0.0);
    }

    Translate(double x, double y, double z) {
        translateX = x;
        translateY = y;
        translateZ = z;
    }

    // method functions
    virtual Vect apply (Vect vector) {
        return Vect(vector.getVectX()+translateX,
                    vector.getVectY()+translateY,
                    vector.getVectZ()+translateZ);
    }

    virtual Ray apply (Ray ray) {
        Vect ray_origin = ray.getRayOrigin();
        Vect ray_direction = ray.getRayDirection();
        return Ray(apply(ray_origin), apply(ray_direction));
    }

    virtual Transform* inverse() {
        return new Translate(-translateX, -translateY, -translateZ);
    }
};

#endif
