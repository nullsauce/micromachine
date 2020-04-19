#include <stdint.h>
#include <stdbool.h>

#include <micromachine_mcu.h>
#include <tinyprintf.h>


struct usart_config {
	bool enable;
	// Future config will be abstracted where (baud rate, parity, stop/start bits ...)
};

struct usart_device {
	USART_TypeDef* port;
	struct usart_config conf;
};

int usart_init(struct usart_device* dev) {
	MODIFY_REG(dev->port->CR1, USART_CR1_UE, dev->conf.enable);
	return 0;
}

static void usart_transmit(struct usart_device* dev, uint8_t* data, size_t len) {

	size_t transmitted = 0;
	while (transmitted < len) {
		dev->port->TX = data[transmitted];
		transmitted++;
	}
}

void main() {

	static struct usart_device dev = {
		.port = USART,
		.conf =
			{
				.enable = true,
			},
	};

	usart_init(&dev);

	uint8_t data[] = "Hello usart world!\n";
	usart_transmit(&dev, data, sizeof(data));
}
