#include <avr/io.h>

#ifndef F_CPU
// define cpu click speed, needed for util/delay.h
#define F_CPU 16000000
#endif
#include <util/delay.h>

int main(void)
{
  DDRB = 0xff;
  PORTB = 0xff;

  // Endless loop.
  for(;1==1;) {
    PORTB = 0b00000010;
    _delay_ms(2000);
    PORTB = 0b00000001;
    _delay_ms(40000);
    PORTB = 0b00000011;
    _delay_ms(2000);
    PORTB = 0b00000100;
    _delay_ms(36000);
  }

  // finish process.
  return 1;
}

