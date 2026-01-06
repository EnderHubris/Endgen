#ifndef ENDGEN_VEC3
#define ENDGEN_VEC3

class Vector3
{
    public:
        Vector3();
        Vector3(float x_, float y_, float z_);
        Vector3(const Vector3& rhs);
        Vector3& operator=(const Vector3& rhs);

        bool operator==(const Vector3& rhs) const;
        bool operator!=(const Vector3& rhs) const;

    private:
        float x;
        float y;
        float z;
};

#endif