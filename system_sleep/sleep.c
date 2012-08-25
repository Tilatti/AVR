#define F_CPU 1000000UL

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/delay.h>

ISR(WDT_OVERFLOW_vect)
{
  /*
   * Set wdt to generate interrupt
   * (Reset to 0 on each interrupt)
   */

  WDTCSR |= _BV(WDIE);
}

/*
 * Set system into sleep state (POWER DOWN)
 * System wakes up whan watchdog is timed out
 */

void system_sleep(uint8_t duration)
{

  /*
   * Enable Watchdog
   * Set wdt to generate interrupt
   */

  wdt_enable(duration);
  WDTCSR |= _BV(WDIE);

  /*
   * Set sleep mode to POWER DOWN
   */

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();

  /* ... SLEEP (wait duration) ... */

  /*
   * Disable WDT
   */

  wdt_disable();

  _delay_ms(1);
}

int main(void)
{
  DDRB = 0xFF; // All port B are outputs
  sei();

  while (1)
  {
    system_sleep(WDTO_1S);
    PORTB = 0xFF;
    system_sleep(WDTO_1S);
    PORTB = 0x00;
  }
}

