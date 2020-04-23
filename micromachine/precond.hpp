//
// Created by fla on 4/30/17.
//

#pragma once

#include <cstdio>
#include <exception>

#define precond_fail(message,...) { \
		fprintf(stderr, "where: %s line %d\n", __FILE__, __LINE__); \
		fprintf(stderr, message, ##__VA_ARGS__); \
		std::terminate(); \
	} \

#ifdef MICROMACHINE_ENABLE_PRECOND_CHECKS
#define precond(cond, message, ...) { \
	if(!(cond)) { \
		precond_fail("FATAL: " message "\n", ##__VA_ARGS__) \
	}} \

#else
#define precond(...) (void)0
#endif
