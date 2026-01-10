#ifndef ENDGEN_VEC3
#define ENDGEN_VEC3

#include <cmath>
#include <stdexcept>

class Vector3
{
    public:
        constexpr Vector3(): x(0), y(0), z(0) {}
        constexpr Vector3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}

        float& operator[](int i) {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            throw std::out_of_range("Vector3 index out of range");
        }
        const float& operator[](int i) const {
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

#endif