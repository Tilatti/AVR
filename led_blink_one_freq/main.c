#define F_CPU 1000000UL

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/delay.h>

int delta = 1;
int iter_index = 0;

/*
 * We want to generate sin wave with a PWM generated squard wave
 * S_PERIOD : Period (Second) of generated sin wave
 * PWM_FREQ : Frequency of pwm generated square wave
 * ITER_CYCLE : Time during the square wave duty cycle is not modified
 */

#define SIN_PERIOD   5             /* 1 second */
#define PWM_FREQ   (1000000 / 256) /* Hz */
#define ITER_CYCLE ((PWM_FREQ * SIN_PERIOD) / 512)

ISR(TIMER0_OVF_vect)
{
  /*
   * Each ITER_CYCLE we change square wave duty cycle
   */

  if ((iter_index % ITER_CYCLE) == 0)
  {
    if ((OCR0A == 0xff) || (OCR0A == 0x00))
    {
      delta = -delta;
    }

    OCR0A = OCR0A + delta;
  }

  iter_index++;
}

int main(void)
{
  /*
   * =========
   * Setup PWM
   * =========
   */

  /*
   * OCOA1(PB2) is output
   */

  DDRB |= (1 << PB2);

  /*
   * Enable timer0 interrupt
   */

  TIMSK |= _BV(TOIE0);

  /*
   * Stop timer 0
   */

  TCCR0B = 0;

  /*
   * - Fast pwm mode 3 (Select mode)
   * - Up_counting (OCA output)
   * - Set duty cycle (Compare match register)
   * - No presacling
   */

  TCCR0A = (1 << WGM00) | (1 << WGM01);
  TCCR0A |= (1 << COM0A1);
  OCR0A = 0x0f;
  TCCR0B |= (1 << CS00);

  /*
   * ====
   * MAIN
   * ====
   */


  /*
   * Launch operations
   * - Activate interrupts
   * - Main loop
   */

  sei();

  while (1)
  { }
}
