#pragma once
#include "Common/OxygenMathLite.h"

namespace OxyPhysics::Utils
{
    using namespace OxygenMathLite;

    class CoordinateConverter
    {
    public:
        // 世界坐标到屏幕坐标
        static Vec2 WorldToScreen(const Vec2 &worldPos, const Vec2 &cameraPos, real zoom, const Vec2 &screenSize)
        {
            Vec2 screenCenter = screenSize * 0.5f;
            return (worldPos - cameraPos) * zoom + screenCenter;
        }

        // 屏幕坐标到世界坐标
        static Vec2 ScreenToWorld(const Vec2 &screenPos, const Vec2 &cameraPos, real zoom, const Vec2 &screenSize)
        {
            Vec2 screenCenter = screenSize * 0.5f;
            return (screenPos - screenCenter) / zoom + cameraPos;
        }

        // 将长度值从世界单位转换为屏幕像素单位
        static real WorldToScreenScale(real worldValue, real zoom)
        {
            return worldValue * zoom;
        }

        // 将屏幕像素长度转换为世界单位
        static real ScreenToWorldScale(real screenValue, real zoom)
        {
            return screenValue / zoom;
        }
    };
}
