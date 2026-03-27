#ifndef ENDGEN_BASIC_OBJECTS
#define ENDGEN_BASIC_OBJECTS

// do not include multiple times
#pragma once

#include <cmath>
#include <ctime>
#include <cstdlib>

#include <vec.hpp>
#include <SDL_stdinc.h>

#define GREEN          0xFF00FF00
#define SKY_BLUE       0xFF3A7BD5
#define CRIMSON_RED    0xFFE94F37
#define EMERALD_GREEN  0xFF2ECC71
#define ROYAL_PURPLE   0xFF9B59B6
#define SUNFLOWER      0xFFF1C40F
#define TEAL_OCEAN     0xFF16A085
#define TANGERINE      0xFFE67E22
#define SLATE_BLUE     0xFF34495E
#define PINK_ROSE      0xFFFF6F91
#define AQUA_MINT      0xFF00C9A7

namespace ColorUtil {
    /**
     * Generates a random 0xARGB color value
     */
    inline Uint32 RandomColor() {
        Uint8 r = rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;

        return (0xFF << 24) | (r << 16) | (g << 8) | b;
    }
};

class Triangle {
    public:
        Triangle(Vector2 v1, Vector2 v2, Vector2 v3): color(ColorUtil::RandomColor()) {
            vertices[0] = Vector3{v1.x, v1.y, 0.f};
            vertices[1] = Vector3{v2.x, v2.y, 0.f};
            vertices[2] = Vector3{v3.x, v3.y, 0.f};
        }
        Triangle(Vector3 v1, Vector3 v2, Vector3 v3): color(ColorUtil::RandomColor()) {
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;
        }

        Triangle(const Triangle& other) {
            color = other.color;
            for (int i = 0; i < 3; ++i) {
                vertices[i] = other.vertices[i];
            }
        }

        bool ContainsPoint(int x, int y) {
            for (int i = 0; i < 3; ++i) {
                int k = (i + 1) % 3;

                Vector2 edge = vertices[k].ToVector2() - vertices[i].ToVector2(); // triangle edge
                Vector2 normal = VecMath::Perpendicular(edge) * -1;               // negate to get clock-wise perpendicular
                Vector2 toPoint = Vector2{x,y} - vertices[i].ToVector2();

                float dot = VecMath::Dot(normal, toPoint);
                if (dot < 0) {
                    return false;
                }
            }
            return true;
        }

        bool ContainsPoint(int x, int y, int z) {
            for (int i = 0; i < 3; ++i) {
                int k = (i + 1) % 3;

                Vector3 edge = vertices[k] - vertices[i];               // triangle edge
                Vector3 normal = VecMath::Perpendicular(edge) * -1;     // negate to get clock-wise perpendicular
                Vector3 toPoint = Vector3{x,y,z} - vertices[i];

                float dot = VecMath::Dot(normal, toPoint);
                if (dot < 0) {
                    return false;
                }
            }
            return true;
        }

        void Shift() {
            for (auto& v : vertices) {
                v.x += velx;
                v.y += vely;
            }
        }
        
        Vector3 vertices[3];
        Uint32 color;
    
    private:
        const float minX = 25.f, maxX = 100.f;
        const float minY = 25.f, maxY = 100.f;
        double velx = (std::fmod(static_cast<float>(std::rand()), maxX) + minX) * (std::rand() % 2) ? 1 : -1;
        double vely = (std::fmod(static_cast<float>(std::rand()), maxY) + minY) * (std::rand() % 2) ? 1 : -1;
};

#endif