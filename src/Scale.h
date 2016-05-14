#ifndef Scale_H
#define Scale_H

#include <Ray.h>
#include <Vect.h>
#include <Transform.h>

class Scale : public Transform {
    double scaleX, scaleY, scaleZ;

public:

    Scale() {
        Scale(1.0, 1.0, 1.0);
    }

    Scale(double x, double y, double z) {
        scaleX = x;
        scaleY = y;
        scaleZ = z;
    }

    // method functions
    virtual Vect apply (Vect vector) {
        return Vect(vector.getVectX()*scaleX,
                    vector.getVectY()*scaleY,
                    vector.getVectZ()*scaleZ);
    }

    virtual Ray apply (Ray ray) {
        Vect ray_origin = ray.getRayOrigin();
        Vect ray_direction = ray.getRayDirection();
        return Ray(apply(ray_origin), apply(ray_direction));
    }

    virtual Transform* inverse() {
        return new Scale(1/scaleX, 1/scaleY, 1/scaleZ);
    }

};

#endif
