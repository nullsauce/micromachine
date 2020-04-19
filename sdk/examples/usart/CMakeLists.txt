cmake_minimum_required(VERSION 3.15)
project(usart_example)

add_executable(usart_simple
		usart_simple.c
)
add_micromachine_executable(usart_simple)
generate_dissasembly(usart_simple)


