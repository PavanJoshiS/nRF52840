#include "nrf52840.h"
#include "nrf52840_bitfields.h"
#include <stdint.h>

#include "nrf_uart.h"
#include "uart.h"

static bool txdrdy = false;

int uart_init(uint32_t pseltxd)
{
	nrf_uart_configure(NRF_UART0, NRF_UART_PARITY_EXCLUDED, NRF_UART_HWFC_DISABLED);
	NRF_UART0->PSEL.TXD = pseltxd;
	nrf_uart_baudrate_set(NRF_UART0, NRF_UART_BAUDRATE_1000000);

	/* Enable interrupt source in the peripheral */
	NRF_UART0->INTENSET = UART_INTENSET_TXDRDY_Msk;
	/* Enable interrupt in the CPU interrupt controller */
	NVIC_EnableIRQ(UARTE0_UART0_IRQn);

	return 0;
}

/* Interrupt handler function. For a table with all handler function names, see startup.c */
void UARTE0_UART0_IRQHandler()
{
	if (NRF_UART0->EVENTS_TXDRDY == 1) {
		NRF_UART0->EVENTS_TXDRDY = 0;
		txdrdy = true;
	}
}

void _putchar(char character)
{
	NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Enabled;
	NRF_UART0->TXD = character;
	NRF_UART0->TASKS_STARTTX = 1UL;

	while (!txdrdy) {
		/* Put CPU to sleep and wake-up from interrupt */
		__WFE();
		__SEV();
		__WFE();
	}
	txdrdy = false;

	NRF_UART0->TASKS_STOPTX = 1UL;
	NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Disabled;
}