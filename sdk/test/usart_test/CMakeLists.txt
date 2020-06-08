cmake_minimum_required(VERSION 3.2)
project(usart_test)

add_executable(usart_simple_test
	usart_simple.c
)
add_micromachine_executable(usart_simple_test)
generate_dissasembly(usart_simple_test)

add_executable(usart_interrupt_test
	usart_interrupt.c
)
add_micromachine_executable(usart_interrupt_test)
generate_dissasembly(usart_interrupt_test)


add_executable(usart_echoer_test
		usart_echoer.c
		)
add_micromachine_executable(usart_echoer_test)
generate_dissasembly(usart_echoer_test)
