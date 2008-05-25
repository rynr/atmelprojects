#include <stdint.h>
#include <avr/io.h>

#define __DEBUG__
#ifndef F_CPU
// define cpu click speed
#define F_CPU 3686400
#endif
#include <util/delay.h>

void init(void);
void init_ports(void);
void out(int);
int decode(int);

/*
 * int main(void) - Main routine
 *
 * This is the main part of the application. It initializes the
 * controller, then starts a endless loop that will handle input.
 *
 */
int main(void)
{
  // LED-Counter
  int8_t current_count = 0;

  // Initialize controller
  init();

  // Endless loop.
  for(;1==1;) {
    _delay_ms(100);
    current_count += 1;
    if(current_count >= (8*4)) {
      current_count = 0;
    }
    out(current_count);
  }

  // finish process.
  return 1;
}

/*
 * void init(void) - Initialize the controller
 *
 * Initializes Midi-connection and Ports.
 *
 */
void init(void) {
  // Initialize Ports
  init_ports();
}

/*
 * void init_ports(void) - Initialize Ports
 *
 * Port B is set to input, each pin is a program number to be send as
 * program change.
 *
 */
void init_ports(void) {
  // Port A
  DDRA = 0xff; // 0 ≘ Input, 1 ≘ Output
  PORTA = 0xff;
  // Port B
  DDRB = 0xff; // 0 ≘ Input, 1 ≘ Output
  PORTB = 0xff;
  // Port C
  DDRC = 0xff; // 0 ≘ Input, 1 ≘ Output
  PORTC = 0xff;
  // Port D
  DDRD = 0xff; // 0 ≘ Input, 1 ≘ Output
  PORTD = 0xff;
}

/*
 * Setting the output depending on the value
 */
void out(int current) {
  if(current < 8) {
    PORTB = PORTC = PORTD = 0xff;
    PORTA = 255 ^ decode(current & 0x07);
  } else if (current < 16) {
    PORTA = PORTC = PORTD = 0xff;
    PORTB = 255 ^ decode(current & 0x07);
  } else if (current < 24) {
    PORTA = PORTB = PORTD = 0xff;
    PORTC = 255 ^ decode(current & 0x07);
  } else if (current < 32) {
    PORTA = PORTB = PORTC = 0xff;
    PORTD = 255 ^ decode(current & 0x07);
  } else {
    PORTA = PORTB = PORTC = PORTD = 0x00;
  }
}

int decode(int in) {
  int out = 0;
  switch(in) {
    case 0:
      out = 1;
      break;
    case 1:
      out = 2;
      break;
    case 2:
      out = 4;
      break;
    case 3:
      out = 8;
      break;
    case 4:
      out = 16;
      break;
    case 5:
      out = 32;
      break;
    case 6:
      out = 64;
      break;
    case 7:
      out = 128;
      break;
  }
  return out;
}
