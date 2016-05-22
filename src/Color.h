#ifndef COLOR_H
#define COLOR_H

#include <RGBType.h>

class Color {
    // The RGB will be values between 0.0 and 1.0
    // This is the diffuse colour of an object
    RGBType diffuse;
    // The special number is... special... it acts as a specularity magnitude if between 0.0 and 1.0
    // and is used for special effects if above that value (see checkerboard pattern for an example)
    double special;
    // Shine
    double shine;

public:

    Color ();

    Color (double, double, double, double);

    // method functions
    double getRed() { return diffuse.r; }
    double getGreen() { return diffuse.g; }
    double getBlue() { return diffuse.b; }
    double getSpecial() { return special; }
    double getShine() { return shine; }
    RGBType getDiffuse() { return diffuse; }

    double setRed(double redValue) { diffuse.r = redValue; }
    double setGreen(double greenValue) { diffuse.g = greenValue; }
    double setBlue(double blueValue) { diffuse.b = blueValue; }
    double setSpecial(double specialValue) { special = specialValue; }
    double setShine(double shineValue) { shine = shineValue; }
};

Color::Color () {
    Color(0.5, 0.5, 0.5, 0);
}

Color::Color (double r, double g, double b, double s) {
    diffuse.r = r;
    diffuse.g = g;
    diffuse.b = b;
    special = s;
    shine = 10;
}

#endif
