#include <stdint.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

//#define __DEBUG__
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
void init_timer(void);
void next_step(void);
void out(int8_t);
static int8_t decode[8] = {1, 2, 4, 8, 16, 32, 64, 128};
#ifdef __DEBUG
void debug(int8_t);
#endif

// LED-Counter defined globally
int8_t current_count = 0;

/*
 * This configures the Timer0 Overflow Interrupt vector to call
 * next_step.
 */
ISR(TIMER0_OVF_vect) {
  next_step();
}

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

  // Initialize ports (later interrupts and so on)
  init();

  // Endless loop.
  for(;1==1;) {
#ifdef __DEBUG__
    debug(8);
#endif
    // Set controller into sleep mode (takes not as much power).
    sleep_mode();
  }

  // finish process.
  return 1;
}

/*
 * void init(void) - Initialize ports and timer
 *
 */
void init(void) {
  // Initialize Ports
  init_ports();
#ifdef __DEBUG__
  debug(1);
#endif
  // Now enable sleep mode
  sleep_enable();
#ifdef __DEBUG__
  debug(2);
#endif
  // Initialize Timer
  init_timer();
#ifdef __DEBUG__
  debug(3);
#endif
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
 * void init_timer(void) - Initializes Timer
 */
void init_timer(void) {
  // Enable Timer0 overflow
  TIMSK = TIMSK | 0x02;
  // Timer prescaler set to CK/1024
  TCCR0 = 0x05;
  // Enable Interrupts
  sei();
}

/*
 * void next_step(void) - Output next step
 */
void next_step(void) {
  // increasing the global counter
  current_count += 1;
  // resetting the counter on overflow
  if(current_count >= (8*4)) {
    current_count = 0;
  }
  // handling of output
  out(current_count);
}

/*
 * Setting the output depending on the value
 */
void out(int8_t current) {
  // Perhaps, there's a more neat way to do this? Any ideas?
  if(current < 8) {
    // now the output goes to port A, set all other ports off (high)
    PORTB = PORTC = PORTD = 0xff;
    PORTA = 255 ^ decode[current & 0x07];
  } else if (current < 16) {
    // now the output goes to port B, set all other ports off (high)
    PORTA = PORTC = PORTD = 0xff;
    PORTB = 255 ^ decode[current & 0x07];
  } else if (current < 24) {
    // now the output goes to port C, set all other ports off (high)
    PORTA = PORTB = PORTD = 0xff;
    PORTC = 255 ^ decode[current & 0x07];
  } else if (current < 32) {
    // now the output goes to port D, set all other ports off (high)
    PORTA = PORTB = PORTC = 0xff;
    PORTD = 255 ^ decode[current & 0x07];
  } else {
    // This should not happen, so light all LEDs
    PORTA = PORTB = PORTC = PORTD = 0x00;
  }
}

#ifdef __DEBUG__
/*
 * This debugging-method outputs the code given to Port D.
 * This can be used for people (like me) that don't have any kind of
 * hardware debugging possibility to find out, what's happening during
 * the programm.
 */
void debug(int8_t code) {
  int8_t before = PORTD;
  PORTD = 0x00;
  _delay_ms(100);
  PORTD = code;
  _delay_ms(200);
  PORTD = before;
}
#endif
