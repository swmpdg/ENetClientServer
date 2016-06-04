#include "PlatformHeaders.h"

#include "Platform.h"

#ifdef WIN32
const size_t MAX_PATH_LENGTH = MAX_PATH;
#else
const size_t MAX_PATH_LENGTH = PATH_MAX;
#endif