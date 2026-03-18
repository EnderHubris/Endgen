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
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;
        }

        bool ContainsPoint(int x, int y) {
            for (int i = 0; i < 3; ++i) {
                int k = (i + 1) % 3;

                Vector2 edge = vertices[k] - vertices[i];               // triangle edge
                Vector2 normal = VecMath::Perpendicular(edge) * -1;     // negate to get clock-wise perpendicular
                Vector2 toPoint = Vector2{x,y} - vertices[i];

                float dot = VecMath::Dot(normal, toPoint);
                if (dot < 0) {
                    return false;
                }
            }
            return true;
        }

        Vector2 vertices[3];
        Uint32 color;
};

#endif