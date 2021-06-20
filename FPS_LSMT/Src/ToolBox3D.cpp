#include "ToolBox3D.h"

using namespace Core::Maths;



bool ToolBox3D::operator!=(const vec3& a, const vec3& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

#pragma region Vector3Calculs

vec3 ToolBox3D::pointVector(const vec3& a, const vec3& b)
{
    return { b.x - a.x, b.y - a.y, b.z - a.z };
}

float ToolBox3D::Dot(const vec3& a, const vec3& b)
{
	float   Dot_product = a.x * b.x + a.y * b.y + a.z * b.z;
	return  Dot_product;
}

float ToolBox3D::Sign(const float a)
{
    if (a < 0)
        return -1;

    return 1;
}

vec3 ToolBox3D::Cross(const vec3& a, const vec3& b)
{
	vec3 c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z; 
	c.z = a.x * b.y - a.y * b.x;

	return c;
}

vec3 ToolBox3D::Normalize(const vec3& v)
{
    vec3 result = v;

    float length, ilength;
    length = Magnitude(v);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;

    result.x *= ilength;
    result.y *= ilength;
    result.z *= ilength;

    return result;
}

vec3 ToolBox3D::Normal(const vec3& v)
{
	vec3 vector = { -v.y, v.x, v.z };
	return vector;
}

float ToolBox3D::Magnitude(const vec3& v)
{
	return sqrtf(Dot(v, v));
}

float ToolBox3D::sqrMag(const vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float ToolBox3D::Distance(const vec3& v1, const vec3& v2)
{
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    float result = sqrtf(dx * dx + dy * dy + dz * dz);
    return result;
}

float ToolBox3D::Clamp(float value, float min, float max)
{
	const float res = value < min ? min : value;
	return res > max ? max : res;
}

float Core::Maths::ToolBox3D::lerp(float a, float b, float t)
{
    return (1 - t) * a + t * b;
}

float Core::Maths::ToolBox3D::constantLerp(float a, float b, float t)
{
    float to = b - a;

    if (fabsf(to) < t)
    {
        return b;
    }
    else
    {
        float sign = to / (to == 0.f ? 1.f : fabsf(to));
        return a + (int)sign * t;
    }
}

vec3 ToolBox3D::PositionMatrix(mat4 mat)
{
    //return { mat.c[3].e[0], mat.c[3].e[1], mat.c[3].e[2] };
    return { mat.e[12], mat.e[13], mat.e[14] };
}

float ToolBox3D::ScalingFactorMatrix(mat4 mat)
{
    //return sqrt(mat.c[0].e[0] * mat.c[0].e[0] + mat.c[1].e[0] * mat.c[1].e[0] + mat.c[2].e[0] * mat.c[2].e[0]);
    return sqrt(mat.e[0] * mat.e[0] + mat.e[5] * mat.e[5] + mat.e[9] * mat.e[9]);
}

vec3 ToolBox3D::RotationMatrix(mat4 mat)
{
    float Ry = asin(mat.e[2]);
    float Rx;
    float Rz;

    if (Ry < PI / 2)
    {
        if (Ry > -PI / 2)
        {
            Rx = atan(-mat.e[6] / mat.e[10]);
            Rz = atan(-mat.e[1] / mat.e[0]);
        }
        else
        {
            Rx = -atan(mat.e[6] / mat.e[10]);
            Rz = 0;
        }
    }
    else
    {
        Rx = atan(mat.e[6] / mat.e[10]);
        Rz = 0;
    }

    return vec3{ Rx, Ry, Rz } * ScalingFactorMatrix(mat);
   //mat4 test = {};
   //test.e[0] = mat.e[0];
   //test.e[1] = mat.e[4];
   //test.e[2] = mat.e[8];
   //test.e[3] = 0;
   //test.e[4] = mat.e[1];
   //test.e[5] = mat.e[5];
   //test.e[6] = mat.e[9];
   //test.e[7] = 0;
   //test.e[8] = mat.e[2];
   //test.e[9] = mat.e[6];
   //test.e[10] = mat.e[10];
   //test.e[11] = 0;
   //test.e[12] = 0;
   //test.e[13] = 0;
   //test.e[14] = 0;
   //test.e[15] = 1;
   //
   //return (test
   //*vec4{ (float)(1.0 / ScalingFactorMatrix(mat)), (float)(1.0 / ScalingFactorMatrix(mat)),(float)(1.0 / ScalingFactorMatrix(mat)), 0 }).xyz;


    //return (mat4{ mat.c[0].e[0], mat.c[0].e[1], mat.c[0].e[2], 0,
    //        mat.c[1].e[0], mat.c[1].e[1], mat.c[2].e[1], 0,
    //        mat.c[0].e[2], mat.c[1].e[2], mat.c[2].e[2], 0,
    //        0            , 0            , 0            , 1 }
    //*vec4{ (float)(1.0 / ScalingFactorMatrix(mat)), (float)(1.0 / ScalingFactorMatrix(mat)),(float)(1.0 / ScalingFactorMatrix(mat)), 0 }).xyz;

}

#pragma region QuaternionCalculs

Quaternion ToolBox3D::QuaternionAdd(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w };
    return result;
}

// Add Quaternion and float value
Quaternion ToolBox3D::QuaternionAddValue(Quaternion q, float add)
{
    Quaternion result = { q.x + add, q.y + add, q.z + add, q.w + add };
    return result;
}

// Subtract two quaternions
Quaternion ToolBox3D::QuaternionSubtract(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w };
    return result;
}

