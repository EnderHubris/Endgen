#include <camera.hpp>

Camera::Camera(): xRot(0), yRot(0), position(v3_zero) {};
Camera::Camera(Vector3 pos): xRot(0), yRot(0), position(pos) {}

void Camera::Update() {
    // Update the vectors once per frame
    forward = ComputeForward();
    right = ComputeRight();
    up = ComputeUp();
}

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

Vector3 Camera::ComputeForward() const {
    Vector3 forward = v3_fwd;

    // yaw
    forward = RotateAroundAxis(forward, v3_up, yRot).Normalize();

    // right axis
    Vector3 right = Cross(forward, v3_up).Normalize();

    // pitch
    forward = RotateAroundAxis(forward, right, xRot).Normalize();

    return forward;
}
Vector3 Camera::ComputeRight() const {
    return Cross(ComputeForward(), v3_up).Normalize();
}
Vector3 Camera::ComputeUp() const {
    return Cross(ComputeRight(), ComputeForward()).Normalize();
}