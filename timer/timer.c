#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main(void)
{
  DDRB = 0xFF; // All port B are outputs

  while (1)
  {
    PORTB = 0xFF;
  }
}

