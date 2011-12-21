#include <avr/io.h>
#include "USI_UART_config.h"

void main( void ) {
  unsigned char myString[] = "\n\rYou just sent me: ";
      
  unsigned char counter;
        
  USI_UART_Flush_Buffers();
  USI_UART_Initialise_Receiver();     // Initialisation for USI_UART receiver
  __enable_interrupt();               // Enable global interrupts
			          
  MCUCR = (1<<SE)|(0<<SM1)|(0<<SM0);  // Enable Sleepmode: Idle
       
  for( ; ; ) {                        // Run forever
    if( USI_UART_Data_In_Receive_Buffer() ) {  
      for(counter = 0; counter < 20; counter++) { // Echo myString[]
        USI_UART_Transmit_Byte( (unsigned int)myString[counter] );     
      }
      USI_UART_Transmit_Byte( USI_UART_Receive_Byte() ); // Echo the received character      

    }    

    __sleep();                        // Sleep when waiting for next event
  }    
}
