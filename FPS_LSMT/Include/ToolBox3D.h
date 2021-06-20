#pragma once

#include "Maths.h"

namespace Core
{
	namespace Maths
	{
		namespace ToolBox3D
		{
            bool	operator!=(const vec3& a, const vec3& b);
       

            #pragma region Vector3Calculs

            vec3 pointVector(const vec3& a, const vec3& b);
			float Dot(const vec3& a, const vec3& b);
			float Magnitude(const vec3& v);
            float sqrMag(const vec3& v);
			vec3 Cross(const vec3& a, const vec3& b);
			vec3 Normalize(const vec3& v);
			vec3 Normal(const vec3& v);
            float Distance(const vec3& v1, const vec3& v2);
            vec3 Vector3RotateByQuaternion(vec3 v, Quaternion q);
            float Clamp(float value, float min, float max);
            float lerp(float a, float b, float t);
            float constantLerp(float a, float b, float t);
            vec3 PositionMatrix(mat4 mat);
            float ScalingFactorMatrix(mat4 mat);
            vec3 RotationMatrix(mat4 mat);
            float Sign(const float a);

            namespace mat4x4
            {
                mat4 translate(vec3 position);
                mat4 scale(vec3 s);
                mat4 rotateY(float angleRadians);
                mat4 rotateX(float angleRadians);
                mat4 rotateZ(float angleRadians);
                mat4 frustum(float left, float right, float bottom, float top, float near, float far);
                mat4 frustumOrtho(float left, float right, float bottom, float top, float near, float far);
                mat4 ortho(float fovY, float aspect, float near, float far);
                mat4 perspective(float fovY, float aspect, float near, float far);
                mat4 identity();
            }

            #pragma endregion

            #pragma region QuaternionCalculs

            Quaternion QuaternionAdd(Quaternion q1, Quaternion q2);

            // Add Quaternion and float value
            Quaternion QuaternionAddValue(Quaternion q, float add);

            // Subtract two quaternions
            Quaternion QuaternionSubtract(Quaternion q1, Quaternion q2);

            // Subtract Quaternion and float value
            Quaternion QuaternionSubtractValue(Quaternion q, float sub);

            // Returns identity Quaternion
            Quaternion QuaternionIdentity(void);

            // Computes the length of a Quaternion
            float QuaternionLength(Quaternion q);

            // Normalize provided Quaternion
            Quaternion QuaternionNormalize(Quaternion q);

            // Invert provided Quaternion
            Quaternion QuaternionInvert(Quaternion q);

            // Calculate two Quaternion multiplication
            Quaternion QuaternionMultiply(Quaternion q1, Quaternion q2);

            // Scale Quaternion by float value
            Quaternion QuaternionScale(Quaternion q, float mul);

            // Divide two quaternions
            Quaternion QuaternionDivide(Quaternion q1, Quaternion q2);

            // Calculate linear interpolation between two quaternions
            Quaternion QuaternionLerp(Quaternion q1, Quaternion q2, float amount);

            // Calculate slerp-optimized interpolation between two quaternions
            Quaternion QuaternionNlerp(Quaternion q1, Quaternion q2, float amount);

            // Calculates spherical linear interpolation between two quaternions
            Quaternion QuaternionSlerp(Quaternion q1, Quaternion q2, float amount);

            // Calculate Quaternion based on the rotation from one vector to another
            Quaternion QuaternionFromVector3ToVector3(vec3 from, vec3 to);

            // Returns rotation Quaternion for an angle and axis
            // NOTE: angle must be provided in radians
            Quaternion QuaternionFromAxisAngle(vec3 axis, float angle);

            // Returns the rotation angle and axis for a given Quaternion
            void QuaternionToAxisAngle(Quaternion q, vec3* outAxis, float* outAngle);

            // Returns the Quaternion equivalent to Euler angles
            // NOTE: Rotation order is ZYX
            Quaternion QuaternionFromEuler(float pitch, float yaw, float roll);

            // Return the Euler angles equivalent to Quaternion (roll, pitch, yaw)
            // NOTE: Angles are returned in a Vector3 struct in degrees
            vec3 QuaternionToEuler(Quaternion q);

            Quaternion QuaternionFromMatrix(mat4 mat);

            #pragma endregion

            class Range
            {
            public:
                Range(const float& x, const float& y) { min = x; max = y; };

                float min;
                float max;
            };

            Range RangeUpdate(const Range& range, const float& x);
            vec3 GlobalToLocalVector(const vec3& i, const vec3& j, const vec3& k, const vec3& pt);
            vec3 GlobalToLocalPosition(const vec3& origin, const vec3& i, const vec3& j, const vec3& k, const vec3& pt);
		}
	}
}