#include "MathUtils.h"

#include <algorithm>

namespace MathUtils {
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float clamp01(float t) {
    return std::clamp(t, 0.0f, 1.0f);
}
}  // namespace MathUtils
