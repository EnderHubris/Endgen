#ifndef MAIN_CAMERA
#define MAIN_CAMERA

#include <collections.hpp>
#include <vec3.hpp>

#include <iostream>
#include <vector>

#define VOID_COLOR 0xFF111417 // 0xARBG (in-hex)

class Camera : public Singleton<Camera>
{
    // allows singleton template to access private members
    // of class <T>
    friend class Singleton<Camera>;

    public:
        Camera();
        Camera(Vector3 pos);

        void Update();

        float FovRad() const { return fov * M_PI / 180.f; };

        float xRot;
        float yRot;
        float fov = 75;

        Vector3 position;

        Vector3 forward;
        Vector3 right;
        Vector3 up;
};

#endif