cmake_minimum_required(VERSION 3.2)
project(md5test)

add_executable(md5sum
	md5sum.c
	md5.c
	md5.h
)
add_micromachine_executable(md5sum)
generate_dissasembly(md5sum)
