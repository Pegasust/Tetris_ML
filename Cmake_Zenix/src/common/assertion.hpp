#include <cassert>

namespace Common
{
#ifndef ASSERT
#define ASSERT(expr, msg) assert(expr && msg)
#else
// There can be evaluation of message and expression.
constexpr void ASSERT(const int& expression, const char* message) {
    assert(expression && message);
}
#endif

}