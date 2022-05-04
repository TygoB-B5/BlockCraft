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
#define GLR_LOG(description, value) std::cout << description << value << "\n";
#else
#define GLR_LOG(description, value)
#endif