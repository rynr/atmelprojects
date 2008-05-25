#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "midi.h"

#define __DEBUG__
#ifndef F_CPU
// define cpu click speed
#define F_CPU 3686400
#endif
#include <util/delay.h>

// Calculate Usart frequency configuration
#define USART_UBBR_VALUE ((F_CPU/(MIDI_BAUD_RATE<<4))-1)

void init(void);
void init_midi(void);
void init_ports(void);
void init_interrupts(void);
int8_t check_for_changed(int8_t *last_assignment);
int8_t get_program(int8_t bank, int8_t patch);
void set_data(int8_t bank, int8_t patch);
void transmit_midi_program_change(uint8_t program);
void transmit_midi_message(char a, char b, char c);
void transmit_byte(unsigned char data);
void show_status(uint8_t bank, int8_t patch);

/*
 * int main(void) - Main routine
 *
 * This is the main part of the application. It initializes the
 * controller, then starts a endless loop that will handle input.
 *
 */
int main(void)
{
  // The current bank is stored here (0-7)
  int8_t current_bank = 0;

  // This is used to compare any changes of the foot switches
  int8_t last_foot_assignment = 0;

  // Initialize controller
  init();

  // Get current assignment of the foot switches
  last_foot_assignment = PIND;

  // Endless loop.
  for(;1==1;) {
    _delay_ms(2);
    int8_t changed = check_for_changed(&last_foot_assignment);
    if(changed != 0) {
      switch(changed & 0x3C) {
        case 0x04:
          set_data(current_bank, 0x00);
          break;
        case 0x08:
          set_data(current_bank, 0x01);
          break;
        case 0x10:
          set_data(current_bank, 0x02);
          break;
        case 0x20:
          set_data(current_bank, 0x03);
          break;
      }
      switch(changed & 0x41) {
        case 0x01:
          current_bank--;
          show_status(current_bank, 0x04);
          break;
        case 0x40:
          current_bank++;
          show_status(current_bank, 0x04);
          break;
      }
    }
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
  // Initialize Midi
  init_midi();
}

/*
 * void init_midi(void) - Initialize Midi connection
 *
 * Initializing the Midi connection. It's dependent from frequence of
 * the controller.
 *
 */
void init_midi(void) {
   // Set baud rate
   UBRR = (uint8_t)USART_UBBR_VALUE;
   // Enable transmitter not receiver
   UCR = (0<<RXEN)|(1<<TXEN);
}

/*
 * void init_ports(void) - Initialize Ports
 *
 * Port B is set to input, each pin is a program number to be send as
 * program change.
 *
 */
void init_ports(void) {
  // Port D
  //   PORTD0 (0) Input ↓
  //   PORTD1 (1) TXD
  //   PORTD2 (0) Input Ⓐ
  //   PORTD3 (0) Input Ⓑ
  //   PORTD4 (0) Input Ⓒ
  //   PORTD5 (0) Input Ⓓ
  //   PORTD6 (0) Input ↑
  DDRD = 0x02; // 0 ≘ Input, 1 ≘ Output
  // Port B
  //   PORTB0 (1) Status Ⓐ
  //   PORTB1 (1) Status Ⓑ
  //   PORTB2 (1) Status Ⓒ
  //   PORTB3 (1) Status Ⓓ
  //   PORTB4 (1) Status Patch Bit 1
  //   PORTB5 (1) Status Patch Bit 2
  //   PORTB6 (1) Status Patch Bit 4
  //   PORTB7 (1) Status Patch Bit 8
  DDRB = 0xff; // 0 ≘ Input, 1 ≘ Output
  PORTB = 0x0f;
}

int8_t check_for_changed(int8_t *last_assignment) {
  int8_t result = 0x00;
  int8_t new_assignment = PIND;
  if(*last_assignment != new_assignment) {
    result = *last_assignment ^ new_assignment;
    *last_assignment = new_assignment;
  }
  return result;
}

int8_t get_program(int8_t bank, int8_t patch) {
  return (((bank & 0x0f) << 2) | (patch & 0x03));
}

void set_data(int8_t bank, int8_t patch) {
  transmit_midi_program_change(get_program(bank, patch));
  show_status(bank, patch);
}

void transmit_midi_program_change(uint8_t program) {
#ifdef __DEBUG__
  transmit_byte(program);
#else
  transmit_byte(MIDI_PROGRAM_CHANGE | MIDI_CHAN_1);
  transmit_byte(program);
#endif
}

void transmit_byte(uint8_t data) {
#ifndef __DEBUG__
  // Wait for empty transmit buffer
  while(!(USR & (1 << UDRE)))
  // Start transmittion
  UDR =  data;
#endif
}

void show_status(uint8_t bank, int8_t patch) {
  int8_t patch_represent = 0x00;
  if ((patch & 0xfc) > 0) {
    // If patch is none of 0x00, 0x01, 0x02 or 0x03, show nothing
    patch_represent = 0x0f;
  } else {
    // Otherwise show 0x01, 0x02, 0x04 or 0x08
    patch_represent = 0x0f - (1 << (0x03 & patch));
  }
  PORTB = patch_represent | ((bank & 0x0f) << 4);
}