// Subtract Quaternion and float value
Quaternion ToolBox3D::QuaternionSubtractValue(Quaternion q, float sub)
{
    Quaternion result = { q.x - sub, q.y - sub, q.z - sub, q.w - sub };
    return result;
}

// Returns identity Quaternion
Quaternion ToolBox3D::QuaternionIdentity(void)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };
    return result;
}

// Computes the length of a Quaternion
float ToolBox3D::QuaternionLength(Quaternion q)
{
    float result = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    return result;
}

// Normalize provided Quaternion
Quaternion ToolBox3D::QuaternionNormalize(Quaternion q)
{
    Quaternion result = {};

    float length, ilength;
    length = QuaternionLength(q);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;

    result.x = q.x * ilength;
    result.y = q.y * ilength;
    result.z = q.z * ilength;
    result.w = q.w * ilength;

    return result;
}

// Invert provided Quaternion
Quaternion ToolBox3D::QuaternionInvert(Quaternion q)
{
    Quaternion result = q;
    float length = QuaternionLength(q);
    float lengthSq = length * length;

    if (lengthSq != 0.0)
    {
        float i = 1.0f / lengthSq;

        result.x *= -i;
        result.y *= -i;
        result.z *= -i;
        result.w *= i;
    }

    return result;
}

// Calculate two Quaternion multiplication
Quaternion ToolBox3D::QuaternionMultiply(Quaternion q1, Quaternion q2)
{
    Quaternion result = {};

    float qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
    float qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;

    result.x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
    result.y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
    result.z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
    result.w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

    return result;
}

// Scale Quaternion by float value
Quaternion ToolBox3D::QuaternionScale(Quaternion q, float mul)
{
    Quaternion result = {};

    float qax = q.x, qay = q.y, qaz = q.z, qaw = q.w;

    result.x = qax * mul + qaw * mul + qay * mul - qaz * mul;
    result.y = qay * mul + qaw * mul + qaz * mul - qax * mul;
    result.z = qaz * mul + qaw * mul + qax * mul - qay * mul;
    result.w = qaw * mul - qax * mul - qay * mul - qaz * mul;

    return result;
}

// Divide two quaternions
Quaternion ToolBox3D::QuaternionDivide(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x / q2.x, q1.y / q2.y, q1.z / q2.z, q1.w / q2.w };
    return result;
}

// Calculate linear interpolation between two quaternions
Quaternion ToolBox3D::QuaternionLerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = {};

    result.x = q1.x + amount * (q2.x - q1.x);
    result.y = q1.y + amount * (q2.y - q1.y);
    result.z = q1.z + amount * (q2.z - q1.z);
    result.w = q1.w + amount * (q2.w - q1.w);

    return result;
}

