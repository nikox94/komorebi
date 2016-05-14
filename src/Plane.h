#ifndef PLANE_H
#define PLANE_H

#include <math.h>
#include <Object.h>
#include <Vect.h>
#include <Color.h>

class Plane : public Object {
    Vect normal;
    double distance;
    Color color;

public:

    Plane ();

    Plane (Vect, double, Color);

    // method functions
    Vect getNormal () { return normal; }
    double getDistance() { return distance; }
    virtual Color getColor () { return color; }

    virtual Vect getNormalAt(Vect point) {
        return normal;
    }

    virtual double findIntersection(Ray ray) {
        Vect ray_direction = ray.getRayDirection();

        // TODO: check case ray_dir = - normal, but
        // still intersecting plane
        double a = ray_direction.dot(normal);

        // TODO: Careful about floats and comparing to 0.
        // Consider |a|< \epsilon
        if (a == 0) {
            // ray is parallel to the plane
            return -1;
        }
        else {
            double b = normal.dot(ray.getRayOrigin().add(normal.mult(distance).negative()));
            return -1*b/a;
        }
    }

};

Plane::Plane () {
    normal = Vect(1,0,0);
    distance = 0.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane (Vect normal, double distance, Color color) {
    this->normal = normal;
    this->distance = distance;
    this->color = color;
}

#endif
