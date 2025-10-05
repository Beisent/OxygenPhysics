#pragma once
#include <random>
#include <cstdint>
#include "Common/OxygenMathLite.h"

namespace OxyPhysics::Utils
{
    using namespace OxygenMathLite;

    struct Color
    {
        float r;
        float g;
        float b;
        float a;

        Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
            : r(r), g(g), b(b), a(a) {}
    };

    class ColorGenerator
    {
    public:
        static Color RandomColor(float alpha = 1.0f)
        {
            static std::mt19937 rng(std::random_device{}());
            static std::uniform_real_distribution<float> dist(0.2f, 1.0f);
            return Color(dist(rng), dist(rng), dist(rng), alpha);
        }

        static Color FromHue(float hue, float alpha = 1.0f)
        {
            hue = fmodf(hue, 360.0f) / 60.0f;
            float c = 1.0f;
            float x = 1.0f - fabsf(fmodf(hue, 2.0f) - 1.0f);
            float r = 0, g = 0, b = 0;
            if (hue < 1) { r = c; g = x; }
            else if (hue < 2) { r = x; g = c; }
            else if (hue < 3) { g = c; b = x; }
            else if (hue < 4) { g = x; b = c; }
            else if (hue < 5) { r = x; b = c; }
            else { r = c; b = x; }
            return Color(r, g, b, alpha);
        }
    };
}
