#ifndef BALDUR_UTIL_HH
#define BALDUR_UTIL_HH

#include <vector>
#include <string>

// It seems like gcc's optimization is messing with slang's classes
// manually use breakpoint to debug using the following command
static inline void breakpoint() { __asm__ __volatile__("int3"); }

std::vector<std::string> get_tokens(const std::string &line, const std::string &delimiter);

#endif  // BALDUR_UTIL_HH
