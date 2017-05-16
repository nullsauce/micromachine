//
// Created by fla on 4/30/17.
//

#ifndef THUMBEMU_DEBUG_HPP
#define THUMBEMU_DEBUG_HPP

#include <cstdio>
#include <exception>

#define precond_fail(message,...) { \
		fprintf(stderr, "where: %s line %d\n", __FILE__, __LINE__); \
		fprintf(stderr, message, ##__VA_ARGS__); \
		std::terminate(); \
	} \

#define precond(cond, message, ...) { \
	if(!(cond)) { \
		precond_fail("FATAL: " message "\n", ##__VA_ARGS__) \
	}} \

#endif //THUMBEMU_DEBUG_HPP
