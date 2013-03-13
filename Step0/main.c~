
#include <util/twi.h> 	    
#include <avr/interrupt.h>
#include <avr/io.h>   
#include <stdint.h> 	   
#include "twislave.h"
#include <stdlib.h>       


#define SLAVE_ADRESSE 0x10 

// I2C commands
#define ALL_RELAYS_OFF 	0x17
#define ALL_RELAYS_ON 	0x16
#define RELAY_1_OFF 	0x14
#define RELAY_2_OFF 	0x15
#define RELAY_1_ON 	0x12
#define RELAY_2_ON 	0x13
#define CLEAR_S0_1	0x18
#define CLEAR_S0_2	0x19

#define NO_STATE	0xFF


volatile uint32_t S0_counter1, S0_counter2;
volatile uint32_t S0_counter1_buf, S0_counter2_buf;
volatile uint32_t cnt = 0;

void init_S0_Counters()
{
	S0_counter1 = 0x00;
	S0_counter2 = 0x00;
	S0_counter1_buf = 0x00;
	S0_counter2_buf = 0x00;

	DDRD  &= ~((1<<PD2) | (1<<PD3));    	
	PORTD |= (1<<PD2) | (1<<PD3);	   	

	EICRA |= (1<<ISC11) | (ISC10) | (ISC01) | (ISC00); 	// INT1+INT0 on rising edge
	EIMSK |= (1<<INT1) | (1<<INT0);				// Enable INT1+INT0

  	TCCR1B |= (1<<CS12) | (1<<CS10);			// Timer1 prescaler 1024
	TIMSK1 |= (1<<TOIE1);

	sei(); 			
}


ISR(TIMER1_OVF_vect){			// Timer1 overflow ISR

	if( S0_counter1 == 0x00 && S0_counter2 == 0x00 ){
		i2cdata[1] = ALL_RELAYS_OFF;  
	}else if( S0_counter1 == S0_counter1_buf ) {
		i2cdata[1] = RELAY_1_OFF;
	}else if( S0_counter2 == S0_counter2_buf ) {
		i2cdata[1] = RELAY_2_OFF;
	}

	S0_counter1_buf = S0_counter1;
	S0_counter2_buf = S0_counter2;		

}

ISR(INT0_vect)	{ S0_counter1++; }	// S0_1 count on INT0
ISR(INT1_vect)	{ S0_counter2++; }	// S0_2 count on INT1


int main (void)
{

  DDRB  = 0xFF;	
  PORTB = 0x00; 


  init_twi_slave(SLAVE_ADRESSE);
  init_S0_Counters();


  while(1) 
  {
//-----------------------------------------------------------------------------------------------
// Teil der geändert worden ist
	i2cdata[2] = 0x01; //(S0_counter1 >> 8);	
	i2cdata[3] = 0x02; //S0_counter1;
	i2cdata[4] = 0x03; //(S0_counter2 >> 8);	
	i2cdata[5] = 0x03; //S0_counter2;
//-----------------------------------------------------------------------------------------------
	switch(i2cdata[1]){
        case RELAY_1_ON:  	PORTB |=  (1 << PB1); break;		
  	case RELAY_2_ON:  	PORTB |=  (1 << PB0); break;		
  	case RELAY_1_OFF:  	PORTB &= ~(1 << PB1); break;		
  	case RELAY_2_OFF:  	PORTB &= ~(1 << PB0); break;		
  	case ALL_RELAYS_ON:  	PORTB |=  (1 << PB0) | (1 << PB1); TIMSK1 |= (1<<TOIE1); break;	
  	case ALL_RELAYS_OFF:  	PORTB &= ~((1 << PB0) | (1 << PB1)); break;
	case CLEAR_S0_1: 	S0_counter1 = 0x00; break;
	case CLEAR_S0_2: 	S0_counter2 = 0x00; break;
        default: break;
	
	//i2cdata[1] = 0x00;

     }

  } 

} 



