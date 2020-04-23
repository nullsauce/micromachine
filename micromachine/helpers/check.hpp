//
// Created by fla on 4/30/17.
//

#pragma once

#include <cstdio>
#include <exception>

#define micromachine_fail(message, ...)                                                                 \
	{                                                                                              \
		fprintf(stderr, "where: %s line %d\n", __FILE__, __LINE__);                                \
		fprintf(stderr, message, ##__VA_ARGS__);                                                   \
		std::terminate();                                                                          \
	}

#ifdef MICROMACHINE_ENABLE_CHECKS
#define micromachine_check(cond, message, ...)                                                                \
	{                                                                                              \
		if(!(cond)) {                                                                              \
			micromachine_fail("FATAL: " message "\n", ##__VA_ARGS__)                                    \
		}                                                                                          \
	}

#else
#define micromachine_check(...) (void)0
#endif
