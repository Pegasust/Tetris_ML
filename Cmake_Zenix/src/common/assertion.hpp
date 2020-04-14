// Contains many macro definitions that are useful for developing
#pragma once
#include <cassert>

namespace Common {
#define SILENT_MSG

#ifndef ASSERT
#define ASSERT(expr, msg) assert(expr&& msg)
#else
// There can still be evaluation of message and expression. (Not wanted)
constexpr void ASSERT(const int& expression, const char* message) {
    assert(expression && message);
}
#endif

// Getting line number hack.
#define __STRINGIZE_(line) #line
// The hack that gets line # as string.
#define __LINE__2_STR(line) __STRINGIZE_(line)
// Format for warnings
#define WARNING_FORMAT(print_str) "Warning @" __FILE__ ":" __LINE__2_STR(__LINE__) ": " print_str
// Defines PRINT_DEBUG_WARNINGS(print_str)
#ifndef PRINT_DEBUG_WARNINGS
#ifdef DEBUG_DEFINED
#ifndef VERBOSITY_LOG
#define VERBOSITY_LOG(msg) ASSERT(true, msg)
#endif
#if !defined _IOSTREAM_ || defined SILENT_MSG
#define __PRINT_OUT_MESSAGE__(msg) ASSERT(true,msg)
#else
#define __PRINT_OUT_MESSAGE__(msg) std::cout<<msg<<std::endl;
#endif
#define PRINT_DEBUG_WARNINGS(print_str)                                                            \
    __PRINT_OUT_MESSAGE__(WARNING_FORMAT(print_str));                                                           \
    VERBOSITY_LOG(WARNING_FORMAT(print_str));
#else
#define PRINT_DEBUG_WARNINGS(print_str) ASSERT(true, print_str)
#endif
#endif

constexpr void check(const int& expression, char msg[]) {
    if (!expression) {
        __PRINT_OUT_MESSAGE__(msg);
    }
}
#ifndef UNIMPLEMENTED_FEATURE
#define UNIMPLEMENTED_FEATURE_MSG(msg) ASSERT(false, msg)
#define UMIMPLEMENTED_FEATURE UNIMPLEMENTED_FEATURE_MSG("Unimplemented feature.")
#endif

#ifndef POTENTIAL_BUG_POINT
#define POTENTIAL_BUG_POINT_MSG(msg) PRINT_DEBUG_WARNINGS(msg)
#define POTENTIAL_BUG_POINT POTENTIAL_BUG_POINT_MSG("The point where there might be bug.");
#endif

#ifndef CAN_BE_OPTIMIZED
#define CAN_BE_OPTIMIZED_MSG(msg) PRINT_DEBUG_WARNINGS(msg)
#define CAN_BE_OPTIMIZED CAN_BE_OPTIMIZED_MSG("A point where the programmer can optimize");
#endif


} // namespace Common