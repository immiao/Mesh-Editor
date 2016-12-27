#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cmath>
#include "glm/glm.hpp"
static const float PI = 3.14159265358979323846f;

struct vec4
{
    float p[4];
    vec4()
    {
        p[0] = 0.0f;
        p[1] = 0.0f;
        p[2] = 0.0f;
        p[3] = 1.0f;
    }
    vec4(float x, float y, float z, float w)
    {
        p[0] = x;
        p[1] = y;
        p[2] = z;
        p[3] = w;
    }
    void SetVec(float a, float b, float c, float d)
    {
        p[0] = a;
        p[1] = b;
        p[2] = c;
        p[3] = d;
    }
};

inline void print(glm::vec3 p)
{
    printf("%f %f %f\n", p.x, p.y, p.z);
}


inline void print(glm::vec4 p)
{
    printf("%f %f %f %f\n", p.x, p.y, p.z, p.w);
}

inline void print(glm::mat4 m)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%f ", m[j][i]);
        printf("\n");
    }
}

inline vec4 operator-(float a, vec4 color)
{
    return vec4(a - color.p[0], a - color.p[1], a - color.p[2], 1.0f);
}

inline vec4 operator-(vec4 a, vec4 b)
{
    return vec4(a.p[0] - b.p[0], a.p[1] - b.p[1], a.p[2] - b.p[2], 1.0f);
}

inline vec4 operator+(vec4 a, vec4 b)
{
    return vec4(a.p[0] + b.p[0], a.p[1] + b.p[1], a.p[2] + b.p[2], 1.0f);
}

inline vec4 operator/(vec4 a, int n)
{
    return vec4(a.p[0] / n, a.p[1] / n, a.p[2] / n, 1.0f);
}

inline vec4 operator*(vec4 a, int n)
{
    return vec4(a.p[0] * n, a.p[1] * n, a.p[2] * n, 1.0f);
}

inline vec4 operator*(vec4 a, float n)
{
    return vec4(a.p[0] * n, a.p[1] * n, a.p[2] * n, 1.0f);
}

inline vec4 cross(vec4 a, vec4 b)
{
    return vec4(a.p[1] * b.p[2] - a.p[2] * b.p[1], a.p[2] * b.p[0] - a.p[0] * b.p[2], a.p[0] * b.p[1] - a.p[1] * b.p[0], 1.0f);
}

inline vec4 normalize(vec4 a)
{
    float length = sqrt(a.p[0] * a.p[0] + a.p[1] * a.p[1] + a.p[2] * a.p[2]);
    return vec4(a.p[0] / length, a.p[1] / length, a.p[2] / length, 1.0f);
}

inline float length(vec4 a)
{
    return sqrt(a.p[0] * a.p[0] + a.p[1] * a.p[1] + a.p[2] * a.p[2]);
}

inline bool compare(vec4 a, vec4 b)
{
    if (length(a - b) < 0.001f)
        return true;
    return false;
}

struct vec2
{
    float p[2];
    vec2(float x, float y)
    {
        p[0] = x;
        p[1] = y;
    }
    vec2()
    {

    }
};

struct int3
{
    int p[3];
    int3()
    {

    }
    int3(int a, int b, int c)
    {
        p[0] = a;
        p[1] = b;
        p[2] = c;
    }
};

/// Float approximate-equality comparison
template<typename T>
inline bool fequal(T a, T b, T epsilon = 0.0001){
    if (a == b) {
        // Shortcut
        return true;
    }

    const T diff = std::abs(a - b);
    if (a * b == 0) {
        // a or b or both are zero; relative error is not meaningful here
        return diff < (epsilon * epsilon);
    }

    return diff / (std::abs(a) + std::abs(b)) < epsilon;
}


#endif // UTILS_H
