cmake_minimum_required(VERSION 3.2)
project(cpuidtest)

add_executable(cpuid_test
		cpuid_test.c
)

add_micromachine_executable(cpuid_test)
generate_dissasembly(cpuid_test)
