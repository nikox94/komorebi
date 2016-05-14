/*
 * This defines a struct for Red, Green and Blue colour values.
 */

#ifndef RGBType_H
#define RGBType_H

struct RGBType {
    double r;
    double g;
    double b;

    // default + parameterized constructor
    RGBType(double r=0, double g=0, double b=0)
        : r(r), g(g), b(b) {}

    RGBType& operator=(const RGBType& a)
    {
        r = a.r;
        g = a.g;
        b = a.b;
        return *this;
    }

    RGBType& operator+=(const RGBType& a)
    {
        r = a.r + r;
        g = a.g + g;
        b = a.b + b;
        return *this;
    }

    RGBType operator+(const RGBType& a) const
    {
        return RGBType(a.r+r, a.g+g, a.b+b);
    }

    RGBType operator*(const RGBType& a) const
    {
        return RGBType(a.r*r, a.g*g, a.b*b);
    }

    RGBType operator*(const double& scalar) const
    {
        return RGBType(scalar*r, scalar*g, scalar*b);
    }

    bool operator==(const RGBType& a) const
    {
        return (r == a.r && g == a.g && b == a.b);
    }

    double brightness() {
        return (r + g + b)/3;
    }

    RGBType colorAverage(RGBType color) {
        return RGBType ((r + color.r)/2, (g + color.g)/2, (b + color.b)/2);
    }

    // Clip a color if it got out of bounds
    RGBType clip() {
        double alllight = r + g + b;
        double excesslight = alllight - 3;
        if (excesslight > 0) {
            r = r + excesslight*(r/alllight);
            g = g + excesslight*(g/alllight);
            b = b + excesslight*(b/alllight);
        }
        if (r > 1) {r = 1;}
        if (g > 1) {g = 1;}
        if (b > 1) {b = 1;}
        if (r < 0) {r = 0;}
        if (g < 0) {g = 0;}
        if (b < 0) {b = 0;}

        return RGBType (r, g, b);
    }
};

#endif
