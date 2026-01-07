#ifndef ENDGEN_BASIC_OBJECTS
#define ENDGEN_BASIC_OBJECTS

#include <cmath>
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

        virtual bool ContainsPoint(const Vector3& point) = 0;
        
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
        bool ContainsPoint(const Vector3& point) override {
            bool withinWidth = std::abs(position.x - point.x) <= width * 0.5f;
            bool withinLength = std::abs(position.z - point.z) <= length * 0.5f;
            return withinLength && withinWidth;
        }
};

class Cube : public WorldObject {
    public:
        Cube(): WorldObject() {}
        Cube(int w, int l, int h, Vector3 pos = v3_zero, Uint32 col = GREEN): WorldObject(w, l, h, pos, col) {}
        bool ContainsPoint(const Vector3& point) override {
            float halfW = width  * 0.5f;
            float halfH = height * 0.5f;
            float halfL = length * 0.5f;

            bool withinWidth  = std::abs(point.x - position.x) <= halfW;
            bool withinHeight = std::abs(point.y - position.y) <= halfH;
            bool withinLength = std::abs(point.z - position.z) <= halfL;

            return withinWidth && withinHeight && withinLength;
        }
};

#endif