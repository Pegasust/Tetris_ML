// Contains many macro definitions that are useful for developing
#pragma once
#include <cassert>

namespace Common
{
#ifndef ASSERT
#define ASSERT(expr, msg) assert(expr && msg)
#else
// There can still be evaluation of message and expression. (Not wanted)
constexpr void ASSERT(const int& expression, const char* message) {
    assert(expression && message);
}
#endif

#ifndef UNIMPLEMENTED_FEATURE
#define UNIMPLEMENTED_FEATURE_MSG(msg) ASSERT(false, msg)
#define UMIMPLEMENTED_FEATURE UNIMPLEMENTED_FEATURE_MSG("Unimplemented feature.")
#endif

#ifndef POTENTIAL_BUG_POINT
#define POTENTIAL_BUG_POINT_MSG(msg) ASSERT(true, msg)
#define POTENTIAL_BUG_POINT POTENTIAL_BUG_POINT_MSG("The point where there might be bug.");
#endif

#ifndef CAN_BE_OPTIMIZED
#define CAN_BE_OPTIMIZED_MSG(msg) ASSERT(true, msg)
#define CAN_BE_OPTIMIZED CAN_BE_OPTIMIZED_MSG("A point where the programmer can optimize");
#endif
}