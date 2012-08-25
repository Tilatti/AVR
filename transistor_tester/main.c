#define F_CPU 1000000UL

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/delay.h>

#define RESULT (PD5)
#define E (PB0) // Emitter
#define B (PB1) // Base
#define C (PB2) // Collector

void test(void)
{
  /* ....... TEST CLOSE ....... */
  /* Base is not saturated => Vce = 0V , Ice > 0A */


  PORTB |= (_BV(C));
  PORTB |= (_BV(B));

  _delay_ms(10);

  if (PORTB & _BV(E))
  {
    PORTD |= _BV(RESULT);
  }
  else
  {
    PORTD &= ~(_BV(RESULT));
  }

  /* .........................  */

  _delay_ms(5000);

  /* ....... TEST ....... */
  /* Base is not saturated => Vce = 5V , Ice = 0A */


  PORTB |= (_BV(C));
  PORTB &= ~(_BV(B));

  _delay_ms(10);

  if (PORTB & _BV(E))
  {
    PORTD |= _BV(RESULT);
  }
  else
  {
    PORTD &= ~(_BV(RESULT));
  }

  /* .........................  */

  _delay_ms(5000);
}

int main(void)
{
  /*
   * C and B are outputs
   * E is input
   */

  DDRB |= (0 << E) | (1 << B) | (1 << C);
  PORTB = 0x00;

  DDRD |= (1 << RESULT);
  PORTD = 0x00;

  /*
   * Test loop
   */

  while (1)
  {
    test();
  }
}

