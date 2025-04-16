// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

// <<< Standard Headers >>>
#include <new>     // <<<=== ADD THIS LINE for std::nothrow
#include <limits>  // Include for UINT_MAX
// Add other commonly used standard headers here if needed (e.g., <vector>, <string>, etc.)

#endif //PCH_H
