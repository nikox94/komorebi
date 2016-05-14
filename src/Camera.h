#ifndef Camera_H
#define Camera_H

#include "math.h"

class Camera {
    // TODO: The last three should always be normalised?
    Vect campos, camdir, camright, camdown;

public:

    Camera ();

    Camera (Vect, Vect, Vect, Vect);

    // method functions
    Vect getCameraPosition () { return campos; }
    Vect getCameraDirection() { return camdir; }
    Vect getCameraRight() { return camright; }
    Vect getCameraDown() { return camdown; }

};

Camera::Camera () {
    Vect campos = Vect(-10, 0, 0);
    Vect camdir = Vect(0,0,0);
    Vect camright = Vect(0,-1,0);
    Vect camdown = Vect(0,0,-1);
    Camera(campos, camdir, camright, camdown);
}

Camera::Camera (Vect campos, Vect camdir, Vect camright, Vect camdown) {
    this -> campos = campos;
    this -> camdir = camdir;
    this -> camdown = camdown;
    this -> camright = camright;
}

#endif