// Calculate slerp-optimized interpolation between two quaternions
Quaternion ToolBox3D::QuaternionNlerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = QuaternionLerp(q1, q2, amount);
    result = QuaternionNormalize(result);

    return result;
}

// Calculates spherical linear interpolation between two quaternions
Quaternion ToolBox3D::QuaternionSlerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = {};

    float cosHalfTheta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    if (cosHalfTheta < 0)
    {
        q2.x = -q2.x; q2.y = -q2.y; q2.z = -q2.z; q2.w = -q2.w;
        cosHalfTheta = -cosHalfTheta;
    }

    if (fabs(cosHalfTheta) >= 1.0f) result = q1;
    else if (cosHalfTheta > 0.95f) result = QuaternionNlerp(q1, q2, amount);
    else
    {
        float halfTheta = acosf(cosHalfTheta);
        float sinHalfTheta = sqrtf(1.0f - cosHalfTheta * cosHalfTheta);

        if (fabs(sinHalfTheta) < 0.001f)
        {
            result.x = (q1.x * 0.5f + q2.x * 0.5f);
            result.y = (q1.y * 0.5f + q2.y * 0.5f);
            result.z = (q1.z * 0.5f + q2.z * 0.5f);
            result.w = (q1.w * 0.5f + q2.w * 0.5f);
        }
        else
        {
            float ratioA = sinf((1 - amount) * halfTheta) / sinHalfTheta;
            float ratioB = sinf(amount * halfTheta) / sinHalfTheta;

            result.x = (q1.x * ratioA + q2.x * ratioB);
            result.y = (q1.y * ratioA + q2.y * ratioB);
            result.z = (q1.z * ratioA + q2.z * ratioB);
            result.w = (q1.w * ratioA + q2.w * ratioB);
        }
    }

    return result;
}

// Calculate Quaternion based on the rotation from one vector to another
Quaternion ToolBox3D::QuaternionFromVector3ToVector3(vec3 from, vec3 to)
{
    Quaternion result = {};

    float cos2Theta = Dot(from, to);
    vec3 CrossResult = Cross(from, to);

    result.x = CrossResult.x;
    result.y = CrossResult.y;
    result.z = CrossResult.z;
    result.w = 1.0f + cos2Theta;     // NOTE: Added QuaternioIdentity()

    // Normalize to essentially nlerp the original and identity to 0.5
    result = QuaternionNormalize(result);

    // Above lines are equivalent to:
    //Quaternion result = QuaternionNlerp(q, QuaternionIdentity(), 0.5f);

    return result;
}

// Returns a Quaternion for a given rotation matrix
Quaternion ToolBox3D::QuaternionFromMatrix(mat4 mat)
{
    Quaternion result = {};

    
    float s = sqrtf(1.0f + mat.e[0] - mat.e[5] - mat.e[10]) * 2;
    if (s == 0)
        return QuaternionIdentity();

    if ((mat.e[0] > mat.e[5]) && (mat.e[0] > mat.e[10]))
    {

        result.x = 0.25f * s;
        result.y = (mat.e[4] + mat.e[1]) / s;
        result.z = (mat.e[2] + mat.e[8]) / s;
        result.w = (mat.e[9] - mat.e[6]) / s;
    }
    else if (mat.e[5] > mat.e[10])
    {
        result.x = (mat.e[4] + mat.e[1]) / s;
        result.y = 0.25f * s;
        result.z = (mat.e[9] + mat.e[6]) / s;
        result.w = (mat.e[2] - mat.e[8]) / s;
    }
    else
    {
        result.x = (mat.e[2] + mat.e[8]) / s;
        result.y = (mat.e[9] + mat.e[6]) / s;
        result.z = 0.25f * s;
        result.w = (mat.e[4] - mat.e[1]) / s;
    }

    return result;
}
/*
// Returns a matrix for a given Quaternion
Matrix QuaternionToMatrix(Quaternion q)
{
    Matrix result = MatrixIdentity();

    float a2 = 2 * (q.x * q.x), b2 = 2 * (q.y * q.y), c2 = 2 * (q.z * q.z); //, d2=2*(q.w*q.w);

    float ab = 2 * (q.x * q.y), ac = 2 * (q.x * q.z), bc = 2 * (q.y * q.z);
    float ad = 2 * (q.x * q.w), bd = 2 * (q.y * q.w), cd = 2 * (q.z * q.w);

    result.m0 = 1 - b2 - c2;
    result.m1 = ab - cd;
    result.m2 = ac + bd;

    result.m4 = ab + cd;
    result.m5 = 1 - a2 - c2;
    result.m6 = bc - ad;

    result.m8 = ac - bd;
    result.m9 = bc + ad;
    result.m10 = 1 - a2 - b2;

    return result;
}*/

