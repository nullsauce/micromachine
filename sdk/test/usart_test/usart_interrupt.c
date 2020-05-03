
#include <stdint.h>
#include <stdbool.h>
#include <io.h>
#include <control_registers.h>
#include <system.h>
#include <micromachine_mcu.h>
#include <tinyprintf.h>


struct usart_device;

/**
 * This callback is used when the usart_transmit_async is terminated.
 */
typedef void (*xfer_completed_cb)(struct usart_device* dev);

struct usart_config {
	bool enable;
};

struct usart_device {
	USART_TypeDef* port;
	struct usart_config conf;

	const uint8_t* tx_data;
	volatile size_t tx_len;
	volatile size_t tx_data_size;
	volatile bool tx_completed;
	xfer_completed_cb tx_completed_callback;

	uint8_t* rx_data;
	volatile size_t rx_len;
	volatile size_t rx_data_size;
	volatile bool rx_completed;
	xfer_completed_cb rx_completed_callback;

	void* user_data;
};



void receive_data(struct usart_device* usart_dev) {
	bool rx_data_available = (usart_dev->port->ISR & USART_ISR_RXNE);
	if(rx_data_available && usart_dev->rx_data != NULL && (false == usart_dev->rx_completed)) {

		uint32_t rx_data = usart_dev->port->RX;
		usart_dev->rx_data[usart_dev->rx_len++] = (uint8_t)rx_data;

		if(usart_dev->rx_len == usart_dev->rx_data_size) {
			// Transfer done.
			// unsubscribe to rx clear interrupts and present
			// the data to the user.
			CLEAR_BIT(usart_dev->port->CR1, USART_CR1_RXNE);
			usart_dev->rx_completed = true;
			if(usart_dev->rx_completed_callback) {
				usart_dev->rx_completed_callback(usart_dev);
			}
		}
	}
}

void send_data(struct usart_device* usart_dev) {
	bool can_send_next_data = (usart_dev->port->ISR & USART_ISR_TXE);
	if(can_send_next_data && usart_dev->tx_data != NULL && (false == usart_dev->tx_completed)) {

		if(usart_dev->tx_len < usart_dev->tx_data_size) {
			uint8_t byte = usart_dev->tx_data[usart_dev->tx_len++];
			usart_dev->port->TX = byte;
		} else {
			// Transfer done.
			// unsubscribe to tx clear interrupts and present
			// the data to the user.
			CLEAR_BIT(usart_dev->port->CR1, USART_CR1_TXE);
			usart_dev->tx_completed = true;
			if(usart_dev->tx_completed_callback) {
				usart_dev->tx_completed_callback(usart_dev);
			}
		}
	}
}


int usart_init(struct usart_device* dev) {

	dev->tx_data = NULL;
	dev->tx_data_size = 0;
	dev->tx_len = 0;
	dev->tx_completed = false;

	dev->rx_data = NULL;
	dev->rx_len = 0;
	dev->rx_data_size = 0;
	dev->rx_completed = false;

	MODIFY_REG(dev->port->CR1, USART_CR1_UE, dev->conf.enable);

	NVIC_EnableIRQ(USART_IRQn);
	return 0;
}

int usart_deinit(struct usart_device* dev) {
	CLEAR_BIT(dev->port->CR1, USART_CR1_UE);
	return 0;
}

void usart_transmit_async(struct usart_device* dev, const uint8_t* data, size_t size) {
	dev->tx_data = data;
	dev->tx_data_size = size;
	dev->tx_len = 0;
	dev->tx_completed = false;

	// enable  Tx empty interrupt, which will trigger the first interrupt
	SET_BIT(dev->port->CR1, USART_CR1_TXE);
}

void usart_receive_async(struct usart_device* dev, uint8_t* data, size_t size) {
	dev->rx_data = data;
	dev->rx_data_size = size;
	dev->rx_len = 0;
	dev->rx_completed = false;

	// enable  Rx not empty interrupt, which will trigger the first interrupt
	SET_BIT(dev->port->CR1, USART_CR1_RXNE);
}

static
void usart_wait_transmit_complete(struct usart_device* dev) {
	while(false == dev->tx_completed){};
}

static
void usart_wait_receive_complete(struct usart_device* dev) {
	while(false == dev->rx_completed){};
}

struct usart_device usart_device = {
	.port = USART0,
	.conf = {.enable = true}
};

void _isr_usart() {
	receive_data(&usart_device);
	send_data(&usart_device);
}

void main() {

	usart_init(&usart_device);

	/*
	 * This test works as follow:
	 * 1) The app sends a hello message to the VM.
	 * 3) The app reads the first 15 bytes of data available trough the usart
	 *    (if there isn't any data available for reading, an error message is printed)
	 * 4) The app sends back the 15 bytes of data to the VM.
	 */

	// Send a hello message
	const char* data = "<app> Hello vm!\n";
	usart_transmit_async(&usart_device, data, 16);
	usart_wait_transmit_complete(&usart_device);

	// This test can run only if there's some data available for reading on the usart.
	if(usart_device.port->ISR & USART_ISR_RXNE) {

		// We except the vm to send _exactly_ that many bytes
		const size_t expected_data_size = 16;
		uint8_t receive_buffer[32] = {0};

		// Receive 15 bytes
		usart_receive_async(&usart_device, receive_buffer, expected_data_size);
		usart_wait_receive_complete(&usart_device);

		// We can't rely on printf to generate a test output because there's no
		// synchronization between printf and the buffered usart stream.
		// So we send back the message via the isart instead.
		usart_transmit_async(&usart_device, receive_buffer, expected_data_size);
		usart_wait_transmit_complete(&usart_device);

	} else {
		printf("Error: There was no uart data to read!\n");
	}

	usart_deinit(&usart_device);
}
