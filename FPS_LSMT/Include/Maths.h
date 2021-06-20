#pragma once

#include <cmath>

namespace Core
{
    namespace Maths
    {
        const float TAU = 6.283185307179586476925f;

        inline int min(int x, int y) { return (x < y) ? x : y; };
        inline int max(int x, int y) { return (x > y) ? x : y; };
        inline float min(float x, float y) { return (x < y) ? x : y; };
        inline float max(float x, float y) { return (x > y) ? x : y; };

        inline float cos(float x) { return cosf(x); };
        inline float sin(float x) { return sinf(x); };
        inline float tan(float x) { return tanf(x); };

        #ifndef PI
            #define PI float(3.14f)
        #endif

        #ifndef DEG2RAD
            #define DEG2RAD float(PI/180.0f)
        #endif
        
        #ifndef RAD2DEG
            #define RAD2DEG float(180.0f/PI)
        #endif

        union vec2
        {
            float e[2];
            struct { float x; float y; };
        };

        union vec3
        {
            float e[3];
            struct { float x; float y; float z; };
            vec2 xy;
        };

        union int3
        {
            int3() = default;
            int3(int x, int y, int z) : x(x), y(y), z(z) {}
            int e[3];
            struct { int x; int y; int z; };
        };

        union vec4
        {
            vec4() = default;
            vec4(float x, float y, float z, float w)
                : x(x), y(y), z(z), w(w)
            {}

            vec4(vec3 xyz, float w)
                : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
            {}

            float e[4];
            struct { float x; float y; float z; float w; };
            struct { float r; float g; float b; float a; };
            vec3 xyz;
            vec3 rgb;
            vec2 xy;
        };

        typedef vec4 Quaternion;

        union mat4
        {
            float e[16];
            vec4 c[4];
        };

        void translate(vec3& dest, const vec3& t);

        #pragma region Operator

        inline vec4 operator*(const mat4& m, const vec4& v)
        {
            vec4 r;

            r.x = v.x * m.c[0].e[0] + v.y * m.c[1].e[0] + v.z * m.c[2].e[0] + v.w * m.c[3].e[0];
            r.y = v.x * m.c[0].e[1] + v.y * m.c[1].e[1] + v.z * m.c[2].e[1] + v.w * m.c[3].e[1];
            r.z = v.x * m.c[0].e[2] + v.y * m.c[1].e[2] + v.z * m.c[2].e[2] + v.w * m.c[3].e[2];
            r.w = v.x * m.c[0].e[3] + v.y * m.c[1].e[3] + v.z * m.c[2].e[3] + v.w * m.c[3].e[3];

            return r;
        }

        inline vec2 operator-(const vec2& pos)
        {
            return { -pos.x, -pos.y};
        }

        inline vec2 operator-(const vec2& a, const vec2 b)
        {
            vec2 result = { a.x - b.x, a.y - b.y };
            return result;
        }

        inline vec2 operator+(const vec2& a, const vec2 b)
        {
            vec2 result = { a.x + b.x, a.y + b.y };
            return result;
        }

        inline vec3 operator-(const vec3& pos)
        {
            return { -pos.x, -pos.y, -pos.z };
        }

        inline vec3 operator-(const vec3& a, const vec3 b)
        {
            vec3 result = { a.x - b.x, a.y - b.y, a.z - b.z };
            return result;
        }

        inline mat4 operator*(const mat4& a, const mat4& b)
        {
            mat4 pos = {};

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        pos.c[i].e[j] += a.c[k].e[j] * b.c[i].e[k];
                    }
                }
            }

            return pos;
        }

        inline mat4& operator*=(mat4& a, const mat4& b)
        {
            a = a * b;
            return a;
        }

        inline vec3 operator/(const vec3& v, const float& w)
        {
            return {
            v.x / w,
            v.y / w,
            v.z / w,
            };
        }

        inline vec3 operator*(const vec3& v, const float& a)
        {
            return {
            v.x * a,
            v.y * a,
            v.z * a,
            };
        }

        inline vec3 operator*(const float& a, const vec3& v)
        {
            return {
            v.x * a,
            v.y * a,
            v.z * a,
            };
        }

        inline vec3 operator*(const vec3& a, const vec3& b)
        {
            return {
            a.x * b.x,
            a.y * b.y,
            a.z * b.z,
            };
        }

        inline vec3& operator*=(vec3& a, const vec3& b)
        {
            a = a * b;
            return a;
        }

        inline vec3& operator*=(vec3& v, const float& a)
        {
            v = a * v;
            return v;
        }

        inline vec3 operator+(const vec3& v1, const float& f)
        {
            return {
                v1.x + f,
                v1.y + f,
                v1.z + f,
            };
        }

        inline vec3 operator+(const vec3& v1, const vec3& v2)
        {
            return {
                v1.x + v2.x,
                v1.y + v2.y,
                v1.z + v2.z,
            };
        }

        inline vec3& operator+=(vec3& v1, const vec3& v2)
        {
            v1 = v1 + v2;
            return v1;
        }

        inline vec3& operator-=(vec3& v1, const vec3& v2)
        {
            v1 = v1 - v2;
            return v1;
        }

        inline vec4 operator*(const vec4& v, const float& a)
        {
            return {
            v.x * a,
            v.y * a,
            v.z * a,
            v.w * a,
            };
        }

        inline vec4 operator+(const vec4& v1, const vec4& v2)
        {
            return {
                v1.x + v2.x,
                v1.y + v2.y,
                v1.z + v2.z,
                v1.w + v2.w,
            };
        }

        inline vec4 operator*(const vec4& v1, const vec4& v2)
        {
            return {
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z,
            v1.w * v2.w,
            };
        }

        inline vec4 operator*(const vec3& v1, const vec4& v2)
        {
            return {
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z,
            v2.w,
            };
        }

        inline vec4 operator*(const vec4& v1, const vec3& v2)
        {
            return {
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z,
            v1.w,
            };
        }
        #pragma endregion
    }
}