// Returns rotation Quaternion for an angle and axis
// NOTE: angle must be provided in radians
Quaternion ToolBox3D::QuaternionFromAxisAngle(vec3 axis, float angle)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (Magnitude(axis) != 0.0f)

        angle *= 0.5f;

    axis = Normalize(axis);

    float sinres = sinf(angle);
    float cosres = cosf(angle);

    result.x = axis.x * sinres;
    result.y = axis.y * sinres;
    result.z = axis.z * sinres;
    result.w = cosres;

    result = QuaternionNormalize(result);

    return result;
}

// Returns the rotation angle and axis for a given Quaternion
void ToolBox3D::QuaternionToAxisAngle(Quaternion q, vec3* outAxis, float* outAngle)
{
    if (fabs(q.w) > 1.0f) q = QuaternionNormalize(q);

    vec3 resAxis = { 0.0f, 0.0f, 0.0f };
    float resAngle = 2.0f * acosf(q.w);
    float den = sqrtf(1.0f - q.w * q.w);

    if (den > 0.0001f)
    {
        resAxis.x = q.x / den;
        resAxis.y = q.y / den;
        resAxis.z = q.z / den;
    }
    else
    {
        // This occurs when the angle is zero.
        // Not a problem: just set an arbitrary Normalized axis.
        resAxis.x = 1.0f;
    }

    *outAxis = resAxis;
    *outAngle = resAngle;
}

// Returns the Quaternion equivalent to Euler angles
// NOTE: Rotation order is ZYX
// NOTE: angles are in rad
Quaternion ToolBox3D::QuaternionFromEuler(float pitch, float yaw, float roll)
{
    Quaternion q = {};

    float x0 = cosf(pitch * 0.5f);
    float x1 = sinf(pitch * 0.5f);
    float y0 = cosf(yaw * 0.5f);
    float y1 = sinf(yaw * 0.5f);
    float z0 = cosf(roll * 0.5f);
    float z1 = sinf(roll * 0.5f);

    q.x = x1 * y0 * z0 - x0 * y1 * z1;
    q.y = x0 * y1 * z0 + x1 * y0 * z1;
    q.z = x0 * y0 * z1 - x1 * y1 * z0;
    q.w = x0 * y0 * z0 + x1 * y1 * z1;

    return q;
}

// Return the Euler angles equivalent to Quaternion (roll, pitch, yaw)
// NOTE: Angles are returned in a Vector3 struct in degrees
vec3 ToolBox3D::QuaternionToEuler(Quaternion q)
{
    vec3 result = { 0 };

    // roll (x-axis rotation)
    float x0 = 2.0f * (q.w * q.x + q.y * q.z);
    float x1 = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    result.x = atan2f(x0, x1) * RAD2DEG;

    // pitch (y-axis rotation)
    float y0 = 2.0f * (q.w * q.y - q.z * q.x);
    y0 = y0 > 1.0f ? 1.0f : y0;
    y0 = y0 < -1.0f ? -1.0f : y0;
    result.y = asinf(y0) * RAD2DEG;

    // yaw (z-axis rotation)
    float z0 = 2.0f * (q.w * q.z + q.x * q.y);
    float z1 = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    result.z = atan2f(z0, z1) * RAD2DEG;

    return result;
}

