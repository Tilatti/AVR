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

#define LOW_SIN_PERIOD    1          /* 10 second */
#define HIGH_SIN_PERIOD   10           /* 5 second */
#define PWM_FREQ   (F_CPU / 256)    /* Hz */

#define ITER_CYCLE(Sin_period) ((PWM_FREQ * (Sin_period)) / 512)
#define LOW_ITER_CYCLE (ITER_CYCLE(LOW_SIN_PERIOD))
#define HIGH_ITER_CYCLE (ITER_CYCLE(HIGH_SIN_PERIOD))

/*
 * Internals variables
 */

int delta = 1;
int iter_index = 0;
int iter_cycle = HIGH_ITER_CYCLE;

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
 * == Analog comparator interrupt ==
 * Change PWM output frequency
 * (Speed of duty cycle change)
 */

uint8_t pd1_state = 0x00;

ISR(ANA_COMP_vect)
{
  PORTD = ~pd1_state;

  if (iter_cycle == HIGH_ITER_CYCLE)
  {
    iter_cycle = LOW_ITER_CYCLE;
  }
  else /* (iter_cycle == LOW_ITER_CYCLE) */
  {
    iter_cycle = HIGH_ITER_CYCLE;
  }
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

  DDRD |= (1 << PD1);

  /*
   * ==============================
   * Setup ADC (Variable frequency)
   * ==============================
   */

  /*
   * Enable ADC
   */

  ACSR &= ~_BV(ACD);

  /*
   * - Enable inerrupt
   * - Interrupt on output toggle
   * - Bandgap is selected on input 0 (AIN0 not used)
   * - Desactivate digital inputs (AIN0 and AIN1)
   */

  ACSR |= _BV(ACIE);
  ACSR |= _BV(ACIS0) | _BV(ACIS1);
  //ACSR |= _BV(ACBG);
  //DIDR |= _BV(AIN0D) | _BV(AIN1D);

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
