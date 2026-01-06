#ifndef MAIN_CAMERA
#define MAIN_CAMERA

#include <collections.hpp>
#include <vec3.hpp>

#include <iostream>
#include <vector>

class Camera : public Singleton<Camera>
{
    // allows singleton template to access private members
    // of class <T>
    friend class Singleton<Camera>;

    public:
        Camera();
        Vector3 position;
};

#endif