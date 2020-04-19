
#include <stdint.h>
#include <stdbool.h>
#include <io.h>
#include <control_registers.h>
#include <system.h>
#include <micromachine_mcu.h>

#include <tinyprintf.h>

// todo place it in a common header file.
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

typedef void (*xfer_complete_cb)(void* user, int status);


struct usart_config {
	bool enable;
	// Future config will be abstracted where (baud rate, parity, stop/start bits ...)
};

struct usart_device {
	USART_TypeDef* port;
	struct usart_config conf;

	uint8_t *tx_data;
	size_t tx_len;
	xfer_complete_cb cb; // callback when usart_transmit_async is used
	void* user_data;
};



void xfer_complete(void* user, int status) {
	struct usart_device *dev = user;
	printf("%s\n",__FUNCTION__ );
	*(int*)(dev->user_data) = 0x55;
}


int usart_init(struct usart_device* dev) {
	MODIFY_REG(dev->port->CR1, USART_CR1_UE, dev->conf.enable);
	return 0;
}

void usart_transmit_async(struct usart_device* dev, uint8_t *data, size_t len) {

	dev->tx_data = data;
	dev->tx_len = len;

	// enable  Tx empty interrupt (should raise an interrupt if tx is empty)
	SET_BIT(dev->port->CR1, USART_CR1_TXE);

	NVIC_EnableIRQ(USART_IRQn);
}

// shared data between isr and main task
static int dummy = 42;
static struct usart_device dev = {
	.port = USART,
	.conf = {
		.enable = true,
	},
	.cb = xfer_complete,
	.user_data = &dummy,
};

static void usart_transmit_it(struct usart_device *dev) {

	size_t transmitted = 0;
	while(transmitted < dev->tx_len) {
		dev->port->TX = dev->tx_data[transmitted];
		transmitted++;
	}

	// disable tx empty interrupt
	CLEAR_BIT(dev->port->CR1, USART_CR1_TXE);

	// enable tx complete interrupt
	SET_BIT(dev->port->CR1, USART_CR1_TXC);
}

static void usart_end_transmit_it(struct usart_device *dev) {

	// disable tx complete interrupt
	CLEAR_BIT(dev->port->CR1, USART_CR1_TXC);

	// clear tx empty interrupt status
	SET_BIT(dev->port->ICR, USART_ICR_TXC);

	// todo add a state to signal end of transmit

	if (dev->cb) {
		dev->cb(dev, 0);
	}

}

void _isr_usart() {
	printf("%s\n",__FUNCTION__ );
	uint32_t isr_flags = USART->ISR;
	uint32_t icr_flags = USART->ICR;
	uint32_t cr1_flags = USART->CR1;
	printf("isr_flags %08x\n",isr_flags);

	if ((isr_flags & USART_ISR_TXE) && (cr1_flags & USART_CR1_TXE)) {
		printf("[usart_transmit_it] isr_flags %08x, cr1_flags %08x\n",isr_flags, cr1_flags);
		usart_transmit_it(&dev);
		return;
	}

	if ((isr_flags & USART_ISR_TXC) && (cr1_flags & USART_CR1_TXC)) {
		printf("[usart_end_transmit_it] isr_flags %08x, cr1_flags %08x\n",isr_flags, cr1_flags);
		usart_end_transmit_it(&dev);
		return;
	}

}

void main() {
	usart_init(&dev);
	uint8_t data[] = "Hello usart world!\n";
	usart_transmit_async(&dev, data, sizeof(data));
	printf("transmit...\n");
	while(*(int*)(dev.user_data) != 0x55);
	printf("done!\n");
}
