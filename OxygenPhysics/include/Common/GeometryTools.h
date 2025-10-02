#pragma once
#include "OxygenMathLite.h"
#include <vector>
#include <limits>

namespace OxygenMathLite
{
    namespace GeometryTools
    {
        // -------------------------
        // 距离与最近点工具
        // -------------------------

        inline Vec2 closestPointOnSegment(const Vec2 &p, const Vec2 &a, const Vec2 &b)
        {
            Vec2 AB = b - a;
            real abLenSqr = AB.lengthSquared();

            if (abLenSqr <= Constants::Epsilon)
            {
                return a;
            }

            Vec2 AP = p - a;
            real t = AP.dot(AB) / abLenSqr;
            t = MathTools::Clamp(t, real(0), real(1));
            return a + t * AB;
        }
        // 点到三角形的最近点 (EPA 用到)
        inline Vec2 closestPointOnTriangle(const Vec2 &p, const Vec2 &a, const Vec2 &b, const Vec2 &c)
        {
            Vec2 ab = b - a;
            Vec2 ac = c - a;
            Vec2 ap = p - a;

            real d1 = ab.dot(ap);
            real d2 = ac.dot(ap);
            if (d1 <= 0 && d2 <= 0)
                return a;

            Vec2 bp = p - b;
            real d3 = ab.dot(bp);
            real d4 = ac.dot(bp);
            if (d3 >= 0 && d4 <= d3)
                return b;

            real vc = d1 * d4 - d3 * d2;
            if (vc <= 0 && d1 >= 0 && d3 <= 0)
            {
                real v = d1 / (d1 - d3);
                return a + v * ab;
            }

            Vec2 cp = p - c;
            real d5 = ab.dot(cp);
            real d6 = ac.dot(cp);
            if (d6 >= 0 && d5 <= d6)
                return c;

            real vb = d5 * d2 - d1 * d6;
            if (vb <= 0 && d2 >= 0 && d6 <= 0)
            {
                real w = d2 / (d2 - d6);
                return a + w * ac;
            }

            real va = d3 * d6 - d5 * d4;
            if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0)
            {
                real w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
                return b + w * (c - b);
            }

            return p; // 在三角形内部
        }
        inline real distancePointToPoint(const Vec2 &p1, const Vec2 &p2)
        {
            return (p1 - p2).length();
        }

        inline real distanceSquaredPointToPoint(const Vec2 &p1, const Vec2 &p2)
        {
            return (p1 - p2).lengthSquared();
        }

        inline real distancePointToSegment(const Vec2 &p, const Vec2 &a, const Vec2 &b)
        {
            Vec2 closest = closestPointOnSegment(p, a, b);
            return distancePointToPoint(p, closest);
        }

        inline real distanceSquaredPointToSegment(const Vec2 &p, const Vec2 &a, const Vec2 &b)
        {
            Vec2 closest = closestPointOnSegment(p, a, b);
            return distanceSquaredPointToPoint(p, closest);
        }

        // -------------------------
        // 支持函数 (GJK 用)
        // -------------------------

        // 多边形支持函数
        inline Vec2 supportPolygon(const std::vector<Vec2> &vertices, const Vec2 &dir)
        {
            real maxDot = -std::numeric_limits<real>::infinity();
            Vec2 best;
            for (const auto &v : vertices)
            {
                real d = v.dot(dir);
                if (d > maxDot)
                {
                    maxDot = d;
                    best = v;
                }
            }
            return best;
        }

        // 圆形支持函数
        inline Vec2 supportCircle(const Vec2 &center, real radius, const Vec2 &dir)
        {
            real lenSqr = dir.lengthSquared();
            if (lenSqr <= Constants::Epsilon)
            {
                return center; // 方向接近零，返回圆心
            }

            Vec2 ndir = dir / std::sqrt(lenSqr);
            return center + ndir * radius;
        }

        // -------------------------
        // 法线工具
        // -------------------------

        inline Vec2 leftNormal(const Vec2 &v)
        {
            return Vec2(-v.y, v.x);
        }

        inline Vec2 rightNormal(const Vec2 &v)
        {
            return Vec2(v.y, -v.x);
        }

        // -------------------------
        // EPA 辅助：最近边查找
        // -------------------------

        struct Edge
        {
            Vec2 normal;   // 边的外法线
            real distance; // 边到原点的距离
            int index;     // 边起点索引
        };

        inline Edge findClosestEdge(const std::vector<Vec2> &poly)
        {
            Edge closest;
            closest.distance = std::numeric_limits<real>::infinity();
            closest.index = -1;

            for (int i = 0; i < (int)poly.size(); ++i)
            {
                int j = (i + 1) % poly.size();
                Vec2 a = poly[i];
                Vec2 b = poly[j];

                Vec2 e = b - a;
                Vec2 n = leftNormal(e).normalize();

                // 确保法线朝向原点
                if (n.dot(a) > 0)
                {
                    n = -n;
                }

                real d = n.dot(a); // 边到原点的距离

                if (d < closest.distance)
                {
                    closest.distance = d;
                    closest.normal = n;
                    closest.index = i;
                }
            }

            return closest;
        }
    }
}
