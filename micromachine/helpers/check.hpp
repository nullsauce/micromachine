//

//

#pragma once

#include <cstdio>
#include <exception>

#define __DEBUG_STR2__(x) #x
#define __DEBUG_STR__(x) __DEBUG_STR2__(x)
__glibcxx_assert()
#define micromachine_fail(message, ...)                                                        \
{                                                                                              \
	fprintf(stderr, "where: %s line %d\n", __FILE__, __LINE__);                                \
	fprintf(stderr, "micromachine fatal error: " message "\n", ##__VA_ARGS__);                 \
	std::terminate();                                                                          \
}


#ifdef MICROMACHINE_ENABLE_CHECKS
#define micromachine_check(cond, message, ...)                                                 \
{                                                                                              \
	if(!(cond)) {                                                                              \
		micromachine_fail("the following check failed: " #cond "\nreason: " message, ##__VA_ARGS__)      \
	}                                                                                          \
}
#else
#define micromachine_check(...) (void)0
#endif

#undef __DEBUG_STR2__
#undef __DEBUG_STR__
