#include "nrf52840.h"
#include "nrf52840_bitfields.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "printf.h"
#include "uart.h"

#include <nrf.h>

#define RTCPROG 
#define GPIOTEPROG

#define PIN_UART_TX 6

#ifdef GPIOTEPROG
  #define PIN_LED 14
  #define PIN_BUTTON 11
#endif

char *sysout = "Inside main..\r\n";
// flag for interrupt
volatile uint32_t iflag = 0;

int main(void)
{
   uart_init(PIN_UART_TX);
  // Start LFCLK (32kHz) crystal oscillator. If you don't have crystal on your board, choose RCOSC instead.
  NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos;
  NRF_CLOCK->TASKS_LFCLKSTART = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;

#ifdef GPIOTEPROG
  
   //config button as an event
   NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) | (PIN_BUTTON << GPIOTE_CONFIG_PSEL_Pos) |
				(GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos) ;
   
   //pull up for button
   nrf_gpio_cfg_input(PIN_BUTTON,NRF_GPIO_PIN_PULLUP);

   // config led as a task	
   NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos) | (PIN_LED << GPIOTE_CONFIG_PSEL_Pos) |
				(GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) | (GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos);
	
   // enable interrupt 
   NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Msk;
    
   NVIC_EnableIRQ(GPIOTE_IRQn);
#endif

#ifdef RTCPROG
  // 32kHz timer period
  NRF_RTC0->PRESCALER = 0;

  //  30.5ms us compare value, generates EVENTS_COMPARE[0]
  NRF_RTC0->CC[0] = 1000;

  // Enable EVENTS_COMPARE[0] generation
  NRF_RTC0->EVTENSET = RTC_EVTENSET_COMPARE0_Enabled << RTC_EVTENSET_COMPARE0_Pos;
  // Enable IRQ on EVENTS_COMPARE[0]
  NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos;

  // Enable RTC IRQ and start the RTC
  NVIC_EnableIRQ(RTC0_IRQn);
  NRF_RTC0->TASKS_START = 1;
#endif

 while (1)
 {
    if(iflag == 1){

#ifdef GPIOTEPROG
    NRF_GPIOTE->TASKS_OUT[1] = 1;
#endif
    iflag = 0;
    printf(sysout);
    sysout = "Inside main..\r\n";
  }
    printf(sysout);
    nrf_delay_ms(1000);
  
  } 
}

#ifdef GPIOTEPROG
// This IRQ is generated on the press of button 1 
void GPIOTE_IRQHandler(void)
{
   volatile uint32_t dummy;
    if(NRF_GPIOTE->EVENTS_IN[0] == 1)
    {
        NRF_GPIOTE->EVENTS_IN[0] = 0;
 
        //uart does not work inside IRQ
        sysout = "Inside IRQ...\r\n" ;

        iflag = 1;

        //Read back event register so ensure we have cleared it before exiting IRQ handler.
        dummy = NRF_GPIOTE->EVENTS_IN[0];
        dummy;

    }

}
#endif

#ifdef RTCPROG
// This IRQ handler will trigger every 30.5 ms
void RTC0_IRQHandler(void)
{
  volatile uint32_t dummy;
  if (NRF_RTC0->EVENTS_COMPARE[0] == 1)
  {
    NRF_RTC0->EVENTS_COMPARE[0] = 0;

    // Increment compare value with 30.5 ms from current time.
    NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + 1000;
  
     iflag = 1;
    //uart does not work inside IRQ
    sysout = "Inside IRQ...\r\n" ;
    // Read back event register so ensure we have cleared it before exiting IRQ handler.
    dummy = NRF_RTC0->EVENTS_COMPARE[0];
    dummy;
  }
}
#endif
