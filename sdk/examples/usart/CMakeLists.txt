cmake_minimum_required(VERSION 3.15)
project(usart_example)

add_executable(usart_simple
		usart_simple.c
		)
add_micromachine_executable(usart_simple)
target_link_libraries(usart_simple cmsis)
generate_dissasembly(usart_simple)


add_executable(usart_it
		usart_it.c
		)
add_micromachine_executable(usart_it)
target_link_libraries(usart_it cmsis)
generate_dissasembly(usart_it)

