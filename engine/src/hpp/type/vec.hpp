#ifndef ENDGEN_VEC
#define ENDGEN_VEC

#include <cmath>
#include <stdexcept>

class Vector2 {
    public:
        constexpr Vector2(): x(0), y(0) {}
        constexpr Vector2(float x_, float y_): x(x_), y(y_) {}
        constexpr Vector2(int x_, int y_): x((float)x_), y((float)y_) {}

        float operator[](int i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            throw std::out_of_range("Vector2 index out of range");
        }

        bool operator==(const Vector2& rhs) const {
            return x == rhs.x && y == rhs.y;
        }
        bool operator!=(const Vector2& rhs) const {
            return !(*this == rhs);
        }

        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }
        Vector2 operator*(const Vector2& rhs) const {
            return Vector2(x * rhs.x, y * rhs.y);
        }
        
        Vector2 operator/(float scalar) const {
            if (scalar == 0.f) return Vector2(0,0); // avoid division by zero
            return Vector2(x / scalar, y / scalar);
        }
        Vector2 operator/(const Vector2& rhs) const {
            if (rhs == Vector2(0,0)) return Vector2(0,0); // avoid division by zero
            return Vector2(x / rhs.x, y / rhs.y);
        }
        
        Vector2 operator+(const Vector2& rhs) const {
            return Vector2(x + rhs.x, y + rhs.y);
        }
        Vector2 operator-(const Vector2& rhs) const {
            return Vector2(x - rhs.x, y - rhs.y);
        }

        float Length() const {
            return std::sqrt(x*x + y*y);
        }

        Vector2 Normalize() const {
            float len = Length();
            if (len == 0.f) return Vector2(0,0); // avoid division by zero
            return Vector2(x / len, y / len);
        }

        static constexpr Vector2 Zero() {
            return Vector2{0.f, 0.f};
        }
        static constexpr Vector2 Right() {
            return Vector2{1,0};
        }
        static constexpr Vector2 Up() {
            return Vector2{0,1};
        }

        float x;
        float y;
};

#define v2_zero     Vector2::Zero()
#define v2_right    Vector2::Right()
#define v2_up       Vector2::Up()

class Vector3 {
    public:
        constexpr Vector3(): x(0), y(0), z(0) {}
        constexpr Vector3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}
        constexpr Vector3(int x_, int y_, int z_): x((float)x_), y((float)y_), z((float)z_) {}

        // Allow casting from v2 -> v3
        Vector3(Vector2 v2): x(v2.x), y(v2.y), z(0) {}

        float operator[](int i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            throw std::out_of_range("Vector3 index out of range");
        }

        bool operator==(const Vector3& rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }
        bool operator!=(const Vector3& rhs) const {
            return !(*this == rhs);
        }

        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
        Vector3 operator*(const Vector3& rhs) const {
            return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
        }
        
        Vector3 operator/(float scalar) const {
            if (scalar == 0.f) return Vector3(0,0,0); // avoid division by zero
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        Vector3 operator/(const Vector3& rhs) const {
            if (rhs == Vector3(0,0,0)) return Vector3(0,0,0); // avoid division by zero
            return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
        }
        
        Vector3 operator+(const Vector3& rhs) const {
            return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
        }
        Vector3 operator-(const Vector3& rhs) const {
            return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        float Length() const {
            return std::sqrt(x*x + y*y + z*z);
        }

        Vector3 Normalize() const {
            float len = Length();
            if (len == 0.f) return Vector3(0,0,0); // avoid division by zero
            return Vector3(x / len, y / len, z / len);
        }

        // predefined

        // Usage: Vector3 v = Vector3::Zero();
        static constexpr Vector3 Zero() {
            return Vector3{0.f, 0.f, 0.f};
        }
        static constexpr Vector3 One() {
            return Vector3{1,1,1};
        }
        static constexpr Vector3 Forward() {
            return Vector3{0,0,1};
        }
        static constexpr Vector3 Right() {
            return Vector3{1,0,0};
        }
        static constexpr Vector3 Up() {
            return Vector3{0,1,0};
        }

        float x;
        float y;
        float z;
};

#define v3_zero     Vector3::Zero()
#define v3_one      Vector3::One()
#define v3_fwd      Vector3::Forward()
#define v3_right    Vector3::Right()
#define v3_up       Vector3::Up()

namespace VecMath {
    inline int Dot(Vector3 a, Vector3 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }
    inline Vector2 Perpendicular(const Vector2& v) {
        return {-v.y, v.x};
    }
};

#endif