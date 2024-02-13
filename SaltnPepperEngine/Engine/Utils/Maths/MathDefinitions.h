#ifndef MATHDEFINITION_H
#define MATHDEFINITION_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.inl>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>
#include <array>
#include <algorithm>

namespace SaltnPepperEngine
{

	namespace Math
	{

		static const float PI = glm::pi<float>();
		static const float HALFPI = glm::half_pi<float>();

		static const float EPSILON = glm::epsilon<float>();
		static const float DEGtoRAD = PI / 180.0f;
		static const float DEGtoRAD_2 = PI / 360.0f;
		static const float RADtoDEG = 1.0f / DEGtoRAD;
		static const float RADtoDEG_2 = 1.0f / DEGtoRAD_2;


        // Renaming glm variables into more Usable format

        // ============== Vector Floats =================
        using Vector2 = glm::vec<2, float>;
        using Vector3 = glm::vec<3, float>;
        using Vector4 = glm::vec<4, float>;


        // ============= Vector Integers ===================
        using Vector2Int = glm::vec<2, int>;
        using Vector3Int = glm::vec<3, int>;
        using Vector4Int = glm::vec<4, int>;


        // ============== Matrix floats ====================
        using Matrix2 = glm::mat2;
        using Matrix2x3 = glm::mat2x3;
        using Matrix3 = glm::mat3;
        using Matrix2x4 = glm::mat2x4;
        using Matrix3x4 = glm::mat3x4;
        using Matrix4 = glm::mat4;

        // =============== Quaternion floats ====================
        using Quaternion = glm::quat;

        template<typename T>
        inline constexpr T Radians(const T& degrees)
        {
            return glm::radians(degrees);
        }

        template<typename T>
        inline constexpr T Degrees(const T& radians)
        {
            return glm::degrees(radians);
        }


        // =============== Vector Functionality =======================

        template<typename Vector> 
        inline float Dot(const Vector& v1, const Vector& v2)
        {
            return glm::dot(v1, v2);
        }

        // Calculate the Cross product of two vectors, returns a vector that is perpendicular to both given vectors
        inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
        {
            return glm::cross(v1, v2);
        }

        template<typename Vector>
        inline Vector Normalize(const Vector& value)
        {
            return glm::normalize(value);
        }


        template<typename Vector>
        inline auto Length(const Vector& value)
        {
            return glm::length(value);
        }

        template<typename Vector>
        inline auto LengthSquared(const Vector& value)
        {
            return glm::length2(value);
        }

        template<typename Vector>
        inline auto Distance(const Vector& v1, const Vector& v2)
        {
            return glm::distance(v1, v2);
        }

        template<typename Vector>
        inline auto DistanceSquared(const Vector& v1, const Vector& v2)
        {
            return glm::distance2(v1, v2);
        }



        // The Direction Gets auto Normalized
        inline Quaternion LookAtRotation(const Vector3& direction, const Vector3& up)
        {
            return glm::quatLookAt(Normalize(direction), up);
        }

        inline Matrix4 GetViewMatrix(const Vector3& eye, const Vector3& center, const Vector3& up)
        {
            return glm::lookAt(eye, center, up);
        }

        inline Matrix4 GetPerspectiveMatrix(float fov, float aspect, float znear, float zfar)
        {
            return glm::perspective(Radians(fov), aspect, znear, zfar);
        }

        inline Matrix4 MakeFrustrumMatrix(float left, float right, float bottom, float top, float znear, float zfar)
        {
            return glm::frustum(left, right, bottom, top, znear, zfar);
        }

        inline Matrix4 GetReversePerspective(float fov, float aspect, float znear, float zfar)
        {
            if (fov > 120.0f) { fov = 120.0f; }
            else if (fov < 0.10f) { fov = 0.10f; }

            float const tanHalfFov = std::tan(fov / 2.0f);
            float d = 1 / tanHalfFov;

            Matrix4 reversePerspective(
                d / aspect, 0.0f, 0.0f, 0.0f,
                0.0f, d, 0.0f, 0.0f,
                0.0f, 0.0f, -1.0f, 0.0f,


                0.0f, znear, 0.0f, 0.0f);

            return reversePerspective;
        }

        inline Matrix4 GetOrthographicMatrix(float left, float right, float bottom, float top, float znear, float zfar)
        {
            return glm::ortho(left, right, bottom, top, znear, zfar);
        }
        
        
        inline Matrix3 GetRotationMatrix(const Vector3& angles)
        {
            return glm::yawPitchRoll(angles.y, angles.x, angles.z);
        }
        

        inline Vector3 GetScaleFromMatrix(const Matrix4& transform)
        {
            Vector3 scale = Vector3(transform[0][0], transform[1][1], transform[2][2]);
            return scale;
        }

        inline Vector3 GetRotationFromMatrix(const Matrix4& transform)
        {
            Vector3 rotation = glm::eulerAngles(glm::quat_cast(transform));
            return rotation;
        }

        inline Vector2 WorldToScreen(const Vector3& worldPos, const Matrix4& mvp, float width, float height, float winPosX, float winPosY)
        {
            Vector4 trans = mvp * Vector4(worldPos, 1.0f);
            trans *= 0.5f / trans.w;
            trans += Vector4(0.5f, 0.5f, 0.0f, 0.0f);
            trans.y = 1.f - trans.y;
            trans.x *= width;
            trans.y *= height;
            trans.x += winPosX;
            trans.y += winPosY;
            return Vector2(trans.x, trans.y);
        }
        
        // Matrix Processing functions // Just simplified wrappers

       // Returns a transpose of the give nMatrix
        template<typename Matrix>
        inline Matrix Transpose(const Matrix& mat)
        {
            return glm::transpose(mat);
        }

        // Returns a inverse of the give nMitrix
        template<typename Matrix>
        inline Matrix Inverse(const Matrix& mat)
        {
            return glm::inverse(mat);
        }


        // ===== Tranform Stuff ====
        inline Matrix4 Translate(const Matrix4& matrix, const Vector3& vector)
        {
            return glm::translate(matrix, vector);
        }

        inline Matrix4 Scale(const Matrix4& matrix, const Vector3& vector)
        {
            return glm::scale(matrix, vector);
        }

        inline Matrix4 Scale(const Matrix4& matrix, float value)
        {
            return glm::scale(matrix, Vector3(value));
        }

        inline Matrix4 Rotate(const Matrix4& matrix, float angle, const Vector3& axis)
        {
            return glm::rotate(matrix, angle, axis);
        }

        inline Quaternion Rotate(const Quaternion& quat, float angle, const Vector3& axis)
        {
            return glm::rotate(quat, angle, axis);
        }

        inline Matrix4 QuatToMatrix(const Quaternion& quat)
        {
            return glm::toMat4(quat);
        }

        inline bool Decompose(const Matrix4& modelMat, Vector3& scale, Quaternion& orientation, Vector3& translation, Vector3& skew, Vector4& perspective)
        {
            return glm::decompose(modelMat, scale, orientation, translation, skew, perspective);
        }

        // Quaternion Stuff
        // Calculates and returns a rotation vaule from a 4X4 matrix
        inline Quaternion GetQuaternion(const Matrix4& rotationMat)
        {
            return glm::toQuat(rotationMat);
        }


       

        // Converts a Quaternion into a readable vector3 format
        inline Vector3 GetEularAnglesRadians(const Quaternion& quat)
        {
            return glm::eulerAngles(quat);           
        }

        // Converts a Quaternion into a readable vector3 format
        inline Vector3 GetEularAnglesDegrees(const Quaternion& quat)
        {
            Vector3 radianRotation = GetEularAnglesRadians(quat);

            Vector3 DegreeRotaiton = Vector3(radianRotation.x * RADtoDEG, radianRotation.y * RADtoDEG, radianRotation.z * RADtoDEG);
           

            return DegreeRotaiton;
        }
        

        // ====================== Min Max =================================

        template<typename T, typename U>
        inline constexpr decltype(std::declval<T>() + std::declval<U>()) Max(const T& v1, const U& v2)
        {
            return (v1 > v2 ? v1 : v2);
        }

        template<typename T, typename U, typename R>
        inline constexpr decltype(std::declval<T>() + std::declval<U>() + std::declval<R>()) Max(const T& v1, const U& v2, const R& v3)
        {
            return Max(Max(v1, v2), v3);
        }

        template<typename T, typename U>
        inline constexpr decltype(std::declval<T>() + std::declval<U>()) Min(const T& v1, const U& v2)
        {
            return (v1 < v2 ? v1 : v2);
        }

        template<typename T, typename U, typename R>
        inline constexpr decltype(std::declval<T>() + std::declval<U>() + std::declval<R>()) Min(const T& v1, const U& v2, const R& v3)
        {
            return Min(Min(v1, v2), v3);
        }


        template<typename T>
        inline T VectorMax(const T& v1, const T& v2)
        {
            T result(0.0f);
            for (typename T::length_type i = 0; i < result.length(); i++)
            {
                result[i] = std::max(v1[i], v2[i]);
            }
            return result;
        }

        template<typename T>
        inline T VectorMin(const T& v1, const T& v2)
        {
            T result(0.0f);
            for (typename T::length_type i = 0; i < result.length(); i++)
            {
                result[i] = std::min(v1[i], v2[i]);
            }
            return result;
        }


        template <class T>
        inline bool FloatEquals(T lhs, T rhs, T eps = EPSILON)
        {
            return lhs + eps >= rhs && lhs - eps <= rhs;
        }


        //Return sine of an angle in degrees.
        template <class T>
        inline T Sin(T angle)
        {
            return sin(angle * DEGtoRAD);
        }

        // Return cosine of an angle in degrees.
        template <class T>
        inline T Cos(T angle)
        {
            return cos(angle * DEGtoRAD);
        }

        // Return tangent of an angle in degrees.
        template <class T>
        inline T Tan(T angle)
        {
            return tan(angle * DEGtoRAD);
        }

        // Return arc sine in degrees.
        template <class T>
        inline T Asin(T x)
        {
            return RADtoDEG * asin(Clamp(x, T(-1.0), T(1.0)));
        }

        // Return arc cosine in degrees.
        template <class T>
        inline T Acos(T x)
        {
            return RADtoDEG * acos(Clamp(x, T(-1.0), T(1.0)));
        }

        // Return arc tangent in degrees.
        template <class T>
        inline T Atan(T x)
        {
            return RADtoDEG * atan(x);
        }

        // Return arc tangent of y/x in degrees.
        template <class T>
        inline T Atan2(T y, T x)
        {
            return RADtoDEG * atan2(y, x);

        }

        inline float Square(float value)
        {
            return value * value;
        }

        template<typename T>
        inline constexpr T Clamp(const T& value, const T& low, const T& high)
        {
            return glm::clamp(value, low, high);
        }

        inline float Abs(float value)
        {
            return glm::abs(value);
        }
      
		
	}

    using namespace Math;

}

#endif


