#include "nrf52840.h"
#include "nrf52840_bitfields.h"
#include <stdint.h>

#include "nrf_uart.h"
#include "uart.h"

int uart_init(uint32_t pseltxd)
{
	nrf_uart_configure(NRF_UART0, NRF_UART_PARITY_EXCLUDED, NRF_UART_HWFC_DISABLED);
	NRF_UART0->PSEL.TXD = pseltxd;
	nrf_uart_baudrate_set(NRF_UART0, NRF_UART_BAUDRATE_1000000);
	return 0;
}

void _putchar(char character)
{
	NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Enabled;
	NRF_UART0->TXD = character;
	NRF_UART0->EVENTS_TXDRDY = 0;
	NRF_UART0->TASKS_STARTTX = 1UL;
	while (NRF_UART0->EVENTS_TXDRDY == 0) {
	};
	NRF_UART0->EVENTS_TXDRDY = 0;
	NRF_UART0->TASKS_STOPTX = 1UL;
	NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Disabled;
}