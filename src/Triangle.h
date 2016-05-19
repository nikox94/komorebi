#ifndef Triangle_H
#define Triangle_H

#include <math.h>
#include <vector>
#include <Object.h>
#include <Vect.h>
#include <Color.h>

using namespace std;

class Triangle : public Object {
    Vect A, B, C;
    Color color;

public:

    Triangle ();

    Triangle (Vect, Vect, Vect, Color);


    // method functions
    Vect getAC() {
        return A.getVectBetweenTwoPoints(C);
    }

    Vect getCA() {
        return getAC().negative();
    }

    Vect getAB() {
        return A.getVectBetweenTwoPoints(B);
    }

    Vect getBC() {
        return B.getVectBetweenTwoPoints(C);
    }

    Vect getNormal () {
        Vect AC = getAC();
        Vect AB = getAB();
        Vect normal = AC.cross(AB).normalise();
        return normal;
    }

    double getDistance() {
        Vect normal = getNormal();
        double distance = normal.dot(A);
        return distance;

    }

    virtual Color getColor () { return color; }

    virtual Vect getNormalAt(Vect point) {
        return getNormal();
    }

    virtual double findIntersection(Ray ray) {
        Vect ray_direction = ray.getRayDirection();
        Vect ray_origin = ray.getRayOrigin();

        Vect normal = getNormal();
        double distance = getDistance();

        double a = ray_direction.dot(normal);

        if (a == 0) {
            // ray is parallel to the Triangle
            return -1;
        }
        else {
            double b = normal.dot(ray.getRayOrigin().add(normal.mult(distance).negative()));
            double distanceToPlane = -1*b/a;

            // Point of intersection Q
            Vect Q = ray_direction.mult(distanceToPlane).add(ray_origin);
            Vect CA = getCA();
            Vect QA = Q.getVectBetweenTwoPoints(A);
            Vect BC = getBC();
            Vect QC = Q.getVectBetweenTwoPoints(C);
            Vect AB = getAB();
            Vect QB = Q.getVectBetweenTwoPoints(B);
            double test_1 = CA.cross(QA).dot(normal);
            double test_2 = BC.cross(QC).dot(normal);
            double test_3 = AB.cross(QB).dot(normal);
            // [CAxQA]*normal >= 0 AND
            // [BCxQC]*normal >= 0 AND
            // [ABxQB]*normal >= 0
            if( test_1 >= 0 && test_2 >= 0 && test_3 >= 0) {
                // inside triangle
                return distanceToPlane;
            }
            else {
                // outside triangle
                return -1;
            }
        }
    }

};

Triangle::Triangle () {
    Triangle(Vect(1,0,0),
             Vect(0,1,0),
             Vect(0,0,1),
             Color(0.5, 0.5, 0.5, 0));
}

Triangle::Triangle (Vect A, Vect B, Vect C, Color color) {
    this->A = A;
    this->B = B;
    this->C = C;
    this->color = color;
}

#endif
