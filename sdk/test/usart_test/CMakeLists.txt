cmake_minimum_required(VERSION 3.2)
project(usart_test)

add_executable(usart_simple_test
		usart_simple.c
		)
add_micromachine_executable(usart_simple_test)
generate_dissasembly(usart_simple_test)

add_executable(usart_it_test
		usart_it.c
		)
add_micromachine_executable(usart_it_test)
generate_dissasembly(usart_it_test)
