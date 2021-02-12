#include "Constants.h"

#ifndef BUILD_GIT_REVISION
#error Git revision not defined.
#endif

namespace Constants {
    const char* GIT_REVISION = STRINGIFY(BUILD_GIT_REVISION);
}
