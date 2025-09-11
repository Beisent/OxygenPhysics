#pragma once
#include <cmath>
#include <ostream>

namespace OxyPhysics
{

    namespace Constants
    {
        constexpr float PI = 3.14159265358979323846f;
        constexpr float TWO_PI = 2.0f * PI;
        constexpr float HALF_PI = 0.5f * PI;
        constexpr float DEG2RAD = PI / 180.0f;
        constexpr float RAD2DEG = 180.0f / PI;
        constexpr float EPSILON = 1e-6f;
    } // namespace Constants

    namespace MathTools
    {
        // ========== 角度和弧度的转换 ==========
        constexpr float DegToRad(float deg) { return deg * Constants::DEG2RAD; }
        constexpr float RadToDeg(float rad) { return rad * Constants::RAD2DEG; }
        constexpr float DegToRad(int deg) { return deg * Constants::DEG2RAD; }
    } // MathTools

    
    // ========== 二维向量 ==========
    struct Vec2
    {
        float x = 0.0f;
        float y = 0.0f;

        // ========== 构造函数 ==========
        constexpr Vec2() : x(0.0f), y(0.0f) {}
        constexpr Vec2(float x, float y) : x(x), y(y) {}

        // ========== 静态工厂方法 ==========
        static Vec2 Zero() { return {0.0f, 0.0f}; }
        static Vec2 One() { return {1.0f, 1.0f}; }
        static Vec2 Up() { return {0.0f, 1.0f}; }
        static Vec2 Down() { return {0.0f, -1.0f}; }
        static Vec2 Left() { return {-1.0f, 0.0f}; }
        static Vec2 Right() { return {1.0f, 0.0f}; }

        // ========== 运算符重载 ==========
        Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }
        Vec2 operator-(const Vec2 &other) const { return {x - other.x, y - other.y}; }
        Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
        Vec2 operator/(float scalar) const { return {x / scalar, y / scalar}; }

        Vec2 &operator+=(const Vec2 &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }
        Vec2 &operator-=(const Vec2 &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
        Vec2 &operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        Vec2 &operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        // ========== 数学函数 ==========
        float dot(const Vec2 &other) const { return x * other.x + y * other.y; }
        float cross(const Vec2 &other) const { return x * other.y - y * other.x; }

        Vec2 perpendicular() const { return {-y, x}; }
        Vec2 rotate(float radians) const
        {
            float c = std::cos(radians);
            float s = std::sin(radians);
            return {x * c - y * s, x * s + y * c};
        }

        float length() const { return std::sqrt(x * x + y * y); }
        float lengthSquared() const { return x * x + y * y; }

        Vec2 normalize() const
        {
            float len = length();
            if (len < Constants::EPSILON)
                return {0, 0};
            return {x / len, y / len};
        }

        void normalizeSelf()
        {
            float len = length();
            if (len < Constants::EPSILON)
            {
                x = y = 0;
                return;
            }
            x /= len;
            y /= len;
        }

        // ========== 输出支持 ==========
        friend std::ostream &operator<<(std::ostream &os, const Vec2 &vec)
        {
            os << "Vec2(" << vec.x << ", " << vec.y << ")";
            return os;
        }
    };

    // ========== 全局运算符 ==========
    inline Vec2 operator*(float scalar, const Vec2 &vec)
    {
        return vec * scalar;
    }

} // namespace OxyPhysics