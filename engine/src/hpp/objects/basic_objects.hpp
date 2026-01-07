#ifndef ENDGEN_BASIC_OBJECTS
#define ENDGEN_BASIC_OBJECTS

#include <vec3.hpp>
#include <SDL_stdinc.h>

#define GREEN 0xFF00FF00

class WorldObject {
    public:
        WorldObject(): width(0), length(0), height(0), position(v3_zero), color(GREEN) {}
        WorldObject(
            int w, int l, int h,
            Vector3 pos = v3_zero,
            Uint32 col = GREEN
        ): width(w), length(l), height(h), position(pos), color(col) {}

        Vector3 GetPosition() { return position; }

        int GetWidth() { return width; }
        int GetLength() { return length; }
        int GetHeight() { return height; }

        int GetColor() { return color; }

    protected:
        Vector3 position;
        int width;  // length across x-axis
        int length; // length across z-axis
        int height; // length across y-axis
        Uint32 color;
};

class Ground : public WorldObject {
    public:
        Ground(): WorldObject() {}
        Ground(int w, int l, Vector3 pos = v3_zero, Uint32 col = GREEN): WorldObject(w, l, 0, pos, col) {}
};

class Cube : public WorldObject {
    public:
        Cube(): WorldObject() {}
        Cube(int w, int l, int h, Vector3 pos = v3_zero, Uint32 col = GREEN): WorldObject(w, l, h, pos, col) {}
};

#endif