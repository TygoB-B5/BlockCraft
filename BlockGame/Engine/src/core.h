#pragma once

#ifdef _DEBUG
#define GLR_DEBUG
#endif

#ifdef GLR_DEBUG
#define DEBUG_ONLY(expression) expression
#else
#define DEBUG_ONLY(expression)
#endif

#include <iostream>

#ifdef GLR_DEBUG
#define GLR_LOG(message) std::cout << message << "\n";
#else
#define GLR_LOG(description, value)
#endif

#ifdef GLR_DEBUG
#define GLR_ASSERT(condition, message) if(!condition) {std::cout << "APP: " << message << "\n"; __debugbreak();}
#else
#define GLR_ASSERT(condition, message)
#endif

#ifdef GLR_DEBUG
#define GLR_CORE_ASSERT(condition, message) if(!condition) {std::cout << "CORE: " << message << "\n"; __debugbreak();}
#else
#define GLR_CORE_ASSERT(condition, message)
#endif