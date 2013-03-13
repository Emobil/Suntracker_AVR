#include <stdint.h> 	   
#include "twislave.h"
#include <stdlib.h>       
#include <util/twi.h> 	 
#include <avr/interrupt.h>
#include <avr/io.h>

//#include <util/delay.h>   

#define SLAVE_ADRESSE 0x10

/*
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

volatile uint8_t sekunden = 0x00;
volatile uint32_t S0_counter1, S0_counter2;
volatile uint32_t S0_counter1_buf, S0_counter2_buf;
volatile uint32_t cnt = 0x00;

void init_S0_Counters()
{
	S0_counter1 = 0x00;
	S0_counter2 = 0x00;
	S0_counter1_buf = 0x00;
	S0_counter2_buf = 0x00;

	DDRD  &= ~((1<<PD2) | (1<<PD3));    	
	PORTD |= (1<<PD2) | (1<<PD3);	   	

//	EICRA |= (1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00); 	// INT1+INT0 on rising edge
	EICRA |= (1<<ISC11) | (1<<ISC01); 				// INT1+INT0 on falling edge
	EIMSK |= (1<<INT1) | (1<<INT0);					// Enable INT1+INT0

  	TCCR1B |= (1<<CS12) | (1<<CS10);				// Timer1 prescaler 1024

	sei(); 			
}


ISR(TIMER1_OVF_vect){			// Timer1 overflow ISR

	sekunden++;
	if(sekunden == 0x01){
		if( S0_counter1 == S0_counter1_buf && S0_counter2 == S0_counter2_buf ){
			i2cdata[1] = ALL_RELAYS_OFF;  
		}else if( S0_counter1 == S0_counter1_buf ) {
			i2cdata[1] = RELAY_2_OFF;
		}else if( S0_counter2 == S0_counter2_buf ) {
			i2cdata[1] = RELAY_1_OFF;	
		}
	S0_counter1_buf = S0_counter1;
	S0_counter2_buf = S0_counter2;	
	sekunden = 0x00;
	}
	
//	TIMSK1 |= (1<<TOIE1);						// Start Timer1 

}

ISR(INT0_vect)	{ S0_counter1++; }	// S0_1 count on INT0
ISR(INT1_vect)	{ S0_counter2++; }	// S0_2 count on INT1
*/

int main (void)
{

 // DDRB  = 0xFF;	
 // PORTB = 0x00; 
  DDRD &= ~(1 << PD3); // M1 Hall 1 als Eingang 
  DDRC |= (1 << PC0);
  
init_twi_slave(SLAVE_ADRESSE);
 // init_S0_Counters();
	i2cdata[2] = 0x00; //(S0_counter1 >> 8);	
	i2cdata[3] = 0x00; //S0_counter1;
	i2cdata[4] = 0x00; // (S0_counter2 >> 8);	
	i2cdata[5] = 0x00;//S0_counter2;

  while(1) 
  {
i2cdata[1]=0x00;
i2cdata[2]=0x00;
//	_delay_ms(1);

if ( PIND & (1<<PIND3) ) {
	i2cdata[1] = 0x01;
}else
{
	i2cdata[1] = 0x00;
}


if (i2cdata[2] == 0x01)  {
	PORTC |= (1<<PC0); //EN - LAMPE anschalten 
}else
{
	PORTC &= ~(1<<PC0); // EN - Lampe ausschalten
}

	/*switch(i2cdata[1]){
        case RELAY_1_ON:  	PORTB |=  (1 << PB1); break;		
  	case RELAY_2_ON:  	PORTB |=  (1 << PB0); break;		
  	case RELAY_1_OFF:  	PORTB &= ~(1 << PB1); break;		
  	case RELAY_2_OFF:  	PORTB &= ~(1 << PB0); break;		
  	case ALL_RELAYS_ON:  	PORTB |=  (1 << PB0) | (1 << PB1); TIMSK1 |= (1<<TOIE1); break;	
  	case ALL_RELAYS_OFF:  	PORTB &= ~((1 << PB0) | (1 << PB1)); break;
	case CLEAR_S0_1: 	S0_counter1 = 0x00; break;
	case CLEAR_S0_2: 	S0_counter2 = 0x00; break;
        default: break;
     }*/

  } 

} 


