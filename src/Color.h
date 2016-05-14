#ifndef COLOR_H
#define COLOR_H

#include <RGBType.h>

class Color {
    // The RGB will be values between 0.0 and 1.0
    // The special number is... special
    // This is the diffuse colour of an object
    RGBType diffuse;
    double special;
    // Specular colour
    RGBType specular;
    // Emission colour
    RGBType emission;
    // Ambient Light
    RGBType ambient;
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
    RGBType getSpecular() { return specular; }
    RGBType getEmission() { return emission; }
    RGBType getAmbient() { return ambient; }

    double setRed(double redValue) { diffuse.r = redValue; }
    double setGreen(double greenValue) { diffuse.g = greenValue; }
    double setBlue(double blueValue) { diffuse.b = blueValue; }
    double setSpecial(double specialValue) { special = specialValue; }
    double setShine(double shineValue) { shine = shineValue; }
    RGBType setSpecular(RGBType specularColor) { specular = specularColor; }
    RGBType setEmission(RGBType emissionColor) { emission = emissionColor; }
    RGBType setAmbient(RGBType ambientColor) { ambient = ambientColor; }
};

Color::Color () {
    Color(0.5, 0.5, 0.5, 0);
}

Color::Color (double r, double g, double b, double s) {
    diffuse.r = r;
    diffuse.g = g;
    diffuse.b = b;
    special = s;
    // Default value for the ambient light
    ambient.r = 0.2;
    ambient.g = 0.2;
    ambient.b = 0.2;
}

#endif
