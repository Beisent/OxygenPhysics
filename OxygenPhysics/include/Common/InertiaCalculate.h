#pragma once
#include "Common/OxygenMathLite.h"
namespace OxygenMathLite
{
    class InertiaCalculate
    {
    public:
        static real calculateCircle(const real &mass, const real &radius)
        {
            return mass * radius * radius / 2;
        }
        static real calculatePolygon(const real &mass, const std::vector<Vec2> &vertices)
        {
            const Vec2 center = Vec2();
            real sum1 = 0.0, sum2 = 0.0;
            for (size_t i = 0; i < vertices.size(); ++i)
            {
                Vec2 n1 = vertices[i] - center;
                Vec2 n2 = vertices[(i + 1) % vertices.size()] - center;
                real cross = std::fabs(n1.cross(n2));
                real dot = n2.dot(n2) + n2.dot(n1) + n1.dot(n1);
                sum1 += cross * dot;
                sum2 += cross;
            }
           return (sum2 != 0) ? mass * (1.0f / 6.0f) * sum1 / sum2 : 0;
        }
        static real calculateBox(const real &mass, const real &size)
        {
            return (1.0f / 12.0f) * mass *size *size;
        }
    };
}
