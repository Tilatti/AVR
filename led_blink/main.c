#define F_CPU 1000000UL

/*
 * We want to generate sin wave with a PWM generated squard wave
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/delay.h>

/*
 * S_PERIOD : Period (Second) of generated sin wave
 * PWM_FREQ : Frequency of pwm generated square wave
 * ITER_CYCLE : Time during the square wave duty cycle is not modified
 */

#define PRESCALER 64
#define LOW_SIN_PERIOD    20          /* 20 second */
#define HIGH_SIN_PERIOD   10           /* 10 second */
#define PWM_FREQ   (F_CPU / 256 / PRESCALER)    /* Hz */

#define ITER_CYCLE(Sin_period) ((PWM_FREQ * (Sin_period)) / 512)
#define LOW_ITER_CYCLE (ITER_CYCLE(LOW_SIN_PERIOD))
#define HIGH_ITER_CYCLE (ITER_CYCLE(HIGH_SIN_PERIOD))

/*
 * Internals variables
 */

int delta = 1;
int iter_index = 0;
volatile int iter_cycle = LOW_ITER_CYCLE;

/*
 * == PWM interrupt (TIMER0) ==
 * Change PWM duty cycle on each iter_cycle
 */

ISR(TIMER0_OVF_vect)
{
  if ((iter_index % iter_cycle) == 0)
  {
    if ((OCR0A == 0xff) || (OCR0A == 0x00))
    {
      delta = -delta;
    }

    OCR0A = OCR0A + delta;
  }

  iter_index++;
}

/*
 * == PCINT0 interrupt (PB0) ==
 */

uint8_t pd_state = 0x00;

ISR(PCINT_vect)
{
  pd_state = ~pd_state;
  PORTD = pd_state;

  if (iter_cycle == HIGH_ITER_CYCLE)
    iter_cycle = LOW_ITER_CYCLE;
  else
    iter_cycle = HIGH_ITER_CYCLE;
}

int main(void)
{
  /*
   * =============
   * GENERAL SETUP
   * =============
   */

  /*
   * PD1 is output
   */

  DDRD |= _BV(PD1);

  /*
   * ======================
   * Extern Interrupt SETUP
   * ======================
   */

  DDRB &= ~_BV(PB0);
  GIMSK |= _BV(PCIE);
  PCMSK |= _BV(PCINT0);

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
   * - Presacling (64)
   */

  TCCR0A = (1 << WGM00) | (1 << WGM01);
  TCCR0A |= (1 << COM0A1);
  OCR0A = 0x0f;
  TCCR0B |= (1 << CS00) | (1 << CS01);

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
