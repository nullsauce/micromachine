
#include <stdbool.h>
#include <stdint.h>
#include <micromachine.h>

// Fwd declaration
struct usart_device;

struct usart_config {
	bool enable;
};

struct usart_device {
	USART_TypeDef* port;
	struct usart_config conf;
};

int usart_init(struct usart_device* dev) {
	MODIFY_REG(dev->port->CR1, USART_CR1_UE, dev->conf.enable);
	return 0;
}

int usart_deinit(struct usart_device* dev) {
	CLEAR_BIT(dev->port->CR1, USART_CR1_UE);
	return 0;
}

static uint8_t usart_read(struct usart_device* dev) {SET_BIT(dev->port->CR1, USART_CR1_RXNE);
	// Wait RX is fullfilled, it could be coupled with timeout..
	while((dev->port->ISR & USART_ISR_RXNE) == 0)
		;

	CLEAR_BIT(dev->port->ICR, USART_ICR_RXNE);
	uint32_t rx_data = dev->port->RX;
	return (uint8_t)(rx_data & 0xFFu);
}

void wait_for_tx_empty(struct usart_device* dev) {
	while(0 == READ_BIT(dev->port->ISR, USART_ISR_TXE)) {
	}
}

static void echo(struct usart_device* dev) {
	unsigned int len = 4096;
//	printf("[+] Data received : '");
	while(len--) {
		uint8_t rx_data = usart_read(dev);
//		printf("%c", rx_data);
		wait_for_tx_empty(dev);
		dev->port->TX = rx_data;
	}
//	printf("'\n");
}

void main() {

	struct usart_device dev = {
		.port = USART0,
		.conf =
			{
				.enable = true,
			},
	};

	usart_init(&dev);

	echo(&dev);

	usart_deinit(&dev);
}