vec3 ToolBox3D::Vector3RotateByQuaternion(vec3 v, Quaternion q)
{
    vec3 result = { 0 };

    result.x = v.x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + v.y * (2 * q.x * q.y - 2 * q.w * q.z) + v.z * (2 * q.x * q.z + 2 * q.w * q.y);
    result.y = v.x * (2 * q.w * q.z + 2 * q.x * q.y) + v.y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + v.z * (-2 * q.w * q.x + 2 * q.y * q.z);
    result.z = v.x * (-2 * q.w * q.y + 2 * q.x * q.z) + v.y * (2 * q.w * q.x + 2 * q.y * q.z) + v.z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);

    return result;
}
#pragma endregion

ToolBox3D::Range ToolBox3D::RangeUpdate(const ToolBox3D::Range& range, const float& x)
{
    return { Maths::min(range.min, x), Maths::max(range.max, x) };
}

vec3 ToolBox3D::GlobalToLocalVector(const vec3& i, const vec3& j, const vec3& k, const vec3& pt)
{
    return { Dot(pt, i),Dot(pt, j), Dot(pt, k) };
}

vec3 ToolBox3D::GlobalToLocalPosition(const vec3& origin, const vec3& i, const vec3& j, const vec3& k, const vec3& pt)
{
    return GlobalToLocalVector(i, j, k, pt - origin);
}

mat4 ToolBox3D::mat4x4::translate(vec3 position)
{
    return
    {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        position.x, position.y, position.z, 1.f,
    };
}

mat4 ToolBox3D::mat4x4::scale(vec3 s)
{
    return
    {
        s.x, 0.f, 0.f, 0.f,
        0.f, s.y, 0.f, 0.f,
        0.f, 0.f, s.z, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}


mat4 ToolBox3D::mat4x4::rotateY(float angleRadians)
{
    return
    {
        cosf(angleRadians), 0.f, -sinf(angleRadians), 0.f,
        0.f, 1.f, 0.f, 0.f,
        sinf(angleRadians), 0.f, cosf(angleRadians), 0.f,
        0.f, 0.f, 0.f, 1.f,

    };
}

mat4 ToolBox3D::mat4x4::rotateZ(float angleRadians)
{
    return
    {
        cosf(angleRadians), -sinf(angleRadians), 0.f, 0.f,
        sinf(angleRadians), cosf(angleRadians), 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f,

    };
}

mat4 ToolBox3D::mat4x4::rotateX(float angleRadians)
{
    return
    {
        1.f, 0.f, 0.f, 0.f,
        0.f, cosf(angleRadians), sinf(angleRadians), 0.f,
        0.f, -sinf(angleRadians), cosf(angleRadians), 0.f,
        0.f, 0.f, 0.f, 1.f,

    };
}

mat4 ToolBox3D::mat4x4::frustum(float left, float right, float bottom, float top, float near, float far)
{
    return
    {
        (near * 2.f) / (right - left),   0.f,                              0.f,                               0.f,
        0.f,                             (near * 2.f) / (top - bottom),    0.f,                               0.f,
        (right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near),      -1.f,
        0.f,                             0.f,                             -(far * near * 2.f) / (far - near), 0.f
    };
}

mat4 ToolBox3D::mat4x4::perspective(float fovY, float aspect, float near, float far)
{
    float top = near * tanf(fovY / 2.f);
    float right = top * aspect;
    return frustum(-right, right, -top, top, near, far);
}

mat4 ToolBox3D::mat4x4::frustumOrtho(float left, float right, float bottom, float top, float near, float far)
{
    return
    {
        2.f / (right - left),   0.f,					0.f,                               0.f,
        0.f,					2 / (top - bottom),		0.f,					 0.f,
        0.f,					0.f,					-2 / (far - near),						 0.f,
        -(right + left) / (right - left),  -(top + bottom) / (top - bottom),  -(far + near) / (far - near), 1.f
    };
}

mat4 ToolBox3D::mat4x4::ortho(float fovY, float aspect, float near, float far)
{
    float top = fovY;
    float right = top * aspect;
    return frustumOrtho(-right, right, -top, top, near, far);
}

mat4 ToolBox3D::mat4x4::identity()
{
    return {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}
