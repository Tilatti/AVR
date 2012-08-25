#define F_CPU 1000000UL

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/delay.h>

int main(void)
{
  uint16_t period = 0xAFFF;
  uint16_t on_time = 100;
  uint16_t on_delta = 5;

  /*
   * Init output
   */

  DDRD |= (1 << PD0) | (1 << PD1);
  PORTD |= _BV(PD1);

  /*
   * Main loop
   */

  while (1)
  {
    /*
     * Off time
     */

    PORTD &= ~_BV(PD0);
    _delay_loop_2(period - on_time);

    /*
     * On time
     */

    PORTD |= _BV(PD0);
    _delay_loop_2(on_time);

    /*
     * Compute time parameters
     */

    if (on_time >= period)
    {
      on_delta = -5;
    }
    else if (on_time <= 30)
    {
      on_delta = 5;
    }
    else
    {
      on_time = on_time + on_delta;
      on_delta = on_delta * 2;
    }
  }
}
