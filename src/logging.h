#include <iostream>

#if INFO_LOG_ENABLE
#define INFO_LOG(x) std::cout << x << std::endl
#else
#define INFO_LOG(x)
#endif

#if ERROR_LOG_ENABLE
#define ERROR_LOG(x) std::cerr << x << std::endl
#else
#define ERROR_LOG(x)
#endif