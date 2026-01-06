#include <vec3.hpp>

Vector3::Vector3() {
    x = y = z = 0;
}

Vector3::Vector3(float x_, float y_, float z_) {
    x = x_;
    y = y_;
    z = z_;
}

Vector3::Vector3(const Vector3& rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}

Vector3& Vector3::operator=(const Vector3& rhs) {
    if (this == &rhs) return *this;

    x = rhs.x;
    y = rhs.y;
    z = rhs.z;

    return *this;
}

bool Vector3::operator==(const Vector3& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
}
bool Vector3::operator!=(const Vector3& rhs) const {
    return !(*this == rhs);
}