#define F_CPU 1000000UL

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/delay.h>

volatile uint16_t timer0_overflows = 0;

ISR(TIMER0_OVF_vect)
{
  timer0_overflows++;
}

void idle_sleep(uint16_t duration, uint16_t prescal)
{
  uint16_t overflows = ((duration * F_CPU) / (0xFF * prescal)) / 1000;

  while (timer0_overflows < overflows)
  {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }

  timer0_overflows = 0;
}

int main(void)
{
  DDRB = 0xFF; // All port B are outputs

  /*
   * Configure TIMER0
   *  - Enable timer0 interrupt
   *  - Set prescaler
   */

  TIMSK |= _BV(TOIE0);
  TCCR0B |= _BV(CS02);

  sei();

  while (1)
  {
    idle_sleep(6000, 256);
    PORTB = 0xFF;
    idle_sleep(1000, 256);
    PORTB = 0x00;
  }
}

