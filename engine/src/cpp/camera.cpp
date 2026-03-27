#include <camera.hpp>

Camera::Camera(): xRot(0), yRot(0), position(v3_zero) {};
Camera::Camera(Vector3 pos): xRot(0), yRot(0), position(pos) {}

float Deg2Rad(float deg) { return deg * M_PI / 180.f; }

Vector3 Cross(const Vector3& a, const Vector3& b) {
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float Dot(const Vector3& a, const Vector3& b) {
    return (a.y * b.x) + (a.y * b.y) + (a.z * b.z);
}

// Vector Rotation Helpers
Vector3 RotateX(const Vector3& v, float deg)
{
    // y' = y*cos(theta) - z*sin(theta)
    // z' = y*sin(theta) + z*cos(theta)

    float angleRad = Deg2Rad(deg);

    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    return Vector3(
        v.x,
        v.y * c - v.z * s,
        v.y * s + v.z * c
    );
}

Vector3 RotateY(const Vector3& v, float deg)
{
    // x' = x*cos(theta) + z*sin(theta)
    // z' = -x*sin(theta) + z*cos(theta)

    float angleRad = Deg2Rad(deg);

    float c = std::cos(angleRad);
    float s = std::sin(angleRad);

    return Vector3(
        v.x * c + v.z * s,
        v.y,
        -v.x * s + v.z * c
    );
}

// returns vector with the applied rotation of a given rotX and rotY
Vector3 Rotate(const Vector3& v, float rotX, float rotY)
{
    Vector3 out = RotateY(v, Deg2Rad(rotY));
    out = RotateX(out, Deg2Rad(rotX));
    return out;
}

Vector3 RotateAroundAxis(const Vector3& v, const Vector3& axis, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return v * cosA + Cross(axis, v) * sinA + axis * Dot(axis, v) * (1 - cosA);
}

// Applies rotation to the camera based on xRot, yRot values
// updating local vectors: fwd, right, and up
void Camera::Update() {
    // lock rotation angles between 0-360 degrees
    xRot = std::fmod(xRot, 360.f);
    yRot = std::fmod(yRot, 360.f);

    // Update the vectors once per frame
    Vector3 fwd = v3_fwd;

    // yaw
    fwd = RotateAroundAxis(fwd, v3_up, yRot).Normalized();

    // right axis
    right = Cross(fwd, v3_up).Normalized();

    // pitch
    forward = RotateAroundAxis(fwd, right, xRot).Normalized();

    up = Cross(right, forward).Normalized();
}