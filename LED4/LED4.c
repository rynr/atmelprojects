#include <stdint.h>
#include <avr/io.h>

#define __DEBUG__
#ifndef F_CPU
// define cpu click speed, needed for util/delay.h
#define F_CPU 3686400
#endif
#include <util/delay.h>

/*
 * To use this program, just connect a LED with a resistor from each
 * port pin to VCC. To get a LED lighted, the Output must be set to low.
 */

void init(void);
void init_ports(void);
void out(int);
int decode(int);

/*
 * int main(void) - Main routine
 *
 * This is the main part of the application. It initializes the
 * controller, then starts a endless loop that will run the sequence.
 *
 * I think, i should change the code a bit to have a interrupt
 * controlling the next step. If i did this, i could have the
 * controller to be set to SLEEP mode, using less power.
 */
int main(void)
{
  // LED-Counter
  int8_t current_count = 0;

  // Initialize ports (later interrupts and so on)
  init();

  // Endless loop.
  for(;1==1;) {
    // waiting some time
    _delay_ms(100);
    // increasing the counter
    current_count += 1;
    // resetting the counter on overflow
    if(current_count >= (8*4)) {
      current_count = 0;
    }
    // handling of output
    out(current_count);
  }

  // finish process.
  return 1;
}

/*
 * void init(void) - Initialize the controller
 *
 */
void init(void) {
  // Initialize Ports
  init_ports();
}

/*
 * void init_ports(void) - Initialize Ports
 *
 * Setting all ports to output, and setting them to true.
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
  // Perhaps, there's a more neat way to do this? Any ideas?
  if(current < 8) {
    // now the output goes to port A, set all other ports off (high)
    PORTB = PORTC = PORTD = 0xff;
    PORTA = 255 ^ decode(current & 0x07);
  } else if (current < 16) {
    // now the output goes to port B, set all other ports off (high)
    PORTA = PORTC = PORTD = 0xff;
    PORTB = 255 ^ decode(current & 0x07);
  } else if (current < 24) {
    // now the output goes to port C, set all other ports off (high)
    PORTA = PORTB = PORTD = 0xff;
    PORTC = 255 ^ decode(current & 0x07);
  } else if (current < 32) {
    // now the output goes to port D, set all other ports off (high)
    PORTA = PORTB = PORTC = 0xff;
    PORTD = 255 ^ decode(current & 0x07);
  } else {
    // This should not happen, so light all LEDs
    PORTA = PORTB = PORTC = PORTD = 0x00;
  }
}

/*
 * This just makes 2 ^ in
 */
int decode(int in) {
  // i used math.h, but this really just biggened up the hex to 4k.
  int out = 0;
  switch(in) {
    case 0: out = 1; break;
    case 1: out = 2; break;
    case 2: out = 4; break;
    case 3: out = 8; break;
    case 4: out = 16; break;
    case 5: out = 32; break;
    case 6: out = 64; break;
    case 7: out = 128; break;
  }
  return out;
}
