#ifndef Light_H
#define Light_H

#include "Vect.h"
#include "Color.h"
#include "Source.h"

struct AttenuationType {
    double constant, linear, quadratic;

    // default + parameterized constructor
    AttenuationType (double c=1, double l=0, double q=0)
        : constant(c), linear(l), quadratic(q) {}
};

/**
 * The class for a point light-source.
 */
class Light : public Source{
    Vect position;
    Color color;
    AttenuationType attenuation;

public:

    Light ();

    Light (Vect, Color);

    AttenuationType getAttenuation() { return attenuation; }

    AttenuationType setAttenuation(AttenuationType attenuationValue) {
        attenuation = attenuationValue;
    }

    AttenuationType setAttenuation(double c, double l, double q) {
        attenuation.constant = c;
        attenuation.linear = l;
        attenuation.quadratic = q;
    }

    virtual double getAttenuationConstant() { return attenuation.constant; }
    virtual double getAttenuationLinear() { return attenuation.linear; }
    virtual double getAttenuationQuadratic() { return attenuation.quadratic; }
    virtual Vect getPosition() { return position; }
    virtual Color getColor() { return color; }
};

Light::Light () {
    Light(Vect(0, 0, 0), Color(1, 1, 1, 0));
}

Light::Light (Vect p, Color c) {
    position = p;
    color = c;
    attenuation.constant = 1;
}

#endif
