#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>
#include <Object.h>
#include <Vect.h>
#include <Color.h>
#include <vector>
#include <Transform.h>

class Sphere : public Object {
    Vect center;
    double radius;
    Color color;
    std::vector<Transform*> transforms;

public:

    Sphere ();

    Sphere (Vect, double, Color);

    Sphere (Vect, double, Color, std::vector<Transform*>);

    // method functions
    Vect getCenter () { return center; }
    double getRadius() { return radius; }
    virtual Color getColor () { return color; }

    virtual Vect getNormalAt(Vect point) {
        // normal always points away from the center of a Sphere
        Vect normal_Vect = point.add(center.negative()).normalise();
        return normal_Vect;
    }

    virtual double findIntersection(Ray ray) {
        // Inverse transform rays before intersection test
        Vect ray_origin = ray.getRayOrigin();
        Vect ray_direction = ray.getRayDirection();
        Vect sphere_center = center;
        for (int i = transforms.size()-1 ; i >= 0 ; i--) {
            //Transform* transform = transforms[i];
            //ray_origin = transform->inverse()->apply(ray_origin);
            //ray_direction = transform->inverse()->apply(ray_direction);
            //center = transform->apply(center);
        }

        double ray_origin_x = ray_origin.getVectX();
        double ray_origin_y = ray_origin.getVectY();
        double ray_origin_z = ray_origin.getVectZ();

        double ray_direction_x = ray_direction.getVectX();
        double ray_direction_y = ray_direction.getVectY();
        double ray_direction_z = ray_direction.getVectZ();

        double sphere_center_x = sphere_center.getVectX();
        double sphere_center_y = sphere_center.getVectY();
        double sphere_center_z = sphere_center.getVectZ();

        double a = 1; // normalised
        double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) +
        (2*(ray_origin_y - sphere_center_y)*ray_direction_y) +
        (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2) +
        pow(ray_origin_y - sphere_center_y, 2) +
        pow(ray_origin_z - sphere_center_z, 2) -
        radius*radius;

        double discriminant = b*b - 4*c;

        if (discriminant > 0) {
            // the ray intersects the sphere

            // the first root
            double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.000001; // helps with computation errors

            if (root_1 > 0) {
                // the first root is the smallest positive root
                return root_1;
            }
            else {
                // the second root is the smallest positive root
                double root_2 = (sqrt(discriminant)-b)/2 - 0.000001;
                return root_2;
            }
        }
        else {
            // the ray missed the sphere
            return -1;
        }
    }

};

Sphere::Sphere () {
    center = Vect(0,0,0);
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere (Vect center, double radius, Color color) {
    this->center = center;
    this->radius = radius;
    this->color = color;
}

Sphere::Sphere (Vect center, double radius, Color color, std::vector<Transform*> transforms) {
    for (int i = transforms.size()-1 ; i >= 0 ; i--) {
        Transform* transform = transforms[i];
        //ray_origin = transform->inverse()->apply(ray_origin);
        //ray_direction = transform->inverse()->apply(ray_direction);
        center = transform->apply(center);
    }
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->transforms = transforms;
}

#endif